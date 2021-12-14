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
	menu_set_hand(menu, 30);

	const int x = 40, start_y = 60;
	menu_add_item(menu, TEXT_BUY, x, start_y, true);
	menu_add_item(menu, TEXT_SELL, x, start_y + 20, true);
	menu_add_item(menu, TEXT_GO_BACK, x, start_y + 60, true);
}

short predungeon_shop_screen_tick() {
	int option = menu_tick(menu, true);
	if (option >= 0) {
		switch (option) {
			case PDSM_Buy:
				return SCREEN_PRE_DUNGEON_SHOP_BUY;
			case PDSM_Sell:
				return SCREEN_PRE_DUNGEON_SHOP_SELL;
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
	menu_render_images(menu, &glistp);

	font_renderer_begin(&glistp);

	font_renderer_set_color(&glistp, FCP_WHITE);
	font_renderer_text(&glistp, 30, 30, TEXT_SHOP);

	char money_text[8];
	sprintf(money_text, "S%d", player.item_bag.money);
	font_renderer_set_color(&glistp, FCP_GREY);
	font_renderer_text(&glistp, 120, 30, money_text);

	menu_render(menu, &glistp);

	font_renderer_end(&glistp);
}
