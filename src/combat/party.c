#include "party.h"

PartyMember get_start_member_for_class(Class class, const char *name) {
	PartyMember member = {.class = class, .level = 1, .name = name};
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
	FONTCOLM(FONT_COL);

	char text[100];
	for (s8 i = 0; i < party->current_member_count; ++i) {
		const int start_x = 20 + (i * 80);
		PartyMember *member = &party->members[i];

		if (highlight_index >= 0 && member->current_health > 0) {
			if (highlight_index == i) {
				FONTCOLM(FONT_COL);
			} else {
				FONTCOLM(FONT_COL_GREY);
			}
		} else if (member->current_health <= 0) {
			FONTCOLM(FONT_COL_GREY);
		}

		sprintf(text, "%s", member->name);
		SHOWFONT(glistp, text, start_x, 180);
		sprintf(text, "HP %d/%d", member->current_health, member->max_health);
		SHOWFONT(glistp, text, start_x, 195);
		sprintf(text, "TP %d/%d", member->current_tp, member->max_tp);
		SHOWFONT(glistp, text, start_x, 210);
	}
}
