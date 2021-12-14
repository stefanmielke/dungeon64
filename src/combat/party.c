#include "party.h"

#include "../game.h"
#include "../util/font_renderer.h"

PartyMember get_start_member_for_class(Class class, char name[], Gender gender) {
	PartyMember member = {.class = class, .level = 1, .gender = gender};
	for (u8 i = 0; i < 9; ++i) {
		member.name[i] = name[i];
	}

	switch (class) {
		case PC_Warrior:
			member.max_health = 10;
			member.max_tp = 5;
			member.damage_range.start = 2;
			member.damage_range.start = 3;
			break;
		case PC_Wizard:
			member.max_health = 7;
			member.max_tp = 10;
			member.damage_range.start = 3;
			member.damage_range.start = 4;
			break;
		case PC_Cleric:
			member.max_health = 5;
			member.max_tp = 15;
			member.damage_range.start = 1;
			member.damage_range.start = 2;
			break;
		case PC_Thief:
			member.max_health = 8;
			member.max_tp = 7;
			member.damage_range.start = 3;
			member.damage_range.start = 4;
			break;
		default:
			break;
	}

	member.current_health = member.max_health;
	member.current_tp = member.max_tp;
	return member;
}

void party_render(Party *party, Gfx **glistp, Dynamic *dynamicp, s8 highlight_index) {
	char text[100];
	for (s8 i = 0; i < party->current_member_count; ++i) {
		const int start_x = 20 + (i * 75);
		PartyMember *member = &party->members[i];

		if (member->current_health <= 0) {
			font_renderer_set_color(glistp, FCP_GREY);
		} else if (highlight_index >= 0) {
			if (highlight_index == i) {
				font_renderer_set_color(glistp, FCP_BLUE);
			} else {
				font_renderer_set_color(glistp, FCP_GREY);
			}
		} else {
			font_renderer_set_color(glistp, FCP_BLUE);
		}

		sprintf(text, "%s", member->name);
		font_renderer_text(glistp, start_x, 190, text);
		sprintf(text, "HP %d/%d", member->current_health, member->max_health);
		font_renderer_text(glistp, start_x, 205, text);
		sprintf(text, "TP %d/%d", member->current_tp, member->max_tp);
		font_renderer_text(glistp, start_x, 215, text);
	}
}
