#include "base/zdefs.h"
#include "zquest.h"
#include "zq_misc.h"
#include "zq_hotkey.h"

extern int32_t prv_mode;
extern bool placing_flags;

int onSetNewLayer(int newlayer);
int onScreenLPal(int lpal);
int32_t do_OpenQuest();
int32_t onSelectFFCombo();
int onReloadPreview();
int onScrollScreen(int dir, bool warp);
int32_t onComboColLeft();
int32_t onComboColRight();
int32_t onComboColUp();
int32_t onComboColDown();

char const* get_hotkey_name(int hkey)
{
	switch(hkey)
	{
		case ZQKEY_UNDO: return "ZQKEY_UNDO";
		case ZQKEY_REDO: return "ZQKEY_REDO";
		case ZQKEY_PLUS_FLAG: return "ZQKEY_PLUS_FLAG";
		case ZQKEY_MINUS_FLAG: return "ZQKEY_MINUS_FLAG";
		case ZQKEY_SAVE: return "ZQKEY_SAVE";
		case ZQKEY_SAVEAS: return "ZQKEY_SAVEAS";
		case ZQKEY_OPEN: return "ZQKEY_OPEN";
		case ZQKEY_SCREEN_PAL: return "ZQKEY_SCREEN_PAL";
		case ZQKEY_SECRET_COMBO: return "ZQKEY_SECRET_COMBO";
		case ZQKEY_DOORS: return "ZQKEY_DOORS";
		case ZQKEY_FFCS: return "ZQKEY_FFCS";
		case ZQKEY_FLAGS: return "ZQKEY_FLAGS";
		case ZQKEY_SCRDATA: return "ZQKEY_SCRDATA";
		case ZQKEY_TILEWARP: return "ZQKEY_TILEWARP";
		case ZQKEY_SIDEWARP: return "ZQKEY_SIDEWARP";
		case ZQKEY_LAYERS: return "ZQKEY_LAYERS";
		case ZQKEY_RESET_TRANSP: return "ZQKEY_RESET_TRANSP";
		case ZQKEY_COPY: return "ZQKEY_COPY";
		case ZQKEY_TOGGLE_DARK: return "ZQKEY_TOGGLE_DARK";
		case ZQKEY_ENEMIES: return "ZQKEY_ENEMIES";
		case ZQKEY_SHOW_FLAGS: return "ZQKEY_SHOW_FLAGS";
		case ZQKEY_ITEMS: return "ZQKEY_ITEMS";
		case ZQKEY_COMBOS: return "ZQKEY_COMBOS";
		case ZQKEY_DARK_PREVIEW: return "ZQKEY_DARK_PREVIEW";
		case ZQKEY_SHOW_INFO: return "ZQKEY_SHOW_INFO";
		case ZQKEY_DRAWING_MODE: return "ZQKEY_DRAWING_MODE";
		case ZQKEY_GOTO_PAGE: return "ZQKEY_GOTO_PAGE";
		case ZQKEY_SHOW_CMB_CS_INFO: return "ZQKEY_SHOW_CMB_CS_INFO";
		case ZQKEY_STRINGS: return "ZQKEY_STRINGS";
		case ZQKEY_ROOM: return "ZQKEY_ROOM";
		case ZQKEY_TILES: return "ZQKEY_TILES";
		case ZQKEY_PASTE: return "ZQKEY_PASTE";
		case ZQKEY_PASTEALL: return "ZQKEY_PASTEALL";
		case ZQKEY_PASTETOALL: return "ZQKEY_PASTETOALL";
		case ZQKEY_PASTEALLTOALL: return "ZQKEY_PASTEALLTOALL";
		case ZQKEY_SHOW_SOLID: return "ZQKEY_SHOW_SOLID";
		case ZQKEY_PREV_MODE: return "ZQKEY_PREV_MODE";
		case ZQKEY_COMPILE_ZSCRIPT: return "ZQKEY_COMPILE_ZSCRIPT";
		case ZQKEY_SCREENSHOT: return "ZQKEY_SCREENSHOT";
		case ZQKEY_ABOUT: return "ZQKEY_ABOUT";
		case ZQKEY_MINUS_MAP: return "ZQKEY_MINUS_MAP";
		case ZQKEY_PLUS_MAP: return "ZQKEY_PLUS_MAP";
		case ZQKEY_MINUS_COLOR: return "ZQKEY_MINUS_COLOR";
		case ZQKEY_PLUS_COLOR: return "ZQKEY_PLUS_COLOR";
		case ZQKEY_MINUS_SCR_PAL: return "ZQKEY_MINUS_SCR_PAL";
		case ZQKEY_PLUS_SCR_PAL: return "ZQKEY_PLUS_SCR_PAL";
		case ZQKEY_MINUS_16_SCR_PAL: return "ZQKEY_MINUS_16_SCR_PAL";
		case ZQKEY_PLUS_16_SCR_PAL: return "ZQKEY_PLUS_16_SCR_PAL";
		case ZQKEY_GRID: return "ZQKEY_GRID";
		case ZQKEY_GRID_COLOR: return "ZQKEY_GRID_COLOR";
		case ZQKEY_COMBO_COL_MODE: return "ZQKEY_COMBO_COL_MODE";
		case ZQKEY_DELETE: return "ZQKEY_DELETE";
		case ZQKEY_FULLSCREEN: return "ZQKEY_FULLSCREEN";
		case ZQKEY_LYR_0: return "ZQKEY_LYR_0";
		case ZQKEY_LYR_1: return "ZQKEY_LYR_1";
		case ZQKEY_LYR_2: return "ZQKEY_LYR_2";
		case ZQKEY_LYR_3: return "ZQKEY_LYR_3";
		case ZQKEY_LYR_4: return "ZQKEY_LYR_4";
		case ZQKEY_LYR_5: return "ZQKEY_LYR_5";
		case ZQKEY_LYR_6: return "ZQKEY_LYR_6";
		case ZQKEY_SCR_LPAL_0: return "ZQKEY_SCR_LPAL_0";
		case ZQKEY_SCR_LPAL_1: return "ZQKEY_SCR_LPAL_1";
		case ZQKEY_SCR_LPAL_2: return "ZQKEY_SCR_LPAL_2";
		case ZQKEY_SCR_LPAL_3: return "ZQKEY_SCR_LPAL_3";
		case ZQKEY_SCR_LPAL_4: return "ZQKEY_SCR_LPAL_4";
		case ZQKEY_SCR_LPAL_5: return "ZQKEY_SCR_LPAL_5";
		case ZQKEY_SCR_LPAL_6: return "ZQKEY_SCR_LPAL_6";
		case ZQKEY_SCR_LPAL_7: return "ZQKEY_SCR_LPAL_7";
		case ZQKEY_SCR_LPAL_8: return "ZQKEY_SCR_LPAL_8";
		case ZQKEY_SCR_LPAL_9: return "ZQKEY_SCR_LPAL_9";
		case ZQKEY_SCR_LPAL_10: return "ZQKEY_SCR_LPAL_10";
		case ZQKEY_SCR_LPAL_11: return "ZQKEY_SCR_LPAL_11";
		case ZQKEY_SCR_LPAL_12: return "ZQKEY_SCR_LPAL_12";
		case ZQKEY_SCR_LPAL_13: return "ZQKEY_SCR_LPAL_13";
		case ZQKEY_SCR_LPAL_14: return "ZQKEY_SCR_LPAL_14";
		case ZQKEY_SCR_LPAL_15: return "ZQKEY_SCR_LPAL_15";
		case ZQKEY_SCROLL_SCREEN_UP: return "ZQKEY_SCROLL_SCREEN_UP";
		case ZQKEY_SCROLL_SCREEN_DOWN: return "ZQKEY_SCROLL_SCREEN_DOWN";
		case ZQKEY_SCROLL_SCREEN_LEFT: return "ZQKEY_SCROLL_SCREEN_LEFT";
		case ZQKEY_SCROLL_SCREEN_RIGHT: return "ZQKEY_SCROLL_SCREEN_RIGHT";
		case ZQKEY_WARP_SCREEN_UP: return "ZQKEY_WARP_SCREEN_UP";
		case ZQKEY_WARP_SCREEN_DOWN: return "ZQKEY_WARP_SCREEN_DOWN";
		case ZQKEY_WARP_SCREEN_LEFT: return "ZQKEY_WARP_SCREEN_LEFT";
		case ZQKEY_WARP_SCREEN_RIGHT: return "ZQKEY_WARP_SCREEN_RIGHT";
		case ZQKEY_SCROLL_COMBO_UP: return "ZQKEY_SCROLL_COMBO_UP";
		case ZQKEY_SCROLL_COMBO_DOWN: return "ZQKEY_SCROLL_COMBO_DOWN";
		case ZQKEY_SCROLL_COMBO_LEFT: return "ZQKEY_SCROLL_COMBO_LEFT";
		case ZQKEY_SCROLL_COMBO_RIGHT: return "ZQKEY_SCROLL_COMBO_RIGHT";
		case ZQKEY_COMBO_PAGEUP: return "ZQKEY_COMBO_PAGEUP";
		case ZQKEY_COMBO_PAGEDN: return "ZQKEY_COMBO_PAGEDN";
	}
	return "ZQ_NIL_KEY";
}

bool Hotkey::check(int k,int shifts,bool exact)
{
	for(int q = 0; q <= 1; ++q)
	{
		int s = shifts & (exact ? HOTKEY_FLAG_FILTER : modflag[q]);
		if(key[q] == k && modflag[q] == s)
			return true;
	}
	return false;
}
int Hotkey::getval() const
{
	return (key[0]&0xFF)<<0 |
		(modflag[0]&HOTKEY_FLAG_FILTER)<<8 |
		(key[1]&0xFF)<<16 |
		(modflag[1]&HOTKEY_FLAG_FILTER)<<24;
}
void Hotkey::setval(int val)
{
	key[0] = (val>>0)&0xFF;
	modflag[0] = (val>>8)&HOTKEY_FLAG_FILTER;
	key[1] = (val>>16)&0xFF;
	modflag[1] = (val>>24)&HOTKEY_FLAG_FILTER;
	for(int q = 0; q < 2; ++q)
		if(modflag[q] & KB_COMMAND_FLAG)
		{
			modflag[q] &= ~KB_COMMAND_FLAG;
			modflag[q] |= KB_CTRL_FLAG;
		}
}
void Hotkey::setval(int ind,int k,int shifts)
{
	if(shifts & KB_COMMAND_FLAG)
	{
		shifts &= ~KB_COMMAND_FLAG;
		shifts |= KB_CTRL_FLAG;
	}
	key[ind] = k&0xFF;
	modflag[ind] = shifts&HOTKEY_FLAG_FILTER;
}
void Hotkey::setval(int k,int shifts,int k2,int shifts2)
{
	if(shifts & KB_COMMAND_FLAG)
	{
		shifts &= ~KB_COMMAND_FLAG;
		shifts |= KB_CTRL_FLAG;
	}
	if(shifts2 & KB_COMMAND_FLAG)
	{
		shifts2 &= ~KB_COMMAND_FLAG;
		shifts2 |= KB_CTRL_FLAG;
	}
	key[0] = k&0xFF;
	modflag[0] = shifts&HOTKEY_FLAG_FILTER;
	key[1] = k2&0xFF;
	modflag[1] = shifts2&HOTKEY_FLAG_FILTER;
}

Hotkey zq_hotkeys[ZQKEY_MAX];

bool is_modkey(int c)
{
	switch(c)
	{
		case KEY_LSHIFT:
		case KEY_RSHIFT:
		case KEY_LCONTROL:
		case KEY_RCONTROL:
		case KEY_COMMAND:
		case KEY_ALT:
		case KEY_ALTGR:
			return true;
	}
	return false;
}
bool is_reserved_key(int c)
{
	switch(c)
	{
		case KEY_ESC: case KEY_F1:
			return true;
	}
	return false;
}

void default_hotkeys()
{
	zq_hotkeys[ZQKEY_UNDO].setval(KEY_Z,KB_CTRL_FLAG, KEY_U,0);
	zq_hotkeys[ZQKEY_REDO].setval(KEY_Z,KB_CTRL_FLAG|KB_SHIFT_FLAG, KEY_Y,KB_CTRL_FLAG);
	zq_hotkeys[ZQKEY_PLUS_FLAG].setval(KEY_SLASH_PAD,0,KEY_OPENBRACE,0);
	zq_hotkeys[ZQKEY_MINUS_FLAG].setval(KEY_ASTERISK,0,KEY_CLOSEBRACE,0);
	zq_hotkeys[ZQKEY_SAVE].setval(KEY_F2,0,KEY_S,KB_CTRL_FLAG);
	zq_hotkeys[ZQKEY_SAVEAS].setval(0,0,0,0);
	zq_hotkeys[ZQKEY_OPEN].setval(KEY_F3,0,KEY_O,KB_CTRL_FLAG);
	zq_hotkeys[ZQKEY_SCREEN_PAL].setval(KEY_F4,0,0,0);
	zq_hotkeys[ZQKEY_SECRET_COMBO].setval(KEY_F5,0,0,0);
	zq_hotkeys[ZQKEY_DOORS].setval(KEY_F6,0,0,0);
	zq_hotkeys[ZQKEY_FFCS].setval(KEY_F7,0,0,0);
	zq_hotkeys[ZQKEY_FLAGS].setval(KEY_F8,0,0,0);
	zq_hotkeys[ZQKEY_SCRDATA].setval(KEY_F9,0,0,0);
	zq_hotkeys[ZQKEY_TILEWARP].setval(KEY_F10,0,0,0);
	zq_hotkeys[ZQKEY_SIDEWARP].setval(KEY_F11,0,0,0);
	zq_hotkeys[ZQKEY_LAYERS].setval(KEY_F12,0,0,0);
	zq_hotkeys[ZQKEY_RESET_TRANSP].setval(KEY_B,0,0,0);
	zq_hotkeys[ZQKEY_COPY].setval(KEY_C,0,0,0);
	zq_hotkeys[ZQKEY_TOGGLE_DARK].setval(KEY_D,0,0,0);
	zq_hotkeys[ZQKEY_ENEMIES].setval(KEY_E,0,0,0);
	zq_hotkeys[ZQKEY_SHOW_FLAGS].setval(KEY_F,0,0,0);
	zq_hotkeys[ZQKEY_ITEMS].setval(KEY_I,0,0,0);
	zq_hotkeys[ZQKEY_COMBOS].setval(KEY_K,0,0,0);
	zq_hotkeys[ZQKEY_DARK_PREVIEW].setval(KEY_L,0,0,0);
	zq_hotkeys[ZQKEY_SHOW_INFO].setval(KEY_N,0,0,0);
	zq_hotkeys[ZQKEY_DRAWING_MODE].setval(KEY_O,0,0,0);
	zq_hotkeys[ZQKEY_GOTO_PAGE].setval(KEY_P,0,0,0);
	zq_hotkeys[ZQKEY_SHOW_CMB_CS_INFO].setval(KEY_Q,0,0,0);
	zq_hotkeys[ZQKEY_ROOM].setval(KEY_R,0,0,0);
	zq_hotkeys[ZQKEY_STRINGS].setval(KEY_S,0,0,0);
	zq_hotkeys[ZQKEY_TILES].setval(KEY_T,0,0,0);
	zq_hotkeys[ZQKEY_PASTE].setval(KEY_V,0,0,0);
	zq_hotkeys[ZQKEY_PASTEALL].setval(KEY_V,KB_SHIFT_FLAG,0,0);
	zq_hotkeys[ZQKEY_PASTETOALL].setval(KEY_V,KB_CTRL_FLAG,0,0);
	zq_hotkeys[ZQKEY_PASTEALLTOALL].setval(KEY_V,KB_CTRL_FLAG|KB_SHIFT_FLAG,0,0);
	zq_hotkeys[ZQKEY_SHOW_SOLID].setval(KEY_W,0,0,0);
	zq_hotkeys[ZQKEY_PREV_MODE].setval(KEY_X,0,0,0);
	zq_hotkeys[ZQKEY_COMPILE_ZSCRIPT].setval(KEY_Y,0,0,0);
	zq_hotkeys[ZQKEY_SCREENSHOT].setval(KEY_Z,0,0,0);
	zq_hotkeys[ZQKEY_ABOUT].setval(KEY_SLASH,KB_SHIFT_FLAG,0,0);
	zq_hotkeys[ZQKEY_MINUS_MAP].setval(KEY_COMMA,0,0,0);
	zq_hotkeys[ZQKEY_PLUS_MAP].setval(KEY_STOP,0,0,0);
	zq_hotkeys[ZQKEY_MINUS_COLOR].setval(KEY_MINUS,0,KEY_MINUS_PAD,0);
	zq_hotkeys[ZQKEY_PLUS_COLOR].setval(KEY_EQUALS,0,KEY_PLUS_PAD,0);
	zq_hotkeys[ZQKEY_MINUS_SCR_PAL].setval(KEY_MINUS,KB_SHIFT_FLAG,KEY_MINUS_PAD,KB_SHIFT_FLAG);
	zq_hotkeys[ZQKEY_PLUS_SCR_PAL].setval(KEY_EQUALS,KB_SHIFT_FLAG,KEY_PLUS_PAD,KB_SHIFT_FLAG);
	zq_hotkeys[ZQKEY_MINUS_16_SCR_PAL].setval(KEY_MINUS,KB_CTRL_FLAG,KEY_MINUS_PAD,KB_CTRL_FLAG);
	zq_hotkeys[ZQKEY_PLUS_16_SCR_PAL].setval(KEY_EQUALS,KB_CTRL_FLAG,KEY_PLUS_PAD,KB_CTRL_FLAG);
	zq_hotkeys[ZQKEY_GRID].setval(KEY_TILDE,0,0,0);
	zq_hotkeys[ZQKEY_GRID_COLOR].setval(KEY_TILDE,KB_CTRL_FLAG,0,0);
	zq_hotkeys[ZQKEY_COMBO_COL_MODE].setval(KEY_SPACE,0,0,0);
	zq_hotkeys[ZQKEY_DELETE].setval(KEY_DEL,0,KEY_DEL_PAD,0);
	zq_hotkeys[ZQKEY_FULLSCREEN].setval(KEY_ENTER,KB_ALT_FLAG,KEY_ENTER_PAD,KB_ALT_FLAG);
	for(int q = 0; q <= 6; ++q) //0-6, +pad
		zq_hotkeys[ZQKEY_LYR_0+q].setval(KEY_0+q,0,KEY_0_PAD+q,0);
	for(int q = 0; q <= 15; ++q)
	{
		if(q < 10) //Ctrl+(0-9 + pad), Ctrl+Shift+(0-5 + pad)
			zq_hotkeys[ZQKEY_SCR_LPAL_0+q].setval(KEY_0+q,KB_CTRL_FLAG,KEY_0_PAD+q,KB_CTRL_FLAG);
		else zq_hotkeys[ZQKEY_SCR_LPAL_0+q].setval(KEY_0+(q-10),KB_CTRL_FLAG|KB_SHIFT_FLAG,
			KEY_0_PAD+(q-10),KB_CTRL_FLAG|KB_SHIFT_FLAG);
	}
	zq_hotkeys[ZQKEY_SCROLL_SCREEN_UP].setval(KEY_UP,0,0,0);
	zq_hotkeys[ZQKEY_SCROLL_SCREEN_DOWN].setval(KEY_DOWN,0,0,0);
	zq_hotkeys[ZQKEY_SCROLL_SCREEN_LEFT].setval(KEY_LEFT,0,0,0);
	zq_hotkeys[ZQKEY_SCROLL_SCREEN_RIGHT].setval(KEY_RIGHT,0,0,0);
	zq_hotkeys[ZQKEY_WARP_SCREEN_UP].setval(KEY_UP,KB_CTRL_FLAG,0,0);
	zq_hotkeys[ZQKEY_WARP_SCREEN_DOWN].setval(KEY_DOWN,KB_CTRL_FLAG,0,0);
	zq_hotkeys[ZQKEY_WARP_SCREEN_LEFT].setval(KEY_LEFT,KB_CTRL_FLAG,0,0);
	zq_hotkeys[ZQKEY_WARP_SCREEN_RIGHT].setval(KEY_RIGHT,KB_CTRL_FLAG,0,0);
	zq_hotkeys[ZQKEY_SCROLL_COMBO_UP].setval(KEY_UP,KB_SHIFT_FLAG,0,0);
	zq_hotkeys[ZQKEY_SCROLL_COMBO_DOWN].setval(KEY_DOWN,KB_SHIFT_FLAG,0,0);
	zq_hotkeys[ZQKEY_SCROLL_COMBO_LEFT].setval(KEY_LEFT,KB_SHIFT_FLAG,0,0);
	zq_hotkeys[ZQKEY_SCROLL_COMBO_RIGHT].setval(KEY_RIGHT,KB_SHIFT_FLAG,0,0);
	zq_hotkeys[ZQKEY_COMBO_PAGEUP].setval(KEY_PGUP,0,0,0);
	zq_hotkeys[ZQKEY_COMBO_PAGEDN].setval(KEY_PGDN,0,0,0);
}

void load_hotkeys()
{
	default_hotkeys();
	for(int q = 0; q < ZQKEY_MAX; ++q)
	{
		int v = zq_hotkeys[q].getval();
		int nv = zc_get_config("ZQ_HOTKEY",get_hotkey_name(q),v);
		if(v!=nv)
			zq_hotkeys[q].setval(nv);
	}
}

int run_hotkey(int hkey)
{
	switch(hkey)
	{
		case ZQKEY_UNDO:
			return onUndo();
		case ZQKEY_REDO:
			return onRedo();
		case ZQKEY_PLUS_FLAG:
			return onIncreaseFlag();
		case ZQKEY_MINUS_FLAG:
			return onDecreaseFlag();
		case ZQKEY_SAVE:
			if(saved|disable_saving|OverwriteProtection)
				break;
			return onSave();
		case ZQKEY_SAVEAS:
			return onSaveAs();
		case ZQKEY_OPEN:
			return do_OpenQuest();
		case ZQKEY_SCREEN_PAL:
			return onScreenPalette();
		case ZQKEY_SECRET_COMBO:
			return onSecretCombo();
		case ZQKEY_DOORS:
			return onDoors();
		case ZQKEY_FFCS:
			return onSelectFFCombo();
		case ZQKEY_FLAGS:
			return onFlags();
		case ZQKEY_SCRDATA:
			return onScrData();
		case ZQKEY_TILEWARP:
			return onTileWarp();
		case ZQKEY_SIDEWARP:
			return onSideWarp();
		case ZQKEY_LAYERS:
			return onLayers();
		case ZQKEY_RESET_TRANSP:
			return onResetTransparency();
		case ZQKEY_COPY:
			return onCopy();
		case ZQKEY_TOGGLE_DARK:
			return onToggleDarkness();
		case ZQKEY_ENEMIES:
			return onEnemies();
		case ZQKEY_SHOW_FLAGS:
			return onShowFlags();
		case ZQKEY_ITEMS:
			return onItem();
		case ZQKEY_COMBOS:
			return onCombos();
		case ZQKEY_DARK_PREVIEW:
			return onShowDarkness();
		case ZQKEY_SHOW_INFO:
			return onToggleShowInfo();
		case ZQKEY_DRAWING_MODE:
			return onDrawingMode();
		case ZQKEY_GOTO_PAGE:
			return onGotoPage();
		case ZQKEY_SHOW_CMB_CS_INFO:
			return onShowComboInfoCSet();
		case ZQKEY_ROOM:
			return onRoom();
		case ZQKEY_STRINGS:
			return onStrings();
		case ZQKEY_TILES:
			return onTiles();
		case ZQKEY_PASTE:
			return onPaste();
		case ZQKEY_PASTEALL:
			return onPasteAll();
		case ZQKEY_PASTETOALL:
			return onPasteToAll();
		case ZQKEY_PASTEALLTOALL:
			return onPasteAllToAll();
		case ZQKEY_SHOW_SOLID:
			return onShowWalkability();
		case ZQKEY_PREV_MODE:
			return onPreviewMode();
		case ZQKEY_COMPILE_ZSCRIPT:
			return onCompileScript();
		case ZQKEY_SCREENSHOT:
			return onSnapshot();
		case ZQKEY_ABOUT:
			return onAbout();
		case ZQKEY_MINUS_MAP:
			return onDecMap();
		case ZQKEY_PLUS_MAP:
			return onIncMap();
		case ZQKEY_MINUS_COLOR:
			return onDecreaseCSet();
		case ZQKEY_PLUS_COLOR:
			return onIncreaseCSet();
		case ZQKEY_MINUS_SCR_PAL:
			return onDecScrPal();
		case ZQKEY_PLUS_SCR_PAL:
			return onIncScrPal();
		case ZQKEY_MINUS_16_SCR_PAL:
			return onDecScrPal16();
		case ZQKEY_PLUS_16_SCR_PAL:
			return onIncScrPal16();
		case ZQKEY_GRID:
			return onToggleGrid(false);
		case ZQKEY_GRID_COLOR:
			return onToggleGrid(true);
			
		case ZQKEY_COMBO_COL_MODE:
			return onSpacebar();
		case ZQKEY_DELETE:
			return onDelete();
		case ZQKEY_FULLSCREEN:
			return onFullScreen();
		case ZQKEY_LYR_0: case ZQKEY_LYR_1: case ZQKEY_LYR_2: case ZQKEY_LYR_3:
		case ZQKEY_LYR_4: case ZQKEY_LYR_5: case ZQKEY_LYR_6:
			return onSetNewLayer(hkey-ZQKEY_LYR_0);
		case ZQKEY_SCR_LPAL_0: case ZQKEY_SCR_LPAL_1: case ZQKEY_SCR_LPAL_2: case ZQKEY_SCR_LPAL_3:
		case ZQKEY_SCR_LPAL_4: case ZQKEY_SCR_LPAL_5: case ZQKEY_SCR_LPAL_6: case ZQKEY_SCR_LPAL_7:
		case ZQKEY_SCR_LPAL_8: case ZQKEY_SCR_LPAL_9: case ZQKEY_SCR_LPAL_10: case ZQKEY_SCR_LPAL_11:
		case ZQKEY_SCR_LPAL_12: case ZQKEY_SCR_LPAL_13: case ZQKEY_SCR_LPAL_14: case ZQKEY_SCR_LPAL_15:
			return onScreenLPal(hkey-ZQKEY_SCR_LPAL_0);
		case ZQKEY_SCROLL_SCREEN_UP: case ZQKEY_SCROLL_SCREEN_DOWN:
		case ZQKEY_SCROLL_SCREEN_LEFT: case ZQKEY_SCROLL_SCREEN_RIGHT:
			return onScrollScreen(hkey-ZQKEY_SCROLL_SCREEN_UP,false);
		case ZQKEY_WARP_SCREEN_UP: case ZQKEY_WARP_SCREEN_DOWN:
		case ZQKEY_WARP_SCREEN_LEFT: case ZQKEY_WARP_SCREEN_RIGHT:
			return onScrollScreen(hkey-ZQKEY_WARP_SCREEN_UP,true);
		case ZQKEY_SCROLL_COMBO_UP:
			return onComboColUp();
		case ZQKEY_SCROLL_COMBO_DOWN:
			return onComboColDown();
		case ZQKEY_SCROLL_COMBO_LEFT:
			return onComboColLeft();
		case ZQKEY_SCROLL_COMBO_RIGHT:
			return onComboColRight();
		case ZQKEY_COMBO_PAGEUP:
			return onPgUp();
		case ZQKEY_COMBO_PAGEDN:
			return onPgDn();
	}
	return D_O_K;
}

int d_zq_hotkey_proc(int msg, DIALOG* d, int c)
{
	int ret = D_O_K;
	switch(msg)
	{
		case MSG_START:
			d->w = d->h = 0;
			break;
		case MSG_XCHAR:
			int key = c>>8;
			if(is_modkey(key) || !(key))
				break;
			int shifts = key_shifts;
			if(shifts&KB_COMMAND_FLAG)
			{
				shifts &= ~KB_COMMAND_FLAG;
				shifts |= KB_CTRL_FLAG;
			}
			if(key==KEY_F4 && (shifts&KB_ALT_FLAG))
			{
				close_button_quit = true;
				return D_USED_CHAR;
			}
			for(int pass = 0; pass <= 1; ++pass)
				for(int q = 0; q < ZQKEY_MAX; ++q)
				{
					if(zq_hotkeys[q].check(c>>8,shifts,!pass))
						return run_hotkey(q) | D_USED_CHAR;
				}
			break;
	}
	return ret;
}

