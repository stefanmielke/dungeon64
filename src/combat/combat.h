#pragma once

#include <stdbool.h>

#include "../../libs/ultra64-extensions/include/tween.h"

#include "../definitions.h"
#include "../game.h"
#include "../types.h"
#include "../util/menu.h"

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
	CAT_ITEM_USE,
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
	bool selecting_target;
	s8 selected;
	u8 current_member_choosing;
	CombatAction player_actions[4];
	float camera_x;

	u64 timer_target;
	u8 current_attacker;
	u8 current_defender;

	Tween *camera_tween;
} CombatData;

typedef struct Combat {
	Party *party;
	EnemyParty enemy_party;
	CombatState state;
	CombatData data;
	Menu *actions_menu;
} Combat;

EnemyParty get_new_enemy_party();
void combat_init(Combat *combat);
void combat_new(Combat *combat, Party *party, Tween *camera_tween);
void combat_render(Map *map, Combat *combat, Gfx **glistp, Dynamic *dynamicp, int pov_x, int pov_z);
void combat_tick(Combat *combat);
