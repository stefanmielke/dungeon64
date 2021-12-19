#include "main_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../static.h"
#include "../text/texts.h"
#include "../util/font_renderer.h"

void main_screen_create() {
}

short main_screen_tick() {
	gd.pad = ReadController(START_BUTTON | A_BUTTON);

	if (IS_BUTTON_PRESSED(START_BUTTON) || IS_BUTTON_PRESSED(A_BUTTON)) {
		return SCREEN_MAIN_MENU;
	}

	return SCREEN_MAIN;
}

void main_screen_display() {
	gSPDisplayList(glistp++, ui_setup_dl);

	font_renderer_begin(&glistp);

	font_renderer_text(&glistp, 116, 200, TEXT_MAIN_START);

	font_renderer_set_color(&glistp, FCP_BLUE);
	font_renderer_text(&glistp, 120, 100, "DUNGEON 64");

	font_renderer_end(&glistp);
}
