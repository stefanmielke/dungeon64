#include "predungeon_guild_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

enum {
	PDGM_AddMember,
	PDGM_RetireMember,
	PDGM_ItemBag,
	PDGM_Back,
	PDGM_MAX,
};

void predungeon_guild_screen_create() {
	menu = menu_init(&memory_pool, PDGM_MAX);
	menu_set_hand(menu, 30);

	bool can_add_member = player.party.current_member_count < 4;
	bool can_retire_member = player.party.current_member_count > 0;

	const int x = 40, start_y = 60;
	menu_add_item(menu, TEXT_GUILD_ADD_MEMBER, x, start_y, can_add_member);
	menu_add_item(menu, TEXT_GUILD_RETIRE_MEMBER, x, start_y + 20, can_retire_member);
	menu_add_item(menu, TEXT_GUILD_ITEM_BAG, x, start_y + 40, true);
	menu_add_item(menu, TEXT_GO_BACK, x, start_y + 80, true);
}

short predungeon_guild_screen_tick() {
	int option = menu_tick(menu, true);
	if (option >= 0) {
		switch (option) {
			case PDGM_AddMember:
				return SCREEN_PRE_DUNGEON_GUILD_ADD_MEMBER;
			case PDGM_RetireMember:
				return SCREEN_PRE_DUNGEON_GUILD_RETIRE_MEMBER;
			case PDGM_ItemBag:
				return SCREEN_PRE_DUNGEON_GUILD_ITEMBAG;
			default:
				return SCREEN_PRE_DUNGEON;
		}
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		return SCREEN_PRE_DUNGEON;
	}

	return SCREEN_PRE_DUNGEON_GUILD;
}

void predungeon_guild_screen_display() {
	menu_render_images(menu, &glistp);

	font_renderer_begin(&glistp);

	font_renderer_text(&glistp, 30, 30, TEXT_GUILD);

	menu_render(menu, &glistp);

	font_renderer_end(&glistp);
}
