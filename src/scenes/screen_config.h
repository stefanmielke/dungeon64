#pragma once

#include "../util/menu.h"

typedef enum {
	SCREEN_NONE = -1,
	SCREEN_NOSAVE,
	SCREEN_MAIN,
	SCREEN_MAIN_MENU,
	SCREEN_PRE_DUNGEON,
	SCREEN_PRE_DUNGEON_INN,
	SCREEN_PLAY
} ScreenType;

void change_screen(short curr_screen, short next_screen);

extern Menu *menu;
