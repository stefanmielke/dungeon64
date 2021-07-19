#include "predungeon_guild_addmember_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

enum {
	PDGAMM_Class,
	PDGAMM_Gender,
	PDGAMM_Done,
	PDGAMM_Back,
	PDGAMM_MAX,
};

typedef struct GuildAddMemberData {
	Class class;
	Gender gender;
} GuildAddMemberData;
GuildAddMemberData *guild_add_member_data;

bool should_enable_done();

void predungeon_guild_addmember_screen_create() {
	menu = menu_init(&memory_pool, PDGAMM_MAX);
	guild_add_member_data = mem_zone_alloc(&memory_pool, sizeof(GuildAddMemberData));
	guild_add_member_data->class = PC_None;
	guild_add_member_data->gender = G_None;

	const int x = 40, start_y = 60;
	menu_add_item(menu, TEXT_GUILD_ADD_MEMBER_CLASS, x, start_y, true);
	menu_add_item(menu, TEXT_GUILD_ADD_MEMBER_GENDER, x, start_y + 20, true);
	menu_add_item(menu, TEXT_GUILD_ADD_MEMBER_DONE, x, start_y + 40, false);
	menu_add_item(menu, TEXT_GO_BACK, x, start_y + 80, true);

	menu_init_submenus(menu, &memory_pool, 2, 4);
	// Class Sub-Menu
	menu_submenu_add_item(menu, 0, TEXT_WARRIOR, x, start_y, true);
	menu_submenu_add_item(menu, 0, TEXT_WIZARD, x, start_y + 20, true);
	menu_submenu_add_item(menu, 0, TEXT_CLERIC, x, start_y + 40, true);
	menu_submenu_add_item(menu, 0, TEXT_THIEF, x, start_y + 60, true);
	// Gender Sub-Menu
	menu_submenu_add_item(menu, 1, TEXT_MALE, x, start_y, true);
	menu_submenu_add_item(menu, 1, TEXT_FEMALE, x, start_y + 20, true);
}

short predungeon_guild_addmember_screen_tick() {
	int option = menu_tick(menu);
	if (option >= 0) {
		if (menu->active_submenu == -1) {  // Setup menu
			switch (option) {
				case PDGAMM_Class:
					menu->active_submenu = 0;
					break;
				case PDGAMM_Gender:
					menu->active_submenu = 1;
					break;
				case PDGAMM_Done:
					player_party_add_member(&player, guild_add_member_data->class, "12345678",
											guild_add_member_data->gender);
					return SCREEN_PRE_DUNGEON_GUILD;
				case PDGAMM_Back:
				default:
					return SCREEN_PRE_DUNGEON_GUILD;
			}
		} else if (menu->active_submenu == 0) {	 // Class menu
			guild_add_member_data->class = option + 1;
			menu->active_submenu = -1;
			menu->items[PDGAMM_Done].enabled = should_enable_done();

		} else if (menu->active_submenu == 1) {	 // Gender menu
			guild_add_member_data->gender = option + 1;
			menu->active_submenu = -1;
			menu->items[PDGAMM_Done].enabled = should_enable_done();
		}
	} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
		if (menu->active_submenu == -1)
			return SCREEN_PRE_DUNGEON_GUILD;
		else
			menu->active_submenu == -1;
	}

	return SCREEN_PRE_DUNGEON_GUILD_ADD_MEMBER;
}

bool should_enable_done() {
	return guild_add_member_data->class != PC_None && guild_add_member_data->gender != G_None;
}

void predungeon_guild_addmember_screen_display() {
	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	FONTCOLM(FONT_COL_WHITE);
	SHOWFONT(&glistp, TEXT_GUILD_ADD_MEMBER, 30, 30);

	menu_render(menu, &glistp);

	const int text_x = 200, class_y = 60;
	if (guild_add_member_data->class == PC_None) {
		FONTCOLM(FONT_COL_GREY);
	} else {
		FONTCOLM(FONT_COL_GREEN);
	}
	switch (guild_add_member_data->class) {
		case PC_Warrior:
			SHOWFONT(&glistp, TEXT_WARRIOR, text_x, class_y);
			break;
		case PC_Wizard:
			SHOWFONT(&glistp, TEXT_WIZARD, text_x, class_y);
			break;
		case PC_Cleric:
			SHOWFONT(&glistp, TEXT_CLERIC, text_x, class_y);
			break;
		case PC_Thief:
			SHOWFONT(&glistp, TEXT_THIEF, text_x, class_y);
			break;
		default:
			SHOWFONT(&glistp, "No Class", text_x, class_y);
			break;
	}
	const int gender_y = 80;
	if (guild_add_member_data->gender == G_None) {
		FONTCOLM(FONT_COL_GREY);
	} else {
		FONTCOLM(FONT_COL_GREEN);
	}
	switch (guild_add_member_data->gender) {
		case G_Male:
			SHOWFONT(&glistp, TEXT_MALE, text_x, gender_y);
			break;
		case G_Female:
			SHOWFONT(&glistp, TEXT_FEMALE, text_x, gender_y);
			break;
		default:
			SHOWFONT(&glistp, "No Gender", text_x, gender_y);
			break;
	}

	font_finish(&glistp);

	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}
