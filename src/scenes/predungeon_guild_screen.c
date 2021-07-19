#include "predungeon_guild_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

enum {
	PDGM_Back,
	PDGM_MAX,
};

void predungeon_guild_screen_create() {
	menu = menu_init(&memory_pool, PDGM_MAX);

	const int x = 40, start_y = 60;
	menu_add_item(menu, TEXT_GO_BACK, x, start_y);
}

short predungeon_guild_screen_tick() {
	int option = menu_tick(menu);
	if (option >= 0) {
		return SCREEN_PRE_DUNGEON;
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_PRE_DUNGEON;
	}

	return SCREEN_PRE_DUNGEON_GUILD;
}

void predungeon_guild_screen_display() {
	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	FONTCOLM(FONT_COL_WHITE);
	SHOWFONT(&glistp, TEXT_GUILD, 30, 30);

	menu_render(menu, &glistp);

	font_finish(&glistp);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}
