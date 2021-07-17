#include "combat.h"

#include <nustd/math.h>

#include "combat_animations.h"
#include "../static.h"
#include "../data/texture.h"
#include "../math.h"
#include "../objects/billboards.h"
#include "../objects/combat/enemies.h"
#include "../objects/combat/player.h"
#include "../scenes/scene_defs.h"

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
		.state = CS_START,
		.data =
			{
				.selected = 0,
				.current_member_choosing = 0,
				.camera_x = -10,
			},
	};

	return combat;
}

void combat_tick(Combat *combat) {
	gd.pad = ReadController(START_BUTTON | A_BUTTON | B_BUTTON | U_JPAD | D_JPAD);
	if (IS_BUTTON_PRESSED(START_BUTTON)) {
		combat->state = CS_END;
	}

	static float camera_speed = 0.1f;
	if (combat->state == CS_PLAYER_PHASE) {
		if (combat->data.camera_x < 5)
			combat->data.camera_x += camera_speed;
	} else {
		if (combat->data.camera_x > 0)
			combat->data.camera_x -= camera_speed;
		else if (combat->data.camera_x < 0)
			combat->data.camera_x += camera_speed;
	}

	switch (combat->state) {
		case CS_PLAYER_PHASE:
			if (combat->data.camera_x < 5)
				combat->data.camera_x += camera_speed;

			if (IS_BUTTON_PRESSED(U_JPAD)) {
				combat->data.selected--;
				if (combat->data.selected < 0)
					combat->data.selected = combat->enemy_party.current_enemy_count - 1;
			} else if (IS_BUTTON_PRESSED(D_JPAD)) {
				combat->data.selected++;
				if (combat->data.selected >= combat->enemy_party.current_enemy_count)
					combat->data.selected = 0;
			} else if (IS_BUTTON_PRESSED(A_BUTTON)) {
				u8 member_index = combat->data.current_member_choosing;
				CombatAction *action = &combat->data.player_actions[member_index];
				action->target = combat->data.selected;
				action->target_is_enemy = true;
				action->type = CAT_ATK_PHYS;
				action->type_arg_1 = range_get_from_int(
					&combat->party->members[member_index].damage_range);

				combat->data.current_member_choosing++;
				if (combat->data.current_member_choosing >= combat->party->current_member_count) {
					combat->state = CS_RUN_COMBAT;
				}
			}
			break;
		case CS_RUN_COMBAT:
			if (combat->data.camera_x > 0)
				combat->data.camera_x -= camera_speed;

			if (IS_BUTTON_PRESSED(A_BUTTON)) {
				combat->state = CS_PLAYER_PHASE;
				combat->data.current_member_choosing = 0;
				combat->data.selected = 0;
			}
			break;
		case CS_START:
			if (combat->data.camera_x < 0)
				combat->data.camera_x += camera_speed;
		case CS_END:
			if (combat->data.camera_x > -10)
				combat->data.camera_x -= camera_speed;
		default:
			break;
	}
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

	font_init(glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	// render enemy data
	char text[100];
	for (u8 i = 0; i < combat->enemy_party.current_enemy_count; ++i) {
		const int start_y = 20 + (i * 15);
		EnemyCombat *member = &combat->enemy_party.enemies[i];

		float health_perc = member->current_health / (float)member->enemy->health;
		if (health_perc > .7f) {
			FONTCOLM(FONT_COL_GREEN);
		} else if (health_perc > .3f) {
			FONTCOLM(FONT_COL_YELLOW);
		} else {
			FONTCOLM(FONT_COL_RED);
		}

		if (combat->state == CS_PLAYER_PHASE && combat->data.selected == i)
			sprintf(text, "-%s", member->enemy->name);
		else
			sprintf(text, " %s", member->enemy->name);
		SHOWFONT(glistp, text, 20, start_y);
	}

	party_render(combat->party, glistp, dynamicp,
				 combat->state == CS_PLAYER_PHASE ? combat->data.current_member_choosing : -1);

	font_finish(glistp);
}
