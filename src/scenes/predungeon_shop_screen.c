#include "predungeon_shop_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

enum {
	PDSM_Buy,
	PDSM_Sell,
	PDSM_Back,
	PDSM_MAX,
};

void predungeon_shop_screen_create() {
	menu = menu_init(&memory_pool, PDSM_MAX);

	const int x = 40, start_y = 60;
	menu_add_item(menu, TEXT_BUY, x, start_y, true);
	menu_add_item(menu, TEXT_SELL, x, start_y + 20, false);
	menu_add_item(menu, TEXT_GO_BACK, x, start_y + 60, true);
}

short predungeon_shop_screen_tick() {
	int option = menu_tick(menu);
	if (option >= 0) {
		switch (option) {
			case PDSM_Buy:
				return SCREEN_PRE_DUNGEON_SHOP_BUY;
			case PDSM_Sell:
			case PDSM_Back:
			default:
				return SCREEN_PRE_DUNGEON;
		}
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_PRE_DUNGEON;
	}

	return SCREEN_PRE_DUNGEON_SHOP;
}

void predungeon_shop_screen_display() {
	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	FONTCOLM(FONT_COL_WHITE);
	SHOWFONT(&glistp, TEXT_SHOP, 30, 30);

	menu_render(menu, &glistp);

	font_finish(&glistp);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}
