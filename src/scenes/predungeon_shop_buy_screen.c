#include "predungeon_shop_buy_screen.h"

#include <nustd/string.h>

#include "scene_defs.h"
#include "../fonts/font_ext.h"
#include "../util/menu.h"
#include "../items/items.h"

#include "../text/texts.h"

enum {
	PDBSM_Back,
	PDBSM_MAX,
};

char **item_descs;
#define ITEM_NAME_LENGTH 30

void predungeon_shop_buy_screen_create() {
	menu = menu_init(&memory_pool, PDBSM_MAX + II_Max);
	item_descs = mem_zone_alloc(&memory_pool, sizeof(char *) * II_Max);
	for (u8 i = 0; i < II_Max; ++i) {
		item_descs[i] = mem_zone_alloc(&memory_pool, sizeof(char) * ITEM_NAME_LENGTH);
	}

	char value_text[6];
	const int x = 40, start_y = 60;
	for (u8 i = 0; i < II_Max; ++i) {
		int name_length = strlen(item_defs[i].name);

		{
			u8 c = 0;
			for (; c < name_length; ++c)
				item_descs[i][c] = item_defs[i].name[c];
			for (; c < ITEM_NAME_LENGTH - 1; ++c)
				item_descs[i][c] = ' ';
			item_descs[i][c] = '\0';
		}

		sprintf(value_text, "%d", item_defs[i].buy_value);
		int value_length = strlen(value_text);
		int value_start = ITEM_NAME_LENGTH - value_length - 1;
		for (u8 c = 0; c < value_length; ++c) {
			item_descs[i][value_start + c] = value_text[c];
		}

		menu_add_item(menu, item_descs[i], x, start_y + (i * 20),
					  item_defs[i].buy_value <= player.item_bag.money);
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
