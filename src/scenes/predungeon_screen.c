#include "predungeon_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

typedef enum PreDungeonMenuItems {
	PDM_Inn,
	PDM_Tavern,
	PDM_Shop,
	PDM_Guild,
	PDM_MAX,
} PreDungeonMenuItems;

void predungeon_screen_create() {
	menu = menu_init(&memory_pool, PDM_MAX);

	const int x = 40, start_y = 60;
	menu_add_item(menu, 0, TEXT_INN, x, start_y);
	menu_add_item(menu, 1, TEXT_TAVERN, x, start_y + 20);
	menu_add_item(menu, 2, TEXT_SHOP, x, start_y + 40);
	menu_add_item(menu, 3, TEXT_GUILD, x, start_y + 60);
}

short predungeon_screen_tick() {
	int option = menu_tick(menu);
	if (option >= 0) {
		return SCREEN_PLAY;
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

	FONTCOLM(FONT_COL_WHITE);
	SHOWFONT(&glistp, TEXT_START, 30, 200);

	font_finish(&glistp);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}
