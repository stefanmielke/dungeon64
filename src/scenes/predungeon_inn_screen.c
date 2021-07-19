#include "predungeon_inn_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

enum {
	PDIM_Back,
	PDIM_MAX,
};

void predungeon_inn_screen_create() {
	menu = menu_init(&memory_pool, PDIM_MAX);

	const int x = 40, start_y = 60;
	menu_add_item(menu, TEXT_GO_BACK, x, start_y, true);
}

short predungeon_inn_screen_tick() {
	int option = menu_tick(menu);
	if (option >= 0) {
		return SCREEN_PRE_DUNGEON;
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_PRE_DUNGEON;
	}

	return SCREEN_PRE_DUNGEON_INN;
}

void predungeon_inn_screen_display() {
	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	FONTCOLM(FONT_COL_WHITE);
	SHOWFONT(&glistp, TEXT_INN, 30, 30);

	menu_render(menu, &glistp);

	font_finish(&glistp);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}
