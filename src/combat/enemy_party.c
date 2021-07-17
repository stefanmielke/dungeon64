#include "enemy_party.h"

typedef struct EnemyPartyDef {
	EnemyType enemies[4];
	u8 party_size;
} EnemyPartyDef;

#define ENEMY_PARTY_DEFS_SIZE 5
EnemyPartyDef defs[ENEMY_PARTY_DEFS_SIZE] = {
	{
		.party_size = 4,
		.enemies = {ET_GoblinBoss, ET_GoblinMinion1, ET_GoblinMinion2, ET_GoblinMinion2},
	},
	{
		.party_size = 2,
		.enemies = {ET_GoblinMinion1, ET_GoblinMinion2},
	},
	{
		.party_size = 1,
		.enemies = {ET_GoblinMinion1},
	},
	{
		.party_size = 1,
		.enemies = {ET_GoblinMinion2},
	},
	{
		.party_size = 3,
		.enemies = {ET_GoblinMinion1, ET_GoblinMinion2, ET_GoblinMinion1},
	},
};

EnemyParty get_new_enemy_party() {
	int random_party = RANDR(0, ENEMY_PARTY_DEFS_SIZE - 1);
	EnemyPartyDef def = defs[random_party];

	EnemyParty party = {.current_enemy_count = def.party_size};
	for (u8 i = 0; i < def.party_size; ++i) {
		Enemy *enemy = get_enemy_data_for_type(def.enemies[i]);
		party.enemies[i].enemy = enemy;
		party.enemies[i].current_health = enemy->health;
	}

	return party;
}
