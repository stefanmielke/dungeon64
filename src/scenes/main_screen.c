#include "main_screen.h"

#include <ultra64.h>

#include "scene_defs.h"
#include "screen_config.h"

#include "../fonts/font_ext.h"
#include "../game.h"

void main_screen_create() {
}

short main_screen_tick() {
	if (IS_BUTTON_PRESSED(START_BUTTON)) {
		return SCREEN_PLAY;
	}

	return SCREEN_MAIN;
}

void main_screen_display() {
	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	FONTCOLM(FONT_COL);
	SHOWFONT(&glistp, "Dungeon 64", 120, 100);

	FONTCOLM(FONT_COL_WHITE);
	SHOWFONT(&glistp, "Press START", 116, 200);

	font_finish(&glistp);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}
