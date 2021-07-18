#pragma once

#include <stdbool.h>

#include "../definitions.h"
#include "../game.h"
#include "../types.h"

#include "party.h"
#include "enemy.h"

typedef enum CombatState {
	CS_START,
	CS_PLAYER_PHASE,
	CS_RUN_COMBAT,
	CS_ENDING,
	CS_END,
} CombatState;

typedef enum CombatActionType {
	CAT_DEFEND,
	CAT_ATK_PHYS,
	CAT_ATK_SKILL,
	CAT_HEAL,
} CombatActionType;

typedef struct EnemyCombat {
	Enemy *enemy;
	int current_health;
} EnemyCombat;

typedef struct EnemyParty {
	EnemyCombat enemies[4];
	u8 current_enemy_count;
} EnemyParty;

typedef struct CombatAction {
	bool target_is_enemy;
	CombatActionType type;
	u8 target;
	u8 type_arg_1;
} CombatAction;

typedef struct CombatData {
	s8 selected;
	u8 current_member_choosing;
	CombatAction player_actions[4];
	float camera_x;

	u64 timer_target;
	u8 current_attacker;
} CombatData;

typedef struct Combat {
	Party *party;
	EnemyParty enemy_party;
	CombatState state;
	CombatData data;
} Combat;

EnemyParty get_new_enemy_party();
Combat combat_new(Party *party);
void combat_render(Combat *combat, Gfx **glistp, Dynamic *dynamicp, int pov_x, int pov_z);
void combat_tick(Combat *combat);
