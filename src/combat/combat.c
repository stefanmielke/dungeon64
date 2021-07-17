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
	Enemy *blue_dragon = get_enemy_data_for_type(ET_BlueDragon);

	EnemyParty party = {
		.current_enemy_count = 3,
		.enemies =
			{
				{
					.enemy = blue_dragon,
					.current_health = blue_dragon->health,
				},
				{
					.enemy = blue_dragon,
					.current_health = blue_dragon->health,
				},
				{
					.enemy = blue_dragon,
					.current_health = blue_dragon->health,
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
		if (combat->enemy_party.enemies[i].enemy->type == ET_BlueDragon) {
			DRAW_BLUE_DRAGON(-3 - (3 * i), -5 + i * 3, pov_x, pov_z, (int)frame_counter);
		}
	}

	for (u8 i = 0; i < combat->party->current_member_count; ++i) {
		if (combat->party->members[i].class == PC_Warrior) {
			DRAW_WARRIOR(3 + (3 * i), 5 - i * 3, pov_x, pov_z, (int)frame_counter);
		}
	}

	// render text
	font_init(glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);
	FONTCOLM(FONT_COL);
	char position[100];
	sprintf(position, "Combat");
	SHOWFONT(glistp, position, 20, 210);
	font_finish(glistp);
}
