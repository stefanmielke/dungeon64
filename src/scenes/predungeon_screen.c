#include "predungeon_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

#include "game_screen.h"

enum {
	PDM_Inn,
	PDM_Tavern,
	PDM_Shop,
	PDM_Guild,
	PDM_Start,
	PDM_MAX,
};

void predungeon_screen_create() {
	menu = menu_init(&memory_pool, PDM_MAX);
	menu_set_hand(menu, 30);

	const bool enable_game = player.party.current_member_count > 0;
	const bool enable_play = enable_game && player_is_any_member_alive(&player);

	const int x = 40, start_y = 60;
	menu_add_item(menu, TEXT_INN, x, start_y, enable_game);
	menu_add_item(menu, TEXT_TAVERN, x, start_y + 20, enable_game);
	menu_add_item(menu, TEXT_SHOP, x, start_y + 40, enable_game);
	menu_add_item(menu, TEXT_GUILD, x, start_y + 60, true);
	menu_add_item(menu, TEXT_START, x, start_y + 100, enable_play);
}

short predungeon_screen_tick() {
	int option = menu_tick(menu, true);
	if (option >= 0) {
		switch (option) {
			case PDM_Inn:
				return SCREEN_PRE_DUNGEON_INN;
			case PDM_Tavern:
				return SCREEN_PRE_DUNGEON_TAVERN;
			case PDM_Shop:
				return SCREEN_PRE_DUNGEON_SHOP;
			case PDM_Guild:
				return SCREEN_PRE_DUNGEON_GUILD;
			case PDM_Start:
				if (player.party.current_member_count > 0 && player_is_any_member_alive(&player)) {
					forced_position_to_load = -1;
					map_to_load = 1;
					return SCREEN_PLAY;
				}
			default:
				break;
		}
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_MAIN_MENU;
	}

	return SCREEN_PRE_DUNGEON;
}

void predungeon_screen_display() {
	menu_render_images(menu, &glistp);

	font_renderer_begin(&glistp);

	font_renderer_text(&glistp, 30, 30, TEXT_PREP);

	menu_render(menu, &glistp);

	font_renderer_end(&glistp);
}
