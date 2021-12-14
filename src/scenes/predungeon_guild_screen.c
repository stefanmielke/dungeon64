#include "predungeon_guild_screen.h"

#include "scene_defs.h"

#include "../static.h"
#include "../fonts/font_ext.h"
#include "../util/menu.h"
#include "../text/texts.h"

typedef enum PDGM_State {
	PDGMS_Talking,
	PDGMS_NotTalking,
} PDGM_State;
PDGM_State pdgm_state;

enum {
	PDGM_Talk,
	PDGM_AddMember,
	PDGM_RetireMember,
	PDGM_ItemBag,
	PDGM_Back,
	PDGM_MAX,
};

void predungeon_guild_screen_create_menu() {
	bool can_add_member = player.party.current_member_count < 4;
	bool can_retire_member = player.party.current_member_count > 0;

	const int x = 40, start_y = 60;
	menu_add_item(menu, TEXT_TALK, x, start_y, !(player.flags & FLAGS_GUILD_START));
	menu_add_item(menu, TEXT_GUILD_ADD_MEMBER, x, start_y + 20,
				  can_add_member && (player.flags & FLAGS_GUILD_START));
	menu_add_item(menu, TEXT_GUILD_RETIRE_MEMBER, x, start_y + 40,
				  can_retire_member && (player.flags & FLAGS_GUILD_START));
	menu_add_item(menu, TEXT_GUILD_ITEM_BAG, x, start_y + 60, player.flags & FLAGS_GUILD_START);
	menu_add_item(menu, TEXT_GO_BACK, x, start_y + 100, true);
}

void predungeon_guild_screen_create() {
	pdgm_state = PDGMS_NotTalking;

	menu = menu_init(&memory_pool, PDGM_MAX);
	menu_set_hand(menu, 30);

	predungeon_guild_screen_create_menu();
}

short predungeon_guild_screen_tick() {
	if (pdgm_state == PDGMS_Talking) {
		gd.pad = ReadController(A_BUTTON);

		if (IS_BUTTON_PRESSED(A_BUTTON)) {
			pdgm_state = PDGMS_NotTalking;

			if (!(player.flags & FLAGS_GUILD_START)) {
				player.flags |= FLAGS_GUILD_START;
				player.item_bag.money += 100;

				menu_reset_items(menu);
				predungeon_guild_screen_create_menu();
			}
		}
	} else {
		int option = menu_tick(menu, true);
		if (option >= 0) {
			switch (option) {
				case PDGM_Talk:
					pdgm_state = PDGMS_Talking;
					break;
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
	}

	return SCREEN_PRE_DUNGEON_GUILD;
}

void predungeon_guild_screen_display() {
	if (pdgm_state == PDGMS_NotTalking) {
		menu_render_images(menu, &glistp);
	} else {
		gSPDisplayList(glistp++, ui_setup_dl);
	}

	font_renderer_begin(&glistp);
	font_renderer_text(&glistp, 30, 30, TEXT_GUILD);

	if (pdgm_state == PDGMS_Talking) {
		font_renderer_text(&glistp, 30, 60, "THANKS FOR JOINING THE GUILD!");
		font_renderer_text(&glistp, 30, 70, "YOU CAN NOW CREATE A PARTY.");
		font_renderer_text(&glistp, 30, 80, "ALSO, TAKE 100 GOLD TO START.");
		font_renderer_text(&glistp, 30, 100, "PRESS A TO GO BACK.");
	} else {
		menu_render(menu, &glistp);

		party_render(&player.party, &glistp, rd.dynamicp, -1);
	}

	font_renderer_end(&glistp);
}
