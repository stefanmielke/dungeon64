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

	// submenu to choose a party member
	menu_init_submenus(menu, &memory_pool, 1);
	menu->submenus[0] = menu_init(&memory_pool, 4);

	for (u8 i = 0; i < player.party.current_member_count; ++i) {
		menu_add_item(menu->submenus[0], player.party.members[i].name, 40, 60 + (i * 20), true);
	}

	predungeon_guild_itembag_screen_set_menu_items();
}

short predungeon_guild_itembag_screen_tick() {
	int option = menu_tick(menu, true);

	const int back_option = player.item_bag.cur_item_bag_count + PDGITM_Back;

	if (option >= 0) {
		if (menu->active_submenu < 0) {
			if (option == back_option) {
				return SCREEN_PRE_DUNGEON_GUILD;
			} else {
				Item *item = &player.item_bag.items[option];
				switch (item->item_def->usage_type) {
					case IUT_AllMembers:
						player_use_item(&player, option);
						menu_reset_items(menu);
						predungeon_guild_itembag_screen_set_menu_items();
						break;
					case IUT_SingleMember:
						menu->active_submenu = 0;
						menu->submenus[0]->current_menu_option = 0;
						break;
					default:
						break;
				}
			}
		} else {
			player_use_item_on_party_member(&player, menu->current_menu_option, option);
			menu_reset_items(menu);
			predungeon_guild_itembag_screen_set_menu_items();
			menu->active_submenu = -1;
		}
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		if (menu->active_submenu < 0)
			return SCREEN_PRE_DUNGEON_GUILD;
		else
			menu->active_submenu = -1;
	}

	return SCREEN_PRE_DUNGEON_GUILD_ITEMBAG;
}

void predungeon_guild_itembag_screen_display() {
	menu_render_images(menu, &glistp);

	font_renderer_begin(&glistp);

	font_renderer_text(&glistp, 30, 30, TEXT_GUILD_ITEM_BAG);

	menu_render(menu, &glistp);

	party_render(&player.party, &glistp, rd.dynamicp, -1);

	font_renderer_end(&glistp);
}
