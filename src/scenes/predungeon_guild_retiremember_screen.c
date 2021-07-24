#include "predungeon_guild_retiremember_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

enum {
	PDGAMM_Back,
	PDGAMM_MAX,
};

void predungeon_guild_retire_member_update_menu_items() {
	for (u8 i = 0; i < 4; ++i) {
		if (player.party.members[i].class == PC_None) {
			menu->items[i].enabled = false;
			menu->items[i].text = TEXT_VACANT;
		} else {
			menu->items[i].enabled = true;
			menu->items[i].text = player.party.members[i].name;
		}
	}
}

void predungeon_guild_retiremember_screen_create() {
	menu = menu_init(&memory_pool, PDGAMM_MAX + 4);

	const int x = 40, start_y = 60;
	menu_add_item(menu, TEXT_VACANT, x, start_y, false);
	menu_add_item(menu, TEXT_VACANT, x, start_y + 20, true);
	menu_add_item(menu, TEXT_VACANT, x, start_y + 40, true);
	menu_add_item(menu, TEXT_VACANT, x, start_y + 60, true);
	menu_add_item(menu, TEXT_GO_BACK, x, start_y + 100, true);

	predungeon_guild_retire_member_update_menu_items();
}

short predungeon_guild_retiremember_screen_tick() {
	int option = menu_tick(menu, true);
	if (option >= 0) {
		if (option >= 4) {
			return SCREEN_PRE_DUNGEON_GUILD;
		}

		player_party_retire_member(&player, option);
		predungeon_guild_retire_member_update_menu_items();
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_PRE_DUNGEON_GUILD;
	}

	return SCREEN_PRE_DUNGEON_GUILD_RETIRE_MEMBER;
}

void predungeon_guild_retiremember_screen_display() {
	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	FONTCOLM(FONT_COL_WHITE);
	SHOWFONT(&glistp, TEXT_GUILD_RETIRE_MEMBER, 30, 30);

	menu_render(menu, &glistp);

	font_finish(&glistp);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}
