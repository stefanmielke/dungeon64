#include "predungeon_guild_addmember_screen.h"

#include "scene_defs.h"

#include "../fonts/font_ext.h"
#include "../util/menu.h"

#include "../text/texts.h"

enum {
	PDGAMM_Name,
	PDGAMM_Class,
	PDGAMM_Gender,
	PDGAMM_Done,
	PDGAMM_Back,
	PDGAMM_MAX,
};

typedef struct GuildAddMemberData {
	char name[9];
	u8 cur_char;
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
	guild_add_member_data->cur_char = 0;
	guild_add_member_data->name[0] = '_';
	for (u8 i = 1; i < 9; ++i)
		guild_add_member_data->name[i] = '\0';

	const int x = 40, start_y = 60;
	menu_add_item(menu, TEXT_GUILD_ADD_MEMBER_NAME, x, start_y, true);
	menu_add_item(menu, TEXT_GUILD_ADD_MEMBER_CLASS, x, start_y + 20, true);
	menu_add_item(menu, TEXT_GUILD_ADD_MEMBER_GENDER, x, start_y + 40, true);
	menu_add_item(menu, TEXT_GUILD_ADD_MEMBER_DONE, x, start_y + 60, false);
	menu_add_item(menu, TEXT_GO_BACK, x, start_y + 80, true);

	menu_init_submenus(menu, &memory_pool, 3, 30);
	// Class Sub-Menu
	menu_submenu_add_item(menu, 0, TEXT_WARRIOR, x, start_y, true);
	menu_submenu_add_item(menu, 0, TEXT_WIZARD, x, start_y + 20, true);
	menu_submenu_add_item(menu, 0, TEXT_CLERIC, x, start_y + 40, true);
	menu_submenu_add_item(menu, 0, TEXT_THIEF, x, start_y + 60, true);
	// Gender Sub-Menu
	menu_submenu_add_item(menu, 1, TEXT_MALE, x, start_y, true);
	menu_submenu_add_item(menu, 1, TEXT_FEMALE, x, start_y + 20, true);
	// Name Sub-Menu
	menu_submenu_set_horizontal(menu, 2, 5);
	menu_submenu_add_item(menu, 2, "A", x + 00, start_y + 0, true);
	menu_submenu_add_item(menu, 2, "B", x + 20, start_y + 0, true);
	menu_submenu_add_item(menu, 2, "C", x + 40, start_y + 0, true);
	menu_submenu_add_item(menu, 2, "D", x + 60, start_y + 0, true);
	menu_submenu_add_item(menu, 2, "E", x + 80, start_y + 0, true);

	menu_submenu_add_item(menu, 2, "F", x + 00, start_y + 20, true);
	menu_submenu_add_item(menu, 2, "G", x + 20, start_y + 20, true);
	menu_submenu_add_item(menu, 2, "H", x + 40, start_y + 20, true);
	menu_submenu_add_item(menu, 2, "I", x + 60, start_y + 20, true);
	menu_submenu_add_item(menu, 2, "J", x + 80, start_y + 20, true);

	menu_submenu_add_item(menu, 2, "K", x + 00, start_y + 40, true);
	menu_submenu_add_item(menu, 2, "L", x + 20, start_y + 40, true);
	menu_submenu_add_item(menu, 2, "M", x + 40, start_y + 40, true);
	menu_submenu_add_item(menu, 2, "N", x + 60, start_y + 40, true);
	menu_submenu_add_item(menu, 2, "O", x + 80, start_y + 40, true);

	menu_submenu_add_item(menu, 2, "P", x + 00, start_y + 60, true);
	menu_submenu_add_item(menu, 2, "Q", x + 20, start_y + 60, true);
	menu_submenu_add_item(menu, 2, "R", x + 40, start_y + 60, true);
	menu_submenu_add_item(menu, 2, "S", x + 60, start_y + 60, true);
	menu_submenu_add_item(menu, 2, "T", x + 80, start_y + 60, true);

	menu_submenu_add_item(menu, 2, "U", x + 00, start_y + 80, true);
	menu_submenu_add_item(menu, 2, "V", x + 20, start_y + 80, true);
	menu_submenu_add_item(menu, 2, "W", x + 40, start_y + 80, true);
	menu_submenu_add_item(menu, 2, "X", x + 60, start_y + 80, true);
	menu_submenu_add_item(menu, 2, "Y", x + 80, start_y + 80, true);

	menu_submenu_add_item(menu, 2, "Z", x + 00, start_y + 100, true);
	menu_submenu_add_item(menu, 2, "!", x + 20, start_y + 100, true);
	menu_submenu_add_item(menu, 2, "?", x + 40, start_y + 100, true);
	menu_submenu_add_item(menu, 2, "BSP", x + 60, start_y + 100, true);
	menu_submenu_add_item(menu, 2, "DONE", x + 90, start_y + 100, true);
}

void predungeon_guild_addmember_name_screen_add_letter(char letter) {
	if (guild_add_member_data->cur_char < 8) {
		guild_add_member_data->name[guild_add_member_data->cur_char] = letter;
		if (guild_add_member_data->cur_char < 7) {
			guild_add_member_data->name[guild_add_member_data->cur_char + 1] = '_';
		}
		guild_add_member_data->cur_char++;
	} else {
		guild_add_member_data->name[7] = letter;
	}
}

short predungeon_guild_addmember_screen_tick() {
	int option = menu_tick(menu, true);
	if (option >= 0) {
		if (menu->active_submenu == -1) {  // Setup menu
			switch (option) {
				case PDGAMM_Name:
					menu->active_submenu = 2;
					break;
				case PDGAMM_Class:
					menu->active_submenu = 0;
					break;
				case PDGAMM_Gender:
					menu->active_submenu = 1;
					break;
				case PDGAMM_Done:
					player_party_add_member(&player, guild_add_member_data->class,
											guild_add_member_data->name,
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
		} else if (menu->active_submenu == 2) {	 // Name menu
			if (option < 26) {
				// letter
				predungeon_guild_addmember_name_screen_add_letter(option + 65);
			} else if (option == 26) {	// !
				predungeon_guild_addmember_name_screen_add_letter('!');
			} else if (option == 27) {	// ?
				predungeon_guild_addmember_name_screen_add_letter('?');
			} else if (option == 28) {	// Backspace
				if (guild_add_member_data->cur_char > 0) {
					guild_add_member_data->name[guild_add_member_data->cur_char] = '\0';
					guild_add_member_data->cur_char--;
					guild_add_member_data->name[guild_add_member_data->cur_char] = '_';
				}
			} else if (option == 29) {	// Done
				if (guild_add_member_data->name[guild_add_member_data->cur_char] == '_')
					guild_add_member_data->name[guild_add_member_data->cur_char] = '\0';
				menu->active_submenu = -1;
				menu->items[PDGAMM_Done].enabled = should_enable_done();
			}
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
	return guild_add_member_data->class != PC_None && guild_add_member_data->gender != G_None &&
		   guild_add_member_data->cur_char > 0;
}

void predungeon_guild_addmember_screen_display() {
	font_init(&glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	FONTCOLM(FONT_COL_WHITE);
	SHOWFONT(&glistp, TEXT_GUILD_ADD_MEMBER, 30, 30);

	menu_render(menu, &glistp);

	const int text_x = 200, name_y = 60;
	if (guild_add_member_data->cur_char == 0) {
		FONTCOLM(FONT_COL_GREY);
		SHOWFONT(&glistp, "No Name", text_x, name_y);
	} else {
		FONTCOLM(FONT_COL_GREEN);
		SHOWFONT(&glistp, guild_add_member_data->name, text_x, name_y);
	}

	const int class_y = 80;
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
	const int gender_y = 100;
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
