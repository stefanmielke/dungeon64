#include "main_menu_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

typedef enum MainMenuItems {
	MMM_NewGame,
	MMM_MAX,
} MainMenuItems;

void main_menu_screen_create() {
	menu = menu_init(&memory_pool, MMM_MAX);

	const int x = 40, start_y = 140;
	menu_add_item(menu, 0, TEXT_NEW_GAME, x, start_y);
}

short main_menu_screen_tick() {
	int option = menu_tick(menu);
	if (option >= 0) {
		return SCREEN_PRE_DUNGEON;
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_MAIN;
	}

	return SCREEN_MAIN_MENU;
}

void main_menu_screen_display() {
	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	FONTCOLM(FONT_COL);
	SHOWFONT(&glistp, "Dungeon 64", 120, 100);

	menu_render(menu, &glistp);

	font_finish(&glistp);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}
