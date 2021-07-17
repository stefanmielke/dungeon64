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

void party_render(Party *party, Gfx **glistp, Dynamic *dynamicp) {
	// render text
	font_init(glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 3);

	char text[100];
	for (u8 i = 0; i < party->current_member_count; ++i) {
		const int start_x = 20 + (i * 80);
		PartyMember *member = &party->members[i];
		FONTCOLM(FONT_COL);
		sprintf(text, "%s\nHP %d/%d\nTP %d/%d", member->name, member->current_health,
				member->max_health, member->current_tp, member->max_tp);
		SHOWFONT(glistp, text, start_x, 180);
	}

	font_finish(glistp);
}
