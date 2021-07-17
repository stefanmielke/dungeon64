#include "enemy.h"

Enemy enemy_defaults[] = {
	{.type = ET_GoblinBoss, .name = "Goblin King", .health = 8, .damage_range = {1, 2}},
	{.type = ET_GoblinMinion1, .name = "Goblin Minion 1", .health = 5, .damage_range = {1, 2}},
	{.type = ET_GoblinMinion2, .name = "Goblin Minion 2", .health = 5, .damage_range = {1, 2}},
};

Enemy *get_enemy_data_for_type(EnemyType type) {
	for (u8 i = 0; i < ET_MAX; ++i) {
		if (enemy_defaults[i].type == type)
			return &enemy_defaults[i];
	}
}
