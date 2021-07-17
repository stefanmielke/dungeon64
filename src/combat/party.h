#pragma once

#include <ultratypes.h>

#include "../../libs/ultra64-extensions/include/range.h"

typedef enum Class {
	PC_None = 0,
	PC_Warrior,
	PC_Wizard,
	PC_Cleric,
	PC_Thief,
} Class;

typedef struct PartyMember {
	Class class;
	u8 level;
	int max_health;
	int current_health;
	RangeInt damage_range;
} PartyMember;

typedef struct Party {
	PartyMember members[4];
	u8 current_member_count;
} Party;

PartyMember get_start_member_for_class(Class class);
