#include "predungeon_inn_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

enum {
	PDIM_Stay,
	PDIM_Back,
	PDIM_MAX,
};

void predungeon_inn_screen_create() {
	menu = menu_init(&memory_pool, PDIM_MAX);
	menu_set_hand(menu, 30);

	const int x = 40, start_y = 60;
	menu_add_item(menu, TEXT_GO_STAY, x, start_y, true);
	menu_add_item(menu, TEXT_GO_BACK, x, start_y + 40, true);
}

short predungeon_inn_screen_tick() {
	int option = menu_tick(menu, true);
	if (option >= 0) {
		switch (option) {
			case PDIM_Stay:
				player_heal_all_party_members(&player);
				break;
			case PDIM_Back:
			default:
				return SCREEN_PRE_DUNGEON;
		}
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_PRE_DUNGEON;
	}

	return SCREEN_PRE_DUNGEON_INN;
}

void predungeon_inn_screen_display() {
	menu_render_images(menu, &glistp);

	font_renderer_begin(&glistp);

	font_renderer_text(&glistp, 30, 30, TEXT_INN);

	menu_render(menu, &glistp);

	party_render(&player.party, &glistp, rd.dynamicp, -1);

	font_renderer_end(&glistp);
}
