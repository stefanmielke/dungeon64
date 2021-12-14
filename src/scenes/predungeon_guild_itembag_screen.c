#include "predungeon_guild_itembag_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"
#include "../items/items.h"

#include "../text/texts.h"

enum {
	PDGITM_Back,
	PDGITM_MAX,
};

void predungeon_guild_itembag_screen_set_menu_items() {
	const int x = 40, start_y = 60;
	for (u8 i = 0; i < player.item_bag.cur_item_bag_count; ++i) {
		menu_add_item(menu, player.item_bag.items[i].item_def->name, x, start_y + (i * 20), true);
	}

	menu_add_item(menu, TEXT_GO_BACK, x, start_y + (player.item_bag.cur_item_bag_count * 20) + 20,
				  true);
}

void predungeon_guild_itembag_screen_create() {
	menu = menu_init(&memory_pool, PDGITM_MAX + player.item_bag.cur_item_bag_count);

	predungeon_guild_itembag_screen_set_menu_items();
}

short predungeon_guild_itembag_screen_tick() {
	int option = menu_tick(menu, true);

	const int back_option = player.item_bag.cur_item_bag_count + PDGITM_Back;

	if (option >= 0) {
		if (option == back_option)
			return SCREEN_PRE_DUNGEON_GUILD;
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_PRE_DUNGEON_GUILD;
	}

	return SCREEN_PRE_DUNGEON_GUILD_ITEMBAG;
}

void predungeon_guild_itembag_screen_display() {
	menu_render_images(menu, &glistp);

	font_renderer_begin(&glistp);

	font_renderer_text(&glistp, 30, 30, TEXT_GUILD_ITEM_BAG);

	menu_render(menu, &glistp);

	font_renderer_end(&glistp);
}
