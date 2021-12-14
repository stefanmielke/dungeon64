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

char **shop_buy_item_descs;

void predungeon_shop_buy_screen_rebuild_menu() {
	const int x = 40, start_y = 60;
	for (u8 i = 0; i < II_Max; ++i) {
		item_def_get_name_and_price(&item_defs[i], shop_buy_item_descs[i]);

		menu_add_item(menu, shop_buy_item_descs[i], x, start_y + (i * 20),
					  item_defs[i].buy_value <= player.item_bag.money);
	}

	menu_add_item(menu, TEXT_GO_BACK, x, start_y + (II_Max * 20) + 20, true);
}

void predungeon_shop_buy_screen_create() {
	menu = menu_init(&memory_pool, PDBSM_MAX + II_Max);
	menu_set_hand(menu, 30);

	shop_buy_item_descs = mem_zone_alloc(&memory_pool, sizeof(char *) * II_Max);
	for (u8 i = 0; i < II_Max; ++i) {
		shop_buy_item_descs[i] = mem_zone_alloc(&memory_pool, sizeof(char) * ITEM_NAME_LENGTH);
	}

	predungeon_shop_buy_screen_rebuild_menu();
}

short predungeon_shop_buy_screen_tick() {
	int option = menu_tick(menu, true);
	if (option >= 0) {
		switch (option) {
			case II_Max + PDBSM_Back:
				return SCREEN_PRE_DUNGEON_SHOP;
			default:
				if (item_bag_spend_money(&player.item_bag, item_defs[option].buy_value)) {
					item_bag_add_item(&player.item_bag, option);
					menu_reset_items(menu);
					predungeon_shop_buy_screen_rebuild_menu();
				}
				break;
		}
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_PRE_DUNGEON_SHOP;
	}

	return SCREEN_PRE_DUNGEON_SHOP_BUY;
}

void predungeon_shop_buy_screen_display() {
	menu_render_images(menu, &glistp);

	font_renderer_begin(&glistp);

	font_renderer_text(&glistp, 30, 30, TEXT_BUY);

	char money_text[8];
	sprintf(money_text, "S%d", player.item_bag.money);
	font_renderer_set_color(&glistp, FCP_GREY);
	font_renderer_text(&glistp, 120, 30, money_text);

	menu_render(menu, &glistp);

	font_renderer_end(&glistp);
}
