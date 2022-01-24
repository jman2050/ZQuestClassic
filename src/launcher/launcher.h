#ifndef ZC_LAUNCHER_H
#define ZC_LAUNCHER_H

#include "zdefs.h"
#include "jwinfsel.h"
#include "zsys.h"
extern volatile bool close_button_quit;
extern int32_t zq_screen_w, zq_screen_h;
extern bool is_large;
extern char temppath[4096], rootpath[4096];

void update_hw_screen();

bool getname(const char *prompt,const char *ext,EXT_LIST *list,const char *def,bool usefilename);

#endif

