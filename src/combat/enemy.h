#pragma once

#include <ultratypes.h>
#include "../../libs/ultra64-extensions/include/range.h"

typedef enum EnemyType {
	ET_None = 0,
	ET_GoblinBoss,
	ET_GoblinMinion1,
	ET_GoblinMinion2,
	ET_MAX,
} EnemyType;

typedef struct Enemy {
	EnemyType type;
	const char *name;
	int health;
	RangeInt damage_range;
} Enemy;

extern Enemy enemy_defaults[];

Enemy *get_enemy_data_for_type(EnemyType type);
