#include "predungeon_shop_sell_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"
#include "../items/items.h"

#include "../text/texts.h"

enum {
	PDSSM_Back,
	PDSSM_MAX,
};

char **shop_sell_item_descs;

void predungeon_shop_sell_screen_set_menu_items() {
	const int x = 40, start_y = 60;
	for (u8 i = 0; i < player.item_bag.cur_item_bag_count; ++i) {
		item_def_get_name_and_price(player.item_bag.items[i].item_def, shop_sell_item_descs[i]);

		menu_add_item(menu, shop_sell_item_descs[i], x, start_y + (i * 20), true);
	}

	menu_add_item(menu, TEXT_GO_BACK, x, start_y + (player.item_bag.cur_item_bag_count * 20) + 20,
				  true);
}

void predungeon_shop_sell_screen_create() {
	menu = menu_init(&memory_pool, PDSSM_MAX + player.item_bag.cur_item_bag_count);
	menu_set_hand(menu, 30);

	shop_sell_item_descs = mem_zone_alloc(&memory_pool,
										  sizeof(char *) * player.item_bag.cur_item_bag_count);
	for (u8 i = 0; i < player.item_bag.cur_item_bag_count; ++i) {
		shop_sell_item_descs[i] = mem_zone_alloc(&memory_pool, sizeof(char) * ITEM_NAME_LENGTH);
	}

	predungeon_shop_sell_screen_set_menu_items();
}

short predungeon_shop_sell_screen_tick() {
	int option = menu_tick(menu, true);

	const int back_option = player.item_bag.cur_item_bag_count + PDSSM_Back;

	if (option >= 0) {
		if (option == back_option)
			return SCREEN_PRE_DUNGEON_SHOP;

		item_bag_add_money(&player.item_bag, item_defs[option].buy_value);
		item_bag_remove_item_by_index(&player.item_bag, option);
		menu_reset_items(menu);
		predungeon_shop_sell_screen_set_menu_items();
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_PRE_DUNGEON_SHOP;
	}

	return SCREEN_PRE_DUNGEON_SHOP_SELL;
}

void predungeon_shop_sell_screen_display() {
	menu_render_images(menu, &glistp);

	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	FONTCOLM(FONT_COL_WHITE);
	SHOWFONT(&glistp, TEXT_SELL, 30, 30);

	char money_text[8];
	sprintf(money_text, "S%d", player.item_bag.money);
	FONTCOLM(FONT_COL_GREY);
	SHOWFONT(&glistp, money_text, 120, 30);

	menu_render(menu, &glistp);

	font_finish(&glistp);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}
