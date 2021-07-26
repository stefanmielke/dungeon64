#include "predungeon_shop_buy_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"
#include "../items/items.h"

#include "../text/texts.h"

enum {
	PDBSM_Back,
	PDBSM_MAX,
};

void predungeon_shop_buy_screen_create() {
	menu = menu_init(&memory_pool, PDBSM_MAX + II_Max);

	const int x = 40, start_y = 60;
	for (u8 i = 0; i < II_Max; ++i) {
		menu_add_item(menu, item_defs[i].name, x, start_y + (i * 20), true);
	}

	menu_add_item(menu, TEXT_GO_BACK, x, start_y + (II_Max * 20) + 20, true);
}

short predungeon_shop_buy_screen_tick() {
	int option = menu_tick(menu, true);
	if (option >= 0) {
		switch (option) {
			case II_Max + PDBSM_Back:
				return SCREEN_PRE_DUNGEON_SHOP;
			default:
				item_bag_add_item(&player.item_bag, option);
				break;
		}
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_PRE_DUNGEON_SHOP;
	}

	return SCREEN_PRE_DUNGEON_SHOP_BUY;
}

void predungeon_shop_buy_screen_display() {
	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	FONTCOLM(FONT_COL_WHITE);
	SHOWFONT(&glistp, TEXT_BUY, 30, 30);

	char money_text[8];
	sprintf(money_text, "S%d", player.item_bag.money);
	FONTCOLM(FONT_COL_GREY);
	SHOWFONT(&glistp, money_text, 120, 30);

	menu_render(menu, &glistp);

	font_finish(&glistp);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}
