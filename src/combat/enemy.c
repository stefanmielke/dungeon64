#include "enemy.h"

Enemy enemy_defaults[] = {
	{.type = ET_BlueDragon, .name = "Blue Dragon", .health = 10, .damage_range = {1, 2}},
};

Enemy *get_enemy_data_for_type(EnemyType type) {
	for (u8 i = 0; i < ET_MAX; ++i) {
		if (enemy_defaults[i].type == type)
			return &enemy_defaults[i];
	}
}
