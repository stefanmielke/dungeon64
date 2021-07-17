#pragma once

#include "../definitions.h"
#include "../game.h"
#include "../types.h"

#include "party.h"
#include "enemy.h"

typedef enum CombatState {
	CS_START,
	CS_PLAYER_PHASE,
	CS_RUN_COMBAT,
	CS_END,
} CombatState;

typedef struct EnemyCombat {
	Enemy *enemy;
	int current_health;
} EnemyCombat;

typedef struct EnemyParty {
	EnemyCombat enemies[4];
	u8 current_enemy_count;
} EnemyParty;

typedef struct Combat {
	Party *party;
	EnemyParty enemy_party;
	CombatState state;
} Combat;

EnemyParty get_new_enemy_party();
Combat combat_new(Party *party);
void combat_render(Combat *combat, Gfx **glistp, Dynamic *dynamicp, int pov_x, int pov_z);
void combat_tick(Combat *combat);
