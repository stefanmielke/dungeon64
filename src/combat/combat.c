#include "combat.h"

#include <nustd/math.h>

#include "combat_animations.h"
#include "../static.h"
#include "../data/texture.h"
#include "../math.h"
#include "../objects/billboards.h"
#include "../objects/combat/enemies.h"
#include "../objects/combat/player.h"

EnemyParty get_new_enemy_party() {
	Enemy *goblin_boss = get_enemy_data_for_type(ET_GoblinBoss);
	Enemy *goblin_minion1 = get_enemy_data_for_type(ET_GoblinMinion1);
	Enemy *goblin_minion2 = get_enemy_data_for_type(ET_GoblinMinion2);

	EnemyParty party = {
		.current_enemy_count = 4,
		.enemies =
			{
				{
					.enemy = goblin_boss,
					.current_health = goblin_boss->health,
				},
				{
					.enemy = goblin_minion1,
					.current_health = goblin_minion1->health,
				},
				{
					.enemy = goblin_minion2,
					.current_health = goblin_minion2->health,
				},
				{
					.enemy = goblin_minion2,
					.current_health = goblin_minion2->health,
				},
			},
	};

	return party;
}

Combat combat_new(Party *party) {
	Combat combat = {
		.party = party,
		.enemy_party = get_new_enemy_party(),
	};

	return combat;
}

void combat_render(Combat *combat, Gfx **glistp, Dynamic *dynamicp, int pov_x, int pov_z) {
	int obj_count = 0;		 /* count of used objects on current frame */
	int billboard_count = 0; /* count of used billboards on current frame */

	static float frame_counter = 0;
	frame_counter += 0.09f;

	// ground
	gSPDisplayList((*glistp)++, ground_texture_setup_dl);
	gSPTexture((*glistp)++, 1024 * 100, 1024 * 100, 0, G_TX_RENDERTILE, G_ON);
	gDPLoadTextureBlock((*glistp)++, spr_ground, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_WRAP,
						G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

	gDPPipeSync((*glistp)++);
	guTranslate(&dynamicp->object_position[obj_count], -50, 0, -50);
	gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamicp->object_position[obj_count])),
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
	gSPDisplayList((*glistp)++, combat_ground_dl);
	obj_count++;

	// billboard setup
	gSPDisplayList((*glistp)++, billboard_texture_setup_dl);
	gSPTexture((*glistp)++, 2048, 2048, 0, G_TX_RENDERTILE, G_ON);

	for (u8 i = 0; i < combat->enemy_party.current_enemy_count; ++i) {
		DRAW_ENEMY(combat->enemy_party.enemies[i].enemy->type, -3 - (3 * i), -5 + i * 3, pov_x,
				   pov_z, (int)frame_counter);
	}

	for (u8 i = 0; i < combat->party->current_member_count; ++i) {
		DRAW_CLASS(combat->party->members[i].class, 3 + (3 * i), 5 - i * 3, pov_x, pov_z,
				   (int)frame_counter);
	}

	party_render(combat->party, glistp, dynamicp);
}
