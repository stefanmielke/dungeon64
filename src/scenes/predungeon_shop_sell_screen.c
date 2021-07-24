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

void predungeon_shop_sell_screen_set_menu_items() {
	const int x = 40, start_y = 60;
	for (u8 i = 0; i < player.item_bag.cur_item_bag_count; ++i) {
		menu_add_item(menu, player.item_bag.items[i].item_def->name, x, start_y + (i * 20), true);
	}

	menu_add_item(menu, TEXT_GO_BACK, x, start_y + (player.item_bag.cur_item_bag_count * 20) + 20,
				  true);
}

void predungeon_shop_sell_screen_create() {
	menu = menu_init(&memory_pool, PDSSM_MAX + player.item_bag.cur_item_bag_count);

	predungeon_shop_sell_screen_set_menu_items();
}

short predungeon_shop_sell_screen_tick() {
	int option = menu_tick(menu, true);

	const int back_option = player.item_bag.cur_item_bag_count + PDSSM_Back;

	if (option >= 0) {
		if (option == back_option)
			return SCREEN_PRE_DUNGEON_SHOP;

		item_bag_remove_item_by_index(&player.item_bag, option);
		menu_reset_items(menu);
		predungeon_shop_sell_screen_set_menu_items();
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_PRE_DUNGEON_SHOP;
	}

	return SCREEN_PRE_DUNGEON_SHOP_SELL;
}

void predungeon_shop_sell_screen_display() {
	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	FONTCOLM(FONT_COL_WHITE);
	SHOWFONT(&glistp, TEXT_SELL, 30, 30);

	menu_render(menu, &glistp);

	font_finish(&glistp);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}
