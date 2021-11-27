#include "game_screen.h"

#include "scene_defs.h"

#include <os_host.h>
#include <nustd/math.h>

#include "../math.h"
#include "../data/texture.h"
#include "../fonts/font_ext.h"
#include "../combat/combat.h"
#include "../combat/player.h"
#include "../objects/map_helper.h"
#include "../maps/maps.h"
#include "../text/texts.h"

#include "../../libs/ultra64-extensions/include/easing.h"
#include "../../libs/ultra64-extensions/include/mem_pool.h"
#include "../../libs/ultra64-extensions/include/tween.h"

typedef enum GameState {
	GM_PAUSE,
	GM_VIEW_ITEMS,
	GM_WALK,
	GM_TO_COMBAT,
	GM_START_COMBAT,
	GM_COMBAT,
	GM_FROM_COMBAT,
	GM_START_WALK,
	GM_USING_STAIRS,
	GM_USED_STAIR,
	GM_EXITING_MAP,
	GM_EXIT_MAP
} GameState;
GameState current_state;

Combat current_combat;
Tween *combat_camera_tween;

Tween *screen_transition_tween;
f32 screen_transition_y_top;
f32 screen_transition_y_bottom;

Map current_map;
u16 map_to_load;
s32 forced_position_to_load;
float forced_angle_to_load;

#define RENDER_SCREEN_TRANSITION()                                                                 \
	{                                                                                              \
		gDPSetCycleType(glistp++, G_CYC_FILL);                                                     \
		gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, rsp_cfb);               \
		gDPSetFillColor(glistp++, GPACK_RGBA5551(0, 0, 0, 1) << 16 | GPACK_RGBA5551(0, 0, 0, 1));  \
		gDPFillRectangle(glistp++, 0, screen_transition_y_top, SCREEN_WD - 1,                      \
						 screen_transition_y_bottom);                                              \
	}

// helper functions for movement
void set_angle(float angle_diff);
void move_to(s32 h_speed, s32 forward_speed);

// tween callbacks
void movement_callback(void *target_object, Position current_value);
void movement_end_callback(void *target);  // starts combat when needed
void view_callback(void *target_object, float current_value);

void set_screen_transition_close_bottom_up();
void set_screen_transition_open_bottom_up();
void set_screen_transition_close_top_down();
void set_screen_transition_open_top_down();
void set_screen_transition(float *value_to_change, float initial_value, float end_value);
void screen_transition_callback(void *target_object, float current_value);

void start_combat();
void reset_combat();

void game_screen_set_map(MapDef *map);

void game_screen_set_menu_items();

void game_screen_create() {
	// reset random seed
	u64 seed = osGetTime();
	srand(seed);

	current_state = GM_WALK;

	MapDef *map_def = map_get_def_by_id(map_to_load);
	game_screen_set_map(map_def);

	Vec3 player_start;
	if (forced_position_to_load < 0) {
		player_start = map_get_start_position(&current_map, &player.current_tile, &player.angle);
	} else {
		player.current_tile = forced_position_to_load;
		player.angle = forced_angle_to_load;
		player_start = map_get_position_from_map_coord(forced_position_to_load, current_map.size,
													   current_map.width);
	}
	player_dungeon_init(&player, player_start);

	player.movement_tween = tween_init(&memory_pool);
	player.view_tween = tween_init(&memory_pool);
	screen_transition_tween = tween_init(&memory_pool);
	combat_camera_tween = tween_init(&memory_pool);

	combat_init(&current_combat);
	reset_combat();

	set_angle(0);
	move_to(0, 0);

	menu = menu_init(&memory_pool, player.item_bag.cur_item_bag_count);
	menu_set_hand(menu, 30);

	game_screen_set_menu_items();
}

short game_screen_tick() {
	tween_tick(screen_transition_tween);

	if (current_state == GM_WALK) {
		gd.pad = ReadController(START_BUTTON | B_BUTTON | L_CBUTTONS);
		tween_tick(player.movement_tween);
		tween_tick(player.view_tween);

		// entered combat!
		if (current_state != GM_WALK)
			return SCREEN_PLAY;

		player.move_forward = 0;
		player.move_lateral = 0;
		player.view_speed = 0;

		// move
		if (player.movement_tween->finished && player.view_tween->finished) {
			if (IS_BUTTON_PRESSED(U_JPAD) || PADTHRESH(gd.pad[0]->stick_y) > 0) {
				player.move_forward = TILE_SIZE;
				move_to(player.move_lateral, player.move_forward);
			} else if (IS_BUTTON_PRESSED(D_JPAD) || PADTHRESH(gd.pad[0]->stick_y) < 0) {
				player.move_forward = -TILE_SIZE;
				move_to(player.move_lateral, player.move_forward);
			} else if (IS_BUTTON_PRESSED(L_JPAD) || PADTHRESH(gd.pad[0]->stick_x) < 0) {
				player.view_speed = -RAD_90;
				set_angle(player.view_speed);
			} else if (IS_BUTTON_PRESSED(R_JPAD) || PADTHRESH(gd.pad[0]->stick_x) > 0) {
				player.view_speed = RAD_90;
				set_angle(player.view_speed);
			} else if (IS_BUTTON_PRESSED(L_TRIG) | IS_BUTTON_PRESSED(Z_TRIG)) {
				player.move_lateral = -TILE_SIZE;
				move_to(player.move_lateral, player.move_forward);
			} else if (IS_BUTTON_PRESSED(R_TRIG)) {
				player.move_lateral = TILE_SIZE;
				move_to(player.move_lateral, player.move_forward);
			} else if (IS_BUTTON_PRESSED(L_CBUTTONS)) {
				current_state = GM_VIEW_ITEMS;
			}
		}
	} else if (current_state == GM_TO_COMBAT) {
		if (screen_transition_tween->finished) {
			current_state = GM_START_COMBAT;
			set_screen_transition_open_bottom_up();
		}
	} else if (current_state == GM_START_COMBAT) {
		combat_tick(&current_combat);
		if (screen_transition_tween->finished) {
			current_state = GM_COMBAT;
		}
	} else if (current_state == GM_FROM_COMBAT) {
		if (screen_transition_tween->finished) {
			current_state = GM_START_WALK;
			set_screen_transition_open_top_down();
		}
	} else if (current_state == GM_START_WALK) {
		// if all players are dead exit dungeon
		bool any_players_alive = player_is_any_member_alive(&player);
		if (!any_players_alive) {
			current_state = GM_EXIT_MAP;
		}

		if (screen_transition_tween->finished) {
			current_state = GM_WALK;
		}
	} else if (current_state == GM_COMBAT) {
		combat_tick(&current_combat);
		if (current_combat.state == CS_END) {
			current_state = GM_FROM_COMBAT;
			set_screen_transition_close_top_down();
		}
	} else if (current_state == GM_EXITING_MAP) {
		if (screen_transition_tween->finished) {
			current_state = GM_EXIT_MAP;
		}
	} else if (current_state == GM_EXIT_MAP) {
		return SCREEN_PRE_DUNGEON;
	} else if (current_state == GM_USING_STAIRS) {
		if (screen_transition_tween->finished) {
			current_state = GM_USED_STAIR;
		}
	} else if (current_state == GM_USED_STAIR) {
		return SCREEN_PLAY_MOVE_TO_MAP;
	} else if (current_state == GM_VIEW_ITEMS) {
		gd.pad = ReadController(START_BUTTON | A_BUTTON | B_BUTTON | U_JPAD | D_JPAD | L_JPAD |
								R_JPAD | L_CBUTTONS);

		if (IS_BUTTON_PRESSED(L_CBUTTONS) || IS_BUTTON_PRESSED(B_BUTTON)) {
			current_state = GM_WALK;
		} else {
			int option = menu_tick(menu, false);
			if (option >= 0) {
				player_use_item(&player, option);
				menu_reset_items(menu);
				game_screen_set_menu_items();
			}
		}
	}

	return SCREEN_PLAY;
}

void game_screen_display() {
	if (current_state == GM_WALK || current_state == GM_TO_COMBAT ||
		current_state == GM_START_WALK || current_state == GM_EXITING_MAP ||
		current_state == GM_USING_STAIRS || current_state == GM_VIEW_ITEMS) {
		// set up matrices
		guPerspectiveF(rd.allmat, &rd.perspnorm, 80.0, 320.0 / 240.0, 1.0, 1024.0, 1.0);
		guPerspective(&(rd.dynamicp->projection), &rd.perspnorm, 80.0, 320.0 / 240.0, 1.0, 1024.0,
					  1.0);

		Vec3f forward = {player.pos.x + player.forward.x, player.pos.y + 5.0,
						 player.pos.z + player.forward.z};
		guLookAtF(rd.m2, player.pos.x, forward.y, player.pos.z, forward.x, forward.y, forward.z,
				  0.0, 1.0, 0.0);
		guLookAt(&(rd.dynamicp->viewing), player.pos.x, forward.y, player.pos.z, forward.x,
				 forward.y, forward.z, 0.0, 1.0, 0.0);

		guMtxCatF(rd.m2, rd.allmat, rd.m1);

		guScale(&(rd.dynamicp->modeling), 1.0, 1.0, 1.0);
		guScaleF(rd.modmat, 1.0, 1.0, 1.0);

		guMtxCatF(rd.modmat, rd.m1, rd.allmat);

		gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(rd.dynamicp->projection)),
				  G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
		gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(rd.dynamicp->viewing)),
				  G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
		gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(rd.dynamicp->modeling)),
				  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

		gSPPerspNormalize(glistp++, rd.perspnorm);
		gSPClipRatio(glistp++, FRUSTRATIO_1);

		// render map
		map_render(&current_map, &glistp, rd.dynamicp, &player);

		// party_render(&player.party, &glistp, rd.dynamicp);

		// render text
		font_init(&glistp);
		font_set_transparent(1);
		font_set_scale(1.0, 1.0);
		font_set_win(200, 1);
		FONTCOLM(FONT_COL);

		party_render(&player.party, &glistp, rd.dynamicp, -1);

		char position[100];
		sprintf(position, "Tile: %d Dir: %.2f, %.2f S: %d/%d", player.current_tile,
				player.forward.x, player.forward.z, player.current_steps_taken,
				player.next_combat_at);
		SHOWFONT(&glistp, position, 20, 10);

		if (current_state == GM_VIEW_ITEMS) {
			FONTCOLM(FONT_COL_WHITE);
			SHOWFONT(&glistp, TEXT_ITEMS, 30, 30);
			menu_render(menu, &glistp);
		}

		font_finish(&glistp);

		if (current_state == GM_VIEW_ITEMS) {
			menu_render_images(menu, &glistp);
		}

		if (current_state == GM_TO_COMBAT || current_state == GM_START_WALK ||
			current_state == GM_EXITING_MAP || current_state == GM_USING_STAIRS) {
			RENDER_SCREEN_TRANSITION();
		}
	} else if (current_state == GM_COMBAT || current_state == GM_FROM_COMBAT ||
			   current_state == GM_START_COMBAT) {
		// set up matrices
		guPerspectiveF(rd.allmat, &rd.perspnorm, 80.0, 320.0 / 240.0, 1.0, 1024.0, 1.0);
		guPerspective(&(rd.dynamicp->projection), &rd.perspnorm, 80.0, 320.0 / 240.0, 1.0, 1024.0,
					  1.0);

		const float pov_y = 25;
		float pov_x = current_combat.data.camera_x;

		guLookAtF(rd.m2, pov_x, 4, pov_y, 0, 4, 0, 0.0, 1.0, 0.0);
		guLookAt(&(rd.dynamicp->viewing), pov_x, 4, pov_y, 0, 4, 0, 0.0, 1.0, 0.0);

		guMtxCatF(rd.m2, rd.allmat, rd.m1);

		guScale(&(rd.dynamicp->modeling), 1.0, 1.0, 1.0);
		guScaleF(rd.modmat, 1.0, 1.0, 1.0);

		guMtxCatF(rd.modmat, rd.m1, rd.allmat);

		gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(rd.dynamicp->projection)),
				  G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
		gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(rd.dynamicp->viewing)),
				  G_MTX_PROJECTION | G_MTX_MUL | G_MTX_NOPUSH);
		gSPMatrix(glistp++, OS_K0_TO_PHYSICAL(&(rd.dynamicp->modeling)),
				  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);

		gSPPerspNormalize(glistp++, rd.perspnorm);
		gSPClipRatio(glistp++, FRUSTRATIO_1);

		// render map
		combat_render(&current_map, &current_combat, &glistp, rd.dynamicp, pov_x, pov_y);

		if (current_state == GM_FROM_COMBAT || current_state == GM_START_COMBAT) {
			RENDER_SCREEN_TRANSITION();
		}
	}
}

void set_angle(float angle_diff) {
	float final_angle = player.angle + angle_diff;
	tween_restart(player.view_tween, &player, &easing_exponential_out, MOVEMENT_SPEED, NULL, false,
				  false);
	tween_set_to_float(player.view_tween, player.angle, final_angle, &view_callback);
}

void move_to(s32 h_speed, s32 forward_speed) {
	bool path_is_blocked = false;

	Position final_position;
	if (forward_speed != 0) {
		s8 sign = forward_speed > 0 ? 1 : -1;
		s32 tile = player.current_tile +
				   (((u32)player.forward.x + ((u32)player.forward.z * current_map.width)) * sign);

		path_is_blocked = map_is_tile_blocked(&current_map, tile);
		if (path_is_blocked) {
			// process exit if is an exit wall
			MapEvent *event = map_get_event_on_tile(&current_map, tile);
			if (event) {
				if (event->type == MET_Exit) {
					current_state = GM_EXITING_MAP;
					set_screen_transition_close_bottom_up();
					return;
				} else if (event->type == MET_Stairs) {
					map_to_load = event->args.stairs.map_id;
					forced_position_to_load = event->args.stairs.tile_to_spawn;
					forced_angle_to_load = event->args.stairs.angle;
					current_state = GM_USING_STAIRS;
					set_screen_transition_close_bottom_up();
					return;
				}
			}

			final_position.x = player.pos.x + (player.forward.x * (forward_speed / 4));
			final_position.y = player.pos.z + (player.forward.z * (forward_speed / 4));
		} else {
			final_position.x = player.pos.x + (player.forward.x * forward_speed);
			final_position.y = player.pos.z + (player.forward.z * forward_speed);
			player.current_tile = tile;
			player.current_steps_taken++;
		}
	} else if (h_speed != 0) {
		s8 sign = h_speed > 0 ? 1 : -1;
		float x, y;
		get_forward_vector_from_angle(player.angle + RAD_90, &x, &y);
		u32 tile = player.current_tile + (((u32)x + ((u32)y * current_map.width)) * sign);

		path_is_blocked = map_is_tile_blocked(&current_map, tile);
		if (path_is_blocked) {
			// process exit if is an exit wall
			MapEvent *event = map_get_event_on_tile(&current_map, tile);
			if (event) {
				if (event->type == MET_Exit) {
					current_state = GM_EXITING_MAP;
					set_screen_transition_close_bottom_up();
					return;
				} else if (event->type == MET_Stairs) {
					map_to_load = event->args.stairs.map_id;
					forced_position_to_load = event->args.stairs.tile_to_spawn;
					forced_angle_to_load = event->args.stairs.angle;
					current_state = GM_USING_STAIRS;
					set_screen_transition_close_bottom_up();
					return;
				}
			}

			final_position.x = player.pos.x + (x * (h_speed / 4));
			final_position.y = player.pos.z + (y * (h_speed / 4));
		} else {
			final_position.x = player.pos.x + (x * h_speed);
			final_position.y = player.pos.z + (y * h_speed);
			player.current_tile = tile;
			player.current_steps_taken++;
		}
	} else {
		final_position.x = player.pos.x;
		final_position.y = player.pos.z;
	}

	tween_restart(player.movement_tween, &player, &easing_exponential_out, MOVEMENT_SPEED,
				  &movement_end_callback, path_is_blocked, false);
	Position p = {player.pos.x, player.pos.z};
	tween_set_to_position(player.movement_tween, p, final_position, &movement_callback);
}

void movement_callback(void *target_object, Position current_value) {
	player.pos.x = current_value.x;
	player.pos.z = current_value.y;
}

void movement_end_callback(void *target) {
	if (current_map.has_random_encounters && player.current_steps_taken >= player.next_combat_at) {
		start_combat();
	}
}

void view_callback(void *target_object, float current_value) {
	player.angle = current_value;
	get_forward_vector_from_angle(player.angle, &player.forward.x, &player.forward.z);
}

void screen_transition_callback(void *target_object, float current_value) {
	float *target = target_object;
	*target = current_value;
}

void set_screen_transition_close_bottom_up() {
	set_screen_transition(&screen_transition_y_top, SCREEN_HT - 1, 0);
	screen_transition_y_bottom = SCREEN_HT - 1;
}

void set_screen_transition_open_bottom_up() {
	screen_transition_y_top = 0;
	set_screen_transition(&screen_transition_y_bottom, SCREEN_HT - 1, 0);
}

void set_screen_transition_close_top_down() {
	screen_transition_y_top = 0;
	set_screen_transition(&screen_transition_y_bottom, 0, SCREEN_HT - 1);
}

void set_screen_transition_open_top_down() {
	set_screen_transition(&screen_transition_y_top, 0, SCREEN_HT - 1);
	screen_transition_y_bottom = SCREEN_HT - 1;
}

void set_screen_transition(float *value_to_change, float initial_value, float end_value) {
	tween_restart(screen_transition_tween, value_to_change, &easing_linear, 1000, NULL, false,
				  false);
	tween_set_to_float(screen_transition_tween, initial_value, end_value,
					   &screen_transition_callback);
}

void start_combat() {
	reset_combat();

	current_state = GM_TO_COMBAT;
	combat_new(&current_combat, &player.party, combat_camera_tween);
	set_screen_transition_close_bottom_up();
}

void reset_combat() {
	player.next_combat_at = player.current_steps_taken +
							range_get_from_int(&current_map.steps_to_combat);
}

void game_screen_set_map(MapDef *map) {
	current_map.tiles = map->tiles;
	current_map.size = map->size;
	current_map.width = map->width;
	current_map.steps_to_combat = map->steps_to_combat;
	current_map.has_random_encounters = map->has_random_encounters;
	current_map.events = map->events;
	current_map.spr_ground = map->spr_ground;
	current_map.spr_wall = map->spr_wall;
	current_map.spr_wall_exit = map->spr_wall_exit;
	current_map.spr_wall_upstairs = map->spr_wall_upstairs;
}

/* VIEW ITEMS */

void game_screen_set_menu_items() {
	const int x = 40, start_y = 60;
	for (u8 i = 0; i < player.item_bag.cur_item_bag_count; ++i) {
		menu_add_item(menu, player.item_bag.items[i].item_def->name, x, start_y + (i * 20), true);
	}
}

/* VIEW ITEMS END*/
