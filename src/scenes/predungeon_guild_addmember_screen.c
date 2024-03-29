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
	menu_set_hand(menu, 30);

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

	menu_init_submenus(menu, &memory_pool, 3);
	menu->submenus[0] = menu_init(&memory_pool, 4);	  // classes
	menu->submenus[1] = menu_init(&memory_pool, 2);	  // genders
	menu->submenus[2] = menu_init(&memory_pool, 30);  // letters

	Menu *class_submenu = menu->submenus[0];
	Menu *gender_submenu = menu->submenus[1];
	Menu *name_submenu = menu->submenus[2];
	// Class Sub-Menu
	menu_set_hand(class_submenu, x - 10);
	menu_add_item(class_submenu, TEXT_WARRIOR, x, start_y, true);
	menu_add_item(class_submenu, TEXT_WIZARD, x, start_y + 20, true);
	menu_add_item(class_submenu, TEXT_CLERIC, x, start_y + 40, true);
	menu_add_item(class_submenu, TEXT_THIEF, x, start_y + 60, true);
	// Gender Sub-Menu
	menu_set_hand(gender_submenu, x - 10);
	menu_add_item(gender_submenu, TEXT_MALE, x, start_y, true);
	menu_add_item(gender_submenu, TEXT_FEMALE, x, start_y + 20, true);
	// Name Sub-Menu
	menu_set_horizontal(name_submenu, 5);
	menu_add_item(name_submenu, "A", x + 00, start_y + 0, true);
	menu_add_item(name_submenu, "B", x + 20, start_y + 0, true);
	menu_add_item(name_submenu, "C", x + 40, start_y + 0, true);
	menu_add_item(name_submenu, "D", x + 60, start_y + 0, true);
	menu_add_item(name_submenu, "E", x + 80, start_y + 0, true);

	menu_add_item(name_submenu, "F", x + 00, start_y + 20, true);
	menu_add_item(name_submenu, "G", x + 20, start_y + 20, true);
	menu_add_item(name_submenu, "H", x + 40, start_y + 20, true);
	menu_add_item(name_submenu, "I", x + 60, start_y + 20, true);
	menu_add_item(name_submenu, "J", x + 80, start_y + 20, true);

	menu_add_item(name_submenu, "K", x + 00, start_y + 40, true);
	menu_add_item(name_submenu, "L", x + 20, start_y + 40, true);
	menu_add_item(name_submenu, "M", x + 40, start_y + 40, true);
	menu_add_item(name_submenu, "N", x + 60, start_y + 40, true);
	menu_add_item(name_submenu, "O", x + 80, start_y + 40, true);

	menu_add_item(name_submenu, "P", x + 00, start_y + 60, true);
	menu_add_item(name_submenu, "Q", x + 20, start_y + 60, true);
	menu_add_item(name_submenu, "R", x + 40, start_y + 60, true);
	menu_add_item(name_submenu, "S", x + 60, start_y + 60, true);
	menu_add_item(name_submenu, "T", x + 80, start_y + 60, true);

	menu_add_item(name_submenu, "U", x + 00, start_y + 80, true);
	menu_add_item(name_submenu, "V", x + 20, start_y + 80, true);
	menu_add_item(name_submenu, "W", x + 40, start_y + 80, true);
	menu_add_item(name_submenu, "X", x + 60, start_y + 80, true);
	menu_add_item(name_submenu, "Y", x + 80, start_y + 80, true);

	menu_add_item(name_submenu, "Z", x + 00, start_y + 100, true);
	menu_add_item(name_submenu, "!", x + 20, start_y + 100, true);
	menu_add_item(name_submenu, "?", x + 40, start_y + 100, true);
	menu_add_item(name_submenu, "BSP", x + 60, start_y + 100, true);
	menu_add_item(name_submenu, "DONE", x + 90, start_y + 100, true);
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
			menu->active_submenu = -1;
	}

	return SCREEN_PRE_DUNGEON_GUILD_ADD_MEMBER;
}

bool should_enable_done() {
	return guild_add_member_data->class != PC_None && guild_add_member_data->gender != G_None &&
		   guild_add_member_data->cur_char > 0;
}

void predungeon_guild_addmember_screen_display() {
	menu_render_images(menu, &glistp);

	font_renderer_begin(&glistp);

	font_renderer_text(&glistp, 30, 30, TEXT_GUILD_ADD_MEMBER);

	menu_render(menu, &glistp);

	const int text_x = 200, name_y = 60;
	if (guild_add_member_data->cur_char == 0) {
		font_renderer_set_color(&glistp, FCP_GREY);
		font_renderer_text(&glistp, text_x, name_y, TEXT_NO_NAME);
	} else {
		font_renderer_set_color(&glistp, FCP_GREEN);
		font_renderer_text(&glistp, text_x, name_y, guild_add_member_data->name);
	}

	const int class_y = 80;
	if (guild_add_member_data->class == PC_None) {
		font_renderer_set_color(&glistp, FCP_GREY);
	} else {
		font_renderer_set_color(&glistp, FCP_GREEN);
	}
	switch (guild_add_member_data->class) {
		case PC_Warrior:
			font_renderer_text(&glistp, text_x, class_y, TEXT_WARRIOR);
			break;
		case PC_Wizard:
			font_renderer_text(&glistp, text_x, class_y, TEXT_WIZARD);
			break;
		case PC_Cleric:
			font_renderer_text(&glistp, text_x, class_y, TEXT_CLERIC);
			break;
		case PC_Thief:
			font_renderer_text(&glistp, text_x, class_y, TEXT_THIEF);
			break;
		default:
			font_renderer_text(&glistp, text_x, class_y, TEXT_NO_CLASS);
			break;
	}
	const int gender_y = 100;
	if (guild_add_member_data->gender == G_None) {
		font_renderer_set_color(&glistp, FCP_GREY);
	} else {
		font_renderer_set_color(&glistp, FCP_GREEN);
	}
	switch (guild_add_member_data->gender) {
		case G_Male:
			font_renderer_text(&glistp, text_x, gender_y, TEXT_MALE);
			break;
		case G_Female:
			font_renderer_text(&glistp, text_x, gender_y, TEXT_FEMALE);
			break;
		default:
			font_renderer_text(&glistp, text_x, gender_y, TEXT_NO_GENDER);
			break;
	}

	party_render(&player.party, &glistp, rd.dynamicp, -1);

	font_renderer_end(&glistp);
}
