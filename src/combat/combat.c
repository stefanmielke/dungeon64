#include "combat.h"

#include <nustd/math.h>
#include "../../libs/ultra64-extensions/include/mem_pool.h"
#include "../../libs/ultra64-extensions/include/easing.h"

#include "combat_animations.h"
#include "enemy_party.h"
#include "../static.h"
#include "../data/texture.h"
#include "../math.h"
#include "../objects/billboards.h"
#include "../objects/combat/enemies.h"
#include "../objects/combat/player.h"
#include "../scenes/scene_defs.h"
#include "../text/texts.h"

#define get_ticks_ms() (OS_CYCLES_TO_NSEC(osGetTime()) / 1000000)
u64 last_tick;

void combat_process_action(Combat *combat, CombatAction *action);
u8 get_enemy_size(EnemyType type);
void set_camera_movement(Combat *combat, float from, float to, u16 time_in_ms);
void reset_menus(Combat *combat);

void combat_init(Combat *combat) {
	combat->actions_menu = menu_init(&memory_pool, 5);
	const int x = 20, start_y = 20;
	menu_add_item(combat->actions_menu, TEXT_COMBAT_ATK, x, start_y, true);
	menu_add_item(combat->actions_menu, TEXT_COMBAT_DEF, x, start_y + 20, true);
	menu_add_item(combat->actions_menu, TEXT_COMBAT_SKL, x, start_y + 40, true);
	menu_add_item(combat->actions_menu, TEXT_COMBAT_ITM, x, start_y + 60, true);
	menu_add_item(combat->actions_menu, TEXT_COMBAT_RUN, x, start_y + 80, true);

	// atk, skill, items
	menu_init_submenus(combat->actions_menu, &memory_pool, 3, ITEM_BAG_MAX_ITEM_COUNT);

	// init items with an additional submenu for chars and enemies (2)
	Menu *items_submenu = combat->actions_menu->submenus[2];
	menu_init_submenus(items_submenu, &memory_pool, 2, 4);
}

void combat_new(Combat *combat, Party *party, Tween *camera_tween) {
	combat->party = party;
	combat->enemy_party = get_new_enemy_party();
	combat->state = CS_START;
	combat->data.selected = 0;
	combat->data.current_member_choosing = 0;
	combat->data.camera_x = -10;
	combat->data.timer_target = 0;
	combat->data.current_attacker = 0;
	combat->data.current_defender = 0;
	combat->data.camera_tween = camera_tween;

	set_camera_movement(combat, -10, 0, 1000);

	last_tick = get_ticks_ms();
}

void combat_tick(Combat *combat) {
	gd.pad = ReadController(START_BUTTON | A_BUTTON | B_BUTTON | U_JPAD | D_JPAD);
	if (IS_BUTTON_PRESSED(START_BUTTON)) {
		combat->state = CS_ENDING;
	}

	tween_tick(combat->data.camera_tween);

	switch (combat->state) {
		case CS_PLAYER_PHASE:
			// skipping dead party members
			for (u8 i = 0; i < combat->party->current_member_count; ++i) {
				if (combat->party->members[combat->data.current_member_choosing].current_health <=
					0)
					combat->data.current_member_choosing++;
			}
			if (combat->data.current_member_choosing >= combat->party->current_member_count) {
				combat->state = CS_RUN_COMBAT;
				break;
			}

			menu_tick(combat->actions_menu, false);

			if (IS_BUTTON_PRESSED(U_JPAD)) {
				combat->data.selected--;
				if (combat->data.selected < 0)
					combat->data.selected = combat->enemy_party.current_enemy_count - 1;
			} else if (IS_BUTTON_PRESSED(D_JPAD)) {
				combat->data.selected++;
				if (combat->data.selected >= combat->enemy_party.current_enemy_count)
					combat->data.selected = 0;
			} else if (IS_BUTTON_PRESSED(A_BUTTON)) {
				// selected action
				/*
				Menu:
				- Atk -> Show enemies -> Go
				- Def -> Go
				- Skill -> Show skills -> Go
				- Items -> Show Items -> Show targets (chars/enemies) -> Go
				- Run -> Go
				*/
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
					set_camera_movement(combat, combat->data.camera_x, -10, 4000);
				} else {
					reset_menus(combat);
				}
			} else if (IS_BUTTON_PRESSED(B_BUTTON)) {
				if (combat->data.current_member_choosing > 0)
					combat->data.current_member_choosing--;
			}
			break;
		case CS_RUN_COMBAT: {
			u64 current_time = get_ticks_ms();
			if (combat->data.timer_target == 0) {
				// combat just started
				combat->data.timer_target = current_time + 500;
			} else if (current_time >= combat->data.timer_target) {
				// end combat phase if everyone did their action
				u8 total_attackers = combat->party->current_member_count +
									 combat->enemy_party.current_enemy_count;
				if (combat->data.current_attacker >= total_attackers) {
					combat->state = CS_PLAYER_PHASE;
					set_camera_movement(combat, combat->data.camera_x, 5, 1000);
					combat->data.timer_target = 0;
					combat->data.current_attacker = 0;
					combat->data.current_defender = 0;

					combat->data.selected = 0;
					for (u8 i = 0; i < combat->enemy_party.current_enemy_count; ++i) {
						if (combat->enemy_party.enemies[combat->data.selected].current_health <= 0)
							combat->data.selected++;
					}
					if (combat->data.selected >= combat->enemy_party.current_enemy_count)
						combat->data.selected = 0;

					combat->data.current_member_choosing = 0;
					reset_menus(combat);
					break;
				}

				// end the combat if one side loses
				u8 enemies_alive = 0;
				for (u8 i = 0; i < combat->enemy_party.current_enemy_count; i++) {
					if (combat->enemy_party.enemies[i].current_health > 0)
						enemies_alive++;
				}
				u8 players_alive = 0;
				for (u8 i = 0; i < combat->party->current_member_count; i++) {
					if (combat->party->members[i].current_health > 0)
						players_alive++;
				}

				// one side lost
				if (enemies_alive == 0 || players_alive == 0) {
					combat->state = CS_ENDING;
					combat->data.timer_target = current_time + 1000;
					return;
				}

				combat->data.timer_target = current_time + 500;

				u8 current_attacker = combat->data.current_attacker;
				if (current_attacker < combat->party->current_member_count) {
					// player attacking
					if (combat->party->members[current_attacker].current_health > 0) {
						combat_process_action(combat,
											  &combat->data.player_actions[current_attacker]);
					} else {
						// rushes another attacker next frame
						combat->data.timer_target = current_time;
					}
				} else {
					// enemy attacking
					u8 current_enemy = current_attacker - combat->party->current_member_count;
					if (combat->enemy_party.enemies[current_enemy].current_health > 0) {
						CombatAction action = {
							.target = RANDR(0, combat->party->current_member_count - 1),
							.target_is_enemy = false,
							.type = CAT_ATK_PHYS,
							.type_arg_1 = range_get_from_int(
								&combat->enemy_party.enemies[current_enemy].enemy->damage_range),
						};
						combat_process_action(combat, &action);
					} else {
						// rushes another attacker next frame
						combat->data.timer_target = current_time;
					}
				}
				combat->data.current_attacker++;
			}
		} break;
		case CS_START:
			if (combat->data.camera_tween->finished) {
				combat->state = CS_PLAYER_PHASE;
				set_camera_movement(combat, combat->data.camera_x, 5, 1000);
				reset_menus(combat);
			}
			break;
		case CS_ENDING: {
			u64 current_time = get_ticks_ms();
			if (current_time >= combat->data.timer_target)
				combat->state = CS_END;
		}  // purposefully goes down to the next
		case CS_END:
			if (combat->data.camera_x > -10)
				combat->data.camera_x -= 0.1f;
		default:
			break;
	}
}

void combat_process_action(Combat *combat, CombatAction *action) {
	switch (action->type) {
		case CAT_ATK_PHYS:
			if (action->target_is_enemy) {
				// if target died before this action, choose another enemy
				if (combat->enemy_party.enemies[action->target].current_health <= 0) {
					action->target = 0;
					while (action->target < combat->enemy_party.current_enemy_count) {
						if (combat->enemy_party.enemies[action->target].current_health > 0)
							break;
						action->target++;
					}
				}

				if (action->target < combat->enemy_party.current_enemy_count) {
					combat->enemy_party.enemies[action->target].current_health -= action
																					  ->type_arg_1;
					combat->data.current_defender = action->target;
				}
			} else {
				// if target died before this action, choose another enemy
				if (combat->party->members[action->target].current_health <= 0) {
					// tries random 4 times before pushing 1-4
					for (u8 i = 0; i < 4; ++i) {
						int random = RANDR(0, combat->party->current_member_count - 1);
						if (combat->party->members[random].current_health > 0) {
							action->target = random;
							break;
						}
					}
				}
				// if random failed, do 1-4
				if (combat->party->members[action->target].current_health <= 0) {
					action->target = 0;
					while (action->target < combat->party->current_member_count) {
						if (combat->party->members[action->target].current_health > 0)
							break;
						action->target++;
					}
				}

				if (action->target < combat->party->current_member_count) {
					combat->party->members[action->target].current_health -= action->type_arg_1;
					combat->data.current_defender = action->target;
				}
			}
			break;
		default:
			break;
	}
}

void combat_render(Map *map, Combat *combat, Gfx **glistp, Dynamic *dynamicp, int pov_x,
				   int pov_z) {
	int obj_count = 0;		 /* count of used objects on current frame */
	int billboard_count = 0; /* count of used billboards on current frame */

	u64 this_tick = get_ticks_ms();
	static float frame_counter = 0;
	frame_counter += 0.09f * ((this_tick - last_tick) / 20.f);
	last_tick = this_tick;

	if (combat->state == CS_PLAYER_PHASE) {
		const u8 enemy_size = get_enemy_size(
			combat->enemy_party.enemies[combat->data.selected].enemy->type);

		const float x = (-1.5 * enemy_size) - (3 * combat->data.selected);
		const float y = -5 + combat->data.selected * 3;

		gSPDisplayList((*glistp)++, combat_selection_setup_dl);
		guTranslate(&dynamicp->object_position[obj_count], x, 5 * enemy_size, y);
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamicp->object_position[obj_count])),
				  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
		guRotate(&dynamicp->billboard_rotation[billboard_count], frame_counter * 10, 0, 1, 0);
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamicp->billboard_rotation[billboard_count])),
				  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);
		gSPDisplayList((*glistp)++, combat_selection_dl);
		obj_count++;
		billboard_count++;
	}

	// ground
	gSPDisplayList((*glistp)++, ground_texture_setup_dl);
	gSPTexture((*glistp)++, 1024 * 100, 1024 * 100, 0, G_TX_RENDERTILE, G_ON);
	gDPLoadTextureBlock((*glistp)++, map->spr_ground, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
						G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

	gDPPipeSync((*glistp)++);
	guTranslate(&dynamicp->object_position[obj_count], -50, 0, -50);
	gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamicp->object_position[obj_count])),
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
	gSPDisplayList((*glistp)++, combat_ground_dl);
	obj_count++;

	// billboard setup
	gSPDisplayList((*glistp)++, billboard_texture_setup_dl);
	gSPTexture((*glistp)++, 2048, 2048, 0, G_TX_RENDERTILE, G_ON);

	s8 current_attacker = combat->data.current_attacker - 1;
	for (u8 i = 0; i < combat->enemy_party.current_enemy_count; ++i) {
		if (combat->enemy_party.enemies[i].current_health > 0) {
			if (current_attacker >= combat->party->current_member_count &&
				current_attacker - combat->party->current_member_count == i) {
				DRAW_ENEMY(combat->enemy_party.enemies[i].enemy->type, 0 - (3 * i), -5 + i * 3,
						   pov_x, pov_z, (int)frame_counter);
			} else {
				DRAW_ENEMY(combat->enemy_party.enemies[i].enemy->type, -3 - (3 * i), -5 + i * 3,
						   pov_x, pov_z, (int)frame_counter);
			}
		}
	}

	for (u8 i = 0; i < combat->party->current_member_count; ++i) {
		if (combat->party->members[i].current_health > 0) {
			if (combat->state == CS_PLAYER_PHASE) {
				if (combat->data.current_member_choosing == i) {
					DRAW_CLASS(combat->party->members[i].class, combat->party->members[i].gender,
							   4 + (3 * i), 5 - i * 3, pov_x, pov_z, (int)frame_counter, 3, idle);
				} else {
					DRAW_CLASS(combat->party->members[i].class, combat->party->members[i].gender,
							   3 + (3 * i), 5 - i * 3, pov_x, pov_z, (int)frame_counter, 3, idle);
				}
			} else if (combat->state == CS_RUN_COMBAT) {
				if (current_attacker < combat->party->current_member_count) {
					// a player is attacking
					if (current_attacker == i) {
						DRAW_CLASS(combat->party->members[i].class,
								   combat->party->members[i].gender, 0 + (3 * i), 5 - i * 3, pov_x,
								   pov_z, (int)frame_counter, 3, attack_1);
					} else {
						DRAW_CLASS(combat->party->members[i].class,
								   combat->party->members[i].gender, 3 + (3 * i), 5 - i * 3, pov_x,
								   pov_z, (int)frame_counter, 3, idle);
					}
				} else {
					// an enemy is attacking
					if (combat->data.current_defender == i) {
						DRAW_CLASS(combat->party->members[i].class,
								   combat->party->members[i].gender, 3 + (3 * i), 5 - i * 3, pov_x,
								   pov_z, (int)frame_counter, 3, hit);
					} else {
						DRAW_CLASS(combat->party->members[i].class,
								   combat->party->members[i].gender, 3 + (3 * i), 5 - i * 3, pov_x,
								   pov_z, (int)frame_counter, 3, idle);
					}
				}
			} else if (combat->state == CS_START) {
				DRAW_CLASS(combat->party->members[i].class, combat->party->members[i].gender,
						   3 + (3 * i), 5 - i * 3, pov_x, pov_z, (int)frame_counter, 3, idle);
			} else if (combat->state == CS_END || combat->state == CS_ENDING) {
				DRAW_CLASS(combat->party->members[i].class, combat->party->members[i].gender,
						   3 + (3 * i), 5 - i * 3, pov_x, pov_z, (int)frame_counter, 4, win);
			}
		} else {
			DRAW_CLASS(combat->party->members[i].class, combat->party->members[i].gender,
					   3 + (3 * i), 5 - i * 3, pov_x, pov_z, (int)frame_counter, 1, dead);
		}
	}

	font_init(glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);

	// render enemy data
	// char text[100];
	// for (u8 i = 0; i < combat->enemy_party.current_enemy_count; ++i) {
	// 	const int start_y = 20 + (i * 15);
	// 	EnemyCombat *member = &combat->enemy_party.enemies[i];

	// 	float health_perc = member->current_health / (float)member->enemy->health;
	// 	if (health_perc > .7f) {
	// 		FONTCOLM(FONT_COL_GREEN);
	// 	} else if (health_perc > .3f) {
	// 		FONTCOLM(FONT_COL_YELLOW);
	// 	} else if (health_perc > 0) {
	// 		FONTCOLM(FONT_COL_RED);
	// 	} else {
	// 		FONTCOLM(FONT_COL_GREY);
	// 	}

	// 	if (combat->state == CS_PLAYER_PHASE && combat->data.selected == i)
	// 		sprintf(text, "-%s %d/%d", member->enemy->name, member->current_health,
	// 				member->enemy->health);
	// 	else
	// 		sprintf(text, " %s %d/%d", member->enemy->name, member->current_health,
	// 				member->enemy->health);
	// 	SHOWFONT(glistp, text, 20, start_y);
	// }

	party_render(combat->party, glistp, dynamicp,
				 combat->state == CS_PLAYER_PHASE ? combat->data.current_member_choosing : -1);

	if (combat->state == CS_PLAYER_PHASE)
		menu_render(combat->actions_menu, glistp);

	font_finish(glistp);
}

u8 get_enemy_size(EnemyType type) {
	switch (type) {
		case ET_GoblinBoss:
			return 2;
		default:
			return 1;
	}
}

void combat_camera_movement_callback(void *target, float value) {
	Combat *combat = target;
	combat->data.camera_x = value;
}

void set_camera_movement(Combat *combat, float from, float to, u16 time_in_ms) {
	Tween *tween = combat->data.camera_tween;
	tween_restart(tween, combat, &easing_sine_in_out, time_in_ms, NULL, false, false);
	tween_set_to_float(tween, from, to, &combat_camera_movement_callback);
}

void reset_menus(Combat *combat) {
}