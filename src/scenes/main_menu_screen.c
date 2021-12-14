#include "main_menu_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

enum {
	MMM_NewGame,
	MMM_MAX,
};

void main_menu_screen_create() {
	menu = menu_init(&memory_pool, MMM_MAX);
	menu_set_hand(menu, 130);

	const int x = 140, start_y = 140;
	menu_add_item(menu, TEXT_NEW_GAME, x, start_y, true);
}

short main_menu_screen_tick() {
	int option = menu_tick(menu, true);
	if (option >= 0) {
		return SCREEN_PRE_DUNGEON;
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_MAIN;
	}

	return SCREEN_MAIN_MENU;
}

void main_menu_screen_display() {
	menu_render_images(menu, &glistp);

	font_renderer_begin(&glistp);

	font_renderer_set_color(&glistp, FCP_BLUE);
	font_renderer_text(&glistp, 120, 100, "DUNGEON 64");

	menu_render(menu, &glistp);

	font_renderer_end(&glistp);
}
