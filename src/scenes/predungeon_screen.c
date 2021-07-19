#include "predungeon_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

enum {
	PDM_Inn,
	PDM_Tavern,
	PDM_Shop,
	PDM_Guild,
	PDM_Start,
	PDM_MAX,
};

void predungeon_screen_create() {
	menu = menu_init(&memory_pool, PDM_MAX);

	const int x = 40, start_y = 60;
	menu_add_item(menu, TEXT_INN, x, start_y);
	menu_add_item(menu, TEXT_TAVERN, x, start_y + 20);
	menu_add_item(menu, TEXT_SHOP, x, start_y + 40);
	menu_add_item(menu, TEXT_GUILD, x, start_y + 60);
	menu_add_item(menu, TEXT_START, x, start_y + 100);
}

short predungeon_screen_tick() {
	int option = menu_tick(menu);
	if (option >= 0) {
		switch (option) {
			case PDM_Inn:
				return SCREEN_PRE_DUNGEON_INN;
			case PDM_Tavern:
				return SCREEN_PRE_DUNGEON_TAVERN;
			case PDM_Shop:
				return SCREEN_PRE_DUNGEON_SHOP;
			case PDM_Guild:
				return SCREEN_PRE_DUNGEON_GUILD;
			case PDM_Start:
				return SCREEN_PLAY;
			default:
				break;
		}
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_MAIN_MENU;
	}

	return SCREEN_PRE_DUNGEON;
}

void predungeon_screen_display() {
	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	FONTCOLM(FONT_COL_WHITE);
	SHOWFONT(&glistp, TEXT_PREP, 30, 30);

	menu_render(menu, &glistp);

	font_finish(&glistp);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}
