#pragma once

#include "../definitions.h"
#include "../game.h"
#include "../types.h"

#include "party.h"
#include "enemy.h"

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
} Combat;

EnemyParty get_new_enemy_party();
Combat combat_new(Party *party);
void combat_render(Combat *combat, Gfx **glistp, Dynamic *dynamicp, int pov_x, int pov_z);
