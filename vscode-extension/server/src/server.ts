import {
	createConnection,
	TextDocuments,
	Diagnostic,
	DiagnosticSeverity,
	ProposedFeatures,
	InitializeParams,
	DidChangeConfigurationNotification,
	CompletionItem,
	TextDocumentPositionParams,
	TextDocumentSyncKind,
	InitializeResult,
	HoverParams,
	Hover,
	DocumentSymbolParams,
	DocumentSymbol,
	MarkupKind,
	DefinitionParams
} from 'vscode-languageserver/node';
import {URI} from 'vscode-uri';
import {
	Range,
	TextDocument
} from 'vscode-languageserver-textdocument';
import * as childProcess from 'child_process';
import {promisify} from 'util';
import * as os from 'os';
import * as fs from 'fs';
import assert = require('assert');
import path = require('path');

const execFile = promisify(childProcess.execFile);

// Create a connection for the server, using Node's IPC as a transport.
// Also include all preview / proposed LSP features.
const connection = createConnection(ProposedFeatures.all);

// Create a simple text document manager.
const documents: TextDocuments<TextDocument> = new TextDocuments(TextDocument);

let hasConfigurationCapability = false;
let hasWorkspaceFolderCapability = false;
let hasDiagnosticRelatedInformationCapability = false;

connection.onInitialize((params: InitializeParams) => {
	const capabilities = params.capabilities;

	// Does the client support the `workspace/configuration` request?
	// If not, we fall back using global settings.
	hasConfigurationCapability = !!(
		capabilities.workspace && !!capabilities.workspace.configuration
	);
	hasWorkspaceFolderCapability = !!(
		capabilities.workspace && !!capabilities.workspace.workspaceFolders
	);
	hasDiagnosticRelatedInformationCapability = !!(
		capabilities.textDocument &&
		capabilities.textDocument.publishDiagnostics &&
		capabilities.textDocument.publishDiagnostics.relatedInformation
	);

	const result: InitializeResult = {
		capabilities: {
			textDocumentSync: TextDocumentSyncKind.Incremental,
			// Tell the client that this server supports code completion.
			completionProvider: {
				resolveProvider: true
			},
			hoverProvider: true,
			definitionProvider: true,
			documentSymbolProvider: {
				label: 'ZScript',
			},
		}
	};
	if (hasWorkspaceFolderCapability) {
		result.capabilities.workspace = {
			workspaceFolders: {
				supported: true
			}
		};
	}
	return result;
});

connection.onInitialized(() => {
	if (hasConfigurationCapability) {
		// Register for all configuration changes.
		connection.client.register(DidChangeConfigurationNotification.type, undefined);
	}
	if (hasWorkspaceFolderCapability) {
		connection.workspace.onDidChangeWorkspaceFolders(_event => {
			connection.console.log('Workspace folder change event received.');
		});
	}
});

// The example settings
interface Settings {
	installationFolder?: string;
	printCompilerOutput?: boolean;
	defaultIncludeFiles?: Array<string>;
	defaultIncludePaths?: Array<string>;
	ignoreConstAssert?: boolean;
}

// The global settings, used when the `workspace/configuration` request is not supported by the client.
// Please note that this is not the case when using this server with the client provided in this example
// but could happen with other clients.
const defaultSettings: Settings = { };
let globalSettings: Settings = defaultSettings;

// Cache the settings of all open documents
const documentSettings: Map<string, Thenable<Settings>> = new Map();

connection.onDidChangeConfiguration(change => {
	if (hasConfigurationCapability) {
		// Reset all cached document settings
		documentSettings.clear();
	} else {
		globalSettings = <Settings>(
			(change.settings.zscriptLanguageServer || defaultSettings)
		);
	}

	// Revalidate all open text documents
	documents.all().forEach(processScript);
});

function getDocumentSettings(resource: string): Thenable<Settings> {
	if (!hasConfigurationCapability) {
		return Promise.resolve(globalSettings);
	}
	let result = documentSettings.get(resource);
	if (!result) {
		result = connection.workspace.getConfiguration({
			scopeUri: resource,
			section: 'zscript'
		});
		documentSettings.set(resource, result);
	}
	return result;
}

documents.onDidOpen(e => {
	processScript(e.document);
});

// Only keep settings for open documents
documents.onDidClose(e => {
	documentSettings.delete(e.document.uri);
	docMetadataMap.delete(e.document.uri);
});

// The content of a text document has changed. This event is emitted
// when the text document first opened or when its content has changed.
documents.onDidChangeContent(change => {
	processScript(change.document);
});

// TODO: this should not be necessary. Get path in better OS-agnostic way.
function cleanupFile(fname: string)
{
	if (os.platform() !== 'win32')
		return fname.trim();
	return fname.replace(/\//g, '\\').trim();
}
function cleanupFile2(fname: string)
{
	if (os.platform() !== 'win32')
		return fname.trim();

	fname = URI.parse(fname).fsPath;
	if (fname.match(/[a-z]:\\.*/)) {
		// capitalize drive letters
		fname = fname[0].toUpperCase() + fname.slice(1);
	}
	return fname.replace(/\//g, '\\').trim();
}
function fileMatches(f1:string, f2:string)
{
	return cleanupFile(f1) == cleanupFile(f2);
}

function parseOutput(settings: Settings, textDocument: TextDocument, stdout: string, stderr: string): {diagnostics: Diagnostic[], metadata?: DocumentMetaData} {
	if (stdout.startsWith('{')) {
		if (settings.printCompilerOutput) {
			console.log(stderr);
		}
		const result = JSON.parse(stdout) as {diagnostics: Diagnostic[], metadata?: DocumentMetaData};
		return result;
	}

	// For older releases, diagnostics and metadata must be parsed from stdout.

	const [compilerOutput, metadataStr] = stdout.split('=== METADATA', 2);
	if (settings.printCompilerOutput) {
		console.log(compilerOutput);
	}

	const diagnostics: Diagnostic[] = [];
	for (const line of compilerOutput.split('\n')) {
		if (line.includes('syntax error')) {
			const m = line.match(/syntax error, (.*) \[(.*) Line (\d+) Column (\d+).*\].*/);
			let message = '';
			let lineNum = 0;
			let colNum = 0;
			let fname = '';
			if (m) {
				fname = cleanupFile(m[2]);
				message = m[1].trim();
				if (fileMatches(fname, "ZQ_BUFFER") || fileMatches(fname, tmpInput))
				{
					lineNum = 0;
					colNum = 0;
					message = `Syntax error in temp file (check your ZScript Extension settings):\n${message}`;
				}
				else if (fileMatches(fname, tmpScript))
				{
					lineNum = Number(m[3]) - 1;
					colNum = Number(m[4]);
				}
				else
				{
					lineNum = 0;
					colNum = 0;
					message = `Syntax error in "${fname}":\n${message}`;
				}
			} else {
				message = line.split('syntax error, ', 2)[1].trim();
			}

			const start = textDocument.offsetAt({line: lineNum, character: 0});
			const end = textDocument.offsetAt({line: lineNum, character: colNum});
			const diagnostic: Diagnostic = {
				severity: DiagnosticSeverity.Error,
				range: {
					start: textDocument.positionAt(start),
					end: textDocument.positionAt(end),
				},
				message,
				source: 'zscript',
			};
			diagnostics.push(diagnostic);
		} else if (line.includes('Error') || line.includes('Warning')) {
			const m = line.match(/(.*)Line (\d+).*Columns (\d+)-(\d+) - (.*)/);
			let message = '';
			let lineNum = 0;
			let colStartNum = 0;
			let colEndNum = 0;
			let sev = 'Warning';
			let fname = '';
			if (m) {
				fname = cleanupFile(m[1]);
				message = m[5].trim();
				if (message.startsWith('Error'))
					sev = 'Error';
				if (fname.length == 0 || fileMatches(fname, "ZQ_BUFFER") || fileMatches(fname, tmpInput)) //error in temp file
				{
					lineNum = 0;
					colStartNum = 0;
					colEndNum = 0;
					message = `${sev} in temp file (check your ZScript Extension settings):\n${message}`;
				}
				else if(fileMatches(fname, tmpScript))
				{
					lineNum = Number(m[2]) - 1;
					colStartNum = Number(m[3]) - 1;
					colEndNum = Number(m[4]) - 1;
				}
				else
				{
					lineNum = 0;
					colStartNum = 0;
					colEndNum = 0;
					message = `${sev} in "${fname}":\n${message}`;
				}
			} else {
				message = line.trim();
			}

			const start = textDocument.offsetAt({ line: lineNum, character: colStartNum });
			const end = textDocument.offsetAt({ line: lineNum, character: colEndNum });
			const diagnostic: Diagnostic = {
				severity: sev=='Error' ? DiagnosticSeverity.Error : DiagnosticSeverity.Warning,
				range: {
					start: textDocument.positionAt(start),
					end: textDocument.positionAt(end),
				},
				message,
				source: 'zscript',
			};
			diagnostics.push(diagnostic);
		} else if (line.includes('ERROR:') || line.includes('WARNING:')) {
			const m = line.match(/(.*) \[(.*) Line (\d+) Column (\d+).*/);
			let message = '';
			let lineNum = 0;
			let colNum = 0;
			let sev = 'Warning';
			let fname = '';
			if (m) {
				fname = cleanupFile(m[2]);
				message = m[1].trim();
				if(message.startsWith('ERROR:'))
					sev = 'Error';
				if (fileMatches(fname, "ZQ_BUFFER") || fileMatches(fname, tmpInput))
				{
					lineNum = 0;
					colNum = 0;
					message = `${sev} in temp file (check your ZScript Extension settings):\n${message}`;
				}
				else if (fileMatches(fname, tmpScript))
				{
					lineNum = Number(m[3]) - 1;
					colNum = Number(m[4]);
				}
				else
				{
					lineNum = 0;
					colNum = 0;
					message = `${sev} in "${fname}":\n${message}`;
				}
			} else {
				message = line.trim();
			}

			const start = textDocument.offsetAt({ line: lineNum, character: 0 });
			const end = textDocument.offsetAt({ line: lineNum, character: colNum });
			const diagnostic: Diagnostic = {
				severity: sev=='Error' ? DiagnosticSeverity.Error : DiagnosticSeverity.Warning,
				range: {
					start: textDocument.positionAt(start),
					end: textDocument.positionAt(end),
				},
				message,
				source: 'zscript',
			};
			diagnostics.push(diagnostic);
		}
	}

	let metadata;
	try {
		metadata = JSON.parse(metadataStr);
	} catch (e: any) {
		connection.console.error(e.toString());
	}

	return {diagnostics, metadata};
}

interface SymbolPos {
	line: number;
	character: number;
	length: number;
}

interface DocumentMetaData {
	currentFileSymbols: DocumentSymbol[];
	symbols: Record<number, {doc?: string, loc: {range: Range, uri: string}}>;
	identifiers: Array<{
		loc: SymbolPos,
		symbol: number,
	}>;
}
const docMetadataMap = new Map<string, DocumentMetaData>();

const globalTmpDir = os.tmpdir();
const tmpInput = cleanupFile(`${globalTmpDir}/tmp2.zs`);
const tmpScript = cleanupFile(`${globalTmpDir}/tmp.zs`);

async function processScript(textDocument: TextDocument): Promise<void> {
	const settings = await getDocumentSettings(textDocument.uri);
	const text = textDocument.getText();
	let includeText = "#option NO_ERROR_HALT on\n#option HEADER_GUARD on\n";

	if (!settings.installationFolder) {
		connection.sendDiagnostics({ uri: textDocument.uri, diagnostics: [{
			severity: DiagnosticSeverity.Error,
			range: {
				start: textDocument.positionAt(0),
				end: textDocument.positionAt(0),
			},
			message: 'Must set zscript.installationFolder setting',
			source: 'extension'
		}]});
		return;
	}

	if (settings.defaultIncludePaths)
	{
		settings.defaultIncludePaths.forEach(str => {
			includeText += `#includepath "${str}"\n`;
		});
	}
	if (settings.defaultIncludeFiles)
	{
		settings.defaultIncludeFiles.forEach(str =>{
			includeText += `#include "${str}"\n`;
		});
	}

	includeText += `#include "${tmpScript}"\n`;
	let stdout = '';
	let stderr = '';
	let success = false;
	fs.writeFileSync(tmpInput, includeText);
	fs.writeFileSync(tmpScript, text);
	const exe = os.platform() === 'win32' ? './zscript.exe' : './zscript';
	if (settings.printCompilerOutput) {
		console.log(`Attempting to compile buffer:\n-----\n${includeText}\n-----`);
	}
	try {
		let originPath = URI.parse(textDocument.uri).fsPath;
		if(originPath.match(/[a-z]:\\.*/))
		{
			const letter = originPath.at(0);
			if(letter) //capitalize drive letters
				originPath = letter.toUpperCase()+originPath.slice(1);
		}
		const args = [
			'-unlinked',
			'-delay_cassert',
			'-input', tmpInput,
			'-force_ignore', originPath,
			'-metadata',
			'-metadata-tmp-path', tmpScript,
			'-metadata-orig-path', originPath,
			'-json',
		];
		if (settings.ignoreConstAssert)
			args.push('-ignore_cassert');
		const cp = await execFile(exe, args, {
			cwd: settings.installationFolder,
			maxBuffer: 20_000_000,
		});
		success = true;
		stdout = cp.stdout;
		stderr = cp.stderr;
	} catch (e: any) {
		console.error(e);
		if (e.code === undefined) throw e;
		stdout = e.stdout || e.toString();
	}

	const {diagnostics, metadata} = parseOutput(settings, textDocument, stdout, stderr);

	// Fallback, incase compiling failed but we failed to parse out an error.
	if (!success && diagnostics.length === 0) {
		diagnostics.push({
			severity: DiagnosticSeverity.Error,
			range: {
				start: textDocument.positionAt(0),
				end: textDocument.positionAt(0),
			},
			message: [stdout, stderr].join('\n\n'),
			source: 'zscript',
		});
	}

	// Send the computed diagnostics to VSCode.
	connection.sendDiagnostics({ uri: textDocument.uri, diagnostics });

	if (!metadata) return;

	docMetadataMap.set(textDocument.uri, metadata);
}

connection.onDidChangeWatchedFiles(_change => {
	// Monitored files have change in VSCode
	connection.console.log('We received an file change event');
});

// This handler provides the initial list of the completion items.
connection.onCompletion(
	(_textDocumentPosition: TextDocumentPositionParams): CompletionItem[] => {
		return [];
		// The pass parameter contains the position of the text document in
		// which code complete got requested. For the example we ignore this
		// info and always provide the same completion items.
		// return [
		// 	{
		// 		label: 'TypeScript',
		// 		kind: CompletionItemKind.Text,
		// 		data: 1
		// 	},
		// 	{
		// 		label: 'JavaScript',
		// 		kind: CompletionItemKind.Text,
		// 		data: 2
		// 	}
		// ];
	}
);

// This handler resolves additional information for the item selected in
// the completion list.
connection.onCompletionResolve(
	(item: CompletionItem): CompletionItem => {
		// if (item.data === 1) {
		// 	item.detail = 'TypeScript details';
		// 	item.documentation = 'TypeScript documentation';
		// } else if (item.data === 2) {
		// 	item.detail = 'JavaScript details';
		// 	item.documentation = 'JavaScript documentation';
		// }
		return item;
	}
);

connection.onHover((item: HoverParams): Hover | null => {
	const metadata = docMetadataMap.get(item.textDocument.uri);
	if (!metadata)
		return null;

	const pos = item.position;
	const identifier = metadata.identifiers.find(ident =>
		ident.loc.line == pos.line &&
		pos.character >= ident.loc.character &&
		pos.character < ident.loc.character + ident.loc.length);
	if (!identifier)
		return null;

	const value = metadata.symbols[identifier.symbol].doc;
	if (!value)
		return null;

	return {
		contents: {kind: MarkupKind.Markdown, value},
	};
});

connection.onDocumentSymbol((p: DocumentSymbolParams) => {
	return docMetadataMap.get(p.textDocument.uri)?.currentFileSymbols;
});

connection.onDefinition((p: DefinitionParams) => {
	const metadata = docMetadataMap.get(p.textDocument.uri);
	if (!metadata)
		return null;

	const pos = p.position;
	const identifier = metadata.identifiers.find(ident =>
		ident.loc.line == pos.line &&
		pos.character >= ident.loc.character &&
		pos.character < ident.loc.character + ident.loc.length);
	if (!identifier)
		return null;

	const symbol = metadata.symbols[identifier.symbol];
	if (cleanupFile2(symbol.loc.uri) === tmpScript || symbol.loc.uri === 'file://' + tmpScript)
		symbol.loc.uri = p.textDocument.uri;
	return symbol.loc;
});

// Make the text document manager listen on the connection
// for open, change and close text document events
documents.listen(connection);

// Listen on the connection
connection.listen();
