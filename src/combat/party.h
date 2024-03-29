#pragma once

#include <ultratypes.h>

#include "../definitions.h"
#include "../graphics.h"
#include "../types.h"

#include "../../libs/ultra64-extensions/include/range.h"

typedef enum Class {
	PC_None = 0,
	PC_Warrior,
	PC_Wizard,
	PC_Cleric,
	PC_Thief,
} Class;

typedef enum Gender {
	G_None,
	G_Male,
	G_Female,
} Gender;

typedef struct PartyMember {
	Class class;
	u8 level;
	Gender gender;
	int max_health;
	int current_health;
	int max_tp;
	int current_tp;
	RangeInt damage_range;
	char name[9];
} PartyMember;

typedef struct Party {
	PartyMember members[4];
	u8 current_member_count;
} Party;

PartyMember get_start_member_for_class(Class class, char name[], Gender gender);
void party_render(Party *party, Gfx **glistp, Dynamic *dynamicp, s8 highlight_index);
