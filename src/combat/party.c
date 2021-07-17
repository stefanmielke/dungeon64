#include "party.h"

PartyMember get_start_member_for_class(Class class) {
	PartyMember member = {.class = class, .level = 1};
	switch (class) {
		case PC_Warrior:
			member.max_health = 10;
			member.damage_range.start = 2;
			member.damage_range.start = 3;
			break;
		case PC_Wizard:
			member.max_health = 7;
			member.damage_range.start = 3;
			member.damage_range.start = 4;
			break;
		case PC_Cleric:
			member.max_health = 5;
			member.damage_range.start = 1;
			member.damage_range.start = 2;
			break;
		case PC_Thief:
			member.max_health = 8;
			member.damage_range.start = 3;
			member.damage_range.start = 4;
			break;
		default:
			break;
	}

	member.current_health = member.max_health;
	return member;
}
