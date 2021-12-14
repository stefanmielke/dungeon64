#include "enemy.h"

Enemy enemy_defaults[] = {
	{.type = ET_GoblinBoss, .name = "GOBLIN KING", .health = 20, .damage_range = {2, 4}},
	{.type = ET_GoblinMinion1, .name = "GOBLIN MINION 1", .health = 5, .damage_range = {1, 2}},
	{.type = ET_GoblinMinion2, .name = "GOBLIN MINION 2", .health = 5, .damage_range = {1, 2}},
};

Enemy *get_enemy_data_for_type(EnemyType type) {
	for (u8 i = 0; i < ET_MAX; ++i) {
		if (enemy_defaults[i].type == type)
			return &enemy_defaults[i];
	}

	return NULL;
}
