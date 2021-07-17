#pragma once

#include <ultratypes.h>
#include "../../libs/ultra64-extensions/include/range.h"

typedef enum {
	ET_None = 0,
	ET_BlueDragon,
	ET_MAX,
} EnemyType;

typedef struct {
	EnemyType type;
	const char *name;
	int health;
	RangeInt damage_range;
} Enemy;

extern Enemy enemy_defaults[];

Enemy *get_enemy_data_for_type(EnemyType type);
