#include "predungeon_tavern_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

enum {
	PDTM_Back,
	PDTM_MAX,
};

void predungeon_tavern_screen_create() {
	menu = menu_init(&memory_pool, PDTM_MAX);
	menu_set_hand(menu, 30);

	const int x = 40, start_y = 60;
	menu_add_item(menu, TEXT_GO_BACK, x, start_y, true);
}

short predungeon_tavern_screen_tick() {
	int option = menu_tick(menu, true);
	if (option >= 0) {
		return SCREEN_PRE_DUNGEON;
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_PRE_DUNGEON;
	}

	return SCREEN_PRE_DUNGEON_TAVERN;
}

void predungeon_tavern_screen_display() {
	menu_render_images(menu, &glistp);

	font_renderer_begin(&glistp);

	font_renderer_text(&glistp, 30, 30, TEXT_TAVERN);

	menu_render(menu, &glistp);

	font_renderer_end(&glistp);
}
