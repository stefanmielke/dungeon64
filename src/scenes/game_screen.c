#include "game_screen.h"

#include "scene_defs.h"

#include <os_host.h>
#include <nustd/math.h>

#include "../math.h"
#include "../data/texture.h"
#include "../fonts/font_ext.h"
#include "../objects/combat_helper.h"
#include "../objects/map_helper.h"
#include "../objects/player.h"
#include "../maps/maps.h"

#include "../../libs/ultra64-extensions/include/easing.h"
#include "../../libs/ultra64-extensions/include/mem_pool.h"
#include "../../libs/ultra64-extensions/include/tween.h"

Map current_map;
Player player;

typedef enum GameState {
	GM_PAUSE,
	GM_WALK,
	GM_TO_COMBAT,
	GM_COMBAT,
	GM_FROM_COMBAT,
} GameState;
GameState current_state;

Tween *combat_start_end_tween;
s16 screen_transition_y;

// helper functions for movement
void set_angle(float angle_diff);
void move_to(s32 h_speed, s32 forward_speed);

// tween callbacks
void movement_callback(void *target_object, Position current_value);
void movement_end_callback(void *target);  // starts combat when needed
void view_callback(void *target_object, float current_value);

void reset_combat() {
	player.next_combat_at = player.current_steps_taken +
							range_get_from_int(&current_map.steps_to_combat);
}

void game_screen_create() {
	// reset random seed
	unsigned int seed = osGetTime();
	srand(seed);

	current_state = GM_WALK;

	player.movement_tween = tween_init(&memory_pool);
	player.view_tween = tween_init(&memory_pool);
	combat_start_end_tween = tween_init(&memory_pool);

	RangeInt r1 = {10, 10};

	current_map.tiles = map1_1;
	current_map.size = map1_1_size;
	current_map.width = map1_1_width;
	current_map.steps_to_combat.start = 10;
	current_map.steps_to_combat.end = 20;

	Vec3 player_start = map_get_start_position(&current_map, &player.current_tile);
	player.pos[0] = player_start.x;
	player.pos[1] = player_start.y;
	player.pos[2] = player_start.z;
	player.angle = 0;

	player.current_steps_taken = 0;
	reset_combat();

	set_angle(0);
	move_to(0, 0);
}

short game_screen_tick() {
	gd.pad = ReadController(START_BUTTON | B_BUTTON);

	if (current_state == GM_WALK) {
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
			}
		}
	} else if (current_state == GM_TO_COMBAT) {
		screen_transition_y -= 5;
		if (screen_transition_y < 0)
			current_state = GM_COMBAT;

	} else if (current_state == GM_FROM_COMBAT) {
		screen_transition_y += 5;
		if (screen_transition_y > SCREEN_HT + 5)
			current_state = GM_WALK;

	} else if (current_state == GM_COMBAT) {
		if (IS_BUTTON_PRESSED(B_BUTTON)) {
			current_state = GM_FROM_COMBAT;
			screen_transition_y = 0;
			return SCREEN_PLAY;
		}
	}

	return SCREEN_PLAY;
}

void game_screen_display() {
	if (current_state == GM_WALK || current_state == GM_TO_COMBAT) {
		// set up matrices
		guPerspectiveF(rd.allmat, &rd.perspnorm, 80.0, 320.0 / 240.0, 1.0, 1024.0, 1.0);
		guPerspective(&(rd.dynamicp->projection), &rd.perspnorm, 80.0, 320.0 / 240.0, 1.0, 1024.0,
					  1.0);

		Vec3f forward = {player.pos[0] + player.forward[0], player.pos[1] + 5.0,
						 player.pos[2] + player.forward[2]};
		guLookAtF(rd.m2, player.pos[0], forward[1], player.pos[2], forward[0], forward[1],
				  forward[2], 0.0, 1.0, 0.0);
		guLookAt(&(rd.dynamicp->viewing), player.pos[0], forward[1], player.pos[2], forward[0],
				 forward[1], forward[2], 0.0, 1.0, 0.0);

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

		// render text
		font_init(&glistp);
		font_set_transparent(1);
		font_set_scale(1.0, 1.0);
		font_set_win(200, 1);
		FONTCOLM(FONT_COL);
		char position[100];
		sprintf(position, "Tile: %d Dir: %.2f, %.2f S: %d/%d", player.current_tile,
				player.forward[0], player.forward[2], player.current_steps_taken,
				player.next_combat_at);
		SHOWFONT(&glistp, position, 20, 210);
		font_finish(&glistp);

		if (current_state == GM_TO_COMBAT) {
			gDPSetCycleType(glistp++, G_CYC_FILL);
			gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, rsp_cfb);
			gDPSetFillColor(glistp++,
							GPACK_RGBA5551(0, 0, 0, 1) << 16 | GPACK_RGBA5551(0, 0, 0, 1));
			gDPFillRectangle(glistp++, 0, screen_transition_y, SCREEN_WD - 1, SCREEN_HT - 1);
		}
	} else if (current_state == GM_COMBAT || current_state == GM_FROM_COMBAT) {
		// set up matrices
		guPerspectiveF(rd.allmat, &rd.perspnorm, 80.0, 320.0 / 240.0, 1.0, 1024.0, 1.0);
		guPerspective(&(rd.dynamicp->projection), &rd.perspnorm, 80.0, 320.0 / 240.0, 1.0, 1024.0,
					  1.0);

		static float pov_x = -5, pov_y = 25, speed = 0.1f;
		pov_x += speed;
		if (pov_x > 5 || pov_x < -5)
			speed *= -1;

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
		combat_render(&glistp, rd.dynamicp, pov_x, pov_y);

		if (current_state == GM_FROM_COMBAT) {
			gDPSetCycleType(glistp++, G_CYC_FILL);
			gDPSetColorImage(glistp++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WD, rsp_cfb);
			gDPSetFillColor(glistp++,
							GPACK_RGBA5551(0, 0, 0, 1) << 16 | GPACK_RGBA5551(0, 0, 0, 1));
			gDPFillRectangle(glistp++, 0, 0, SCREEN_WD - 1, screen_transition_y);
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
				   (((u32)player.forward[0] + ((u32)player.forward[2] * current_map.width)) * sign);

		path_is_blocked = map_is_tile_blocked(&current_map, tile);
		if (path_is_blocked) {
			final_position.x = player.pos[0] + (player.forward[0] * (forward_speed / 4));
			final_position.y = player.pos[2] + (player.forward[2] * (forward_speed / 4));
		} else {
			final_position.x = player.pos[0] + (player.forward[0] * forward_speed);
			final_position.y = player.pos[2] + (player.forward[2] * forward_speed);
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
			final_position.x = player.pos[0] + (x * (h_speed / 4));
			final_position.y = player.pos[2] + (y * (h_speed / 4));
		} else {
			final_position.x = player.pos[0] + (x * h_speed);
			final_position.y = player.pos[2] + (y * h_speed);
			player.current_tile = tile;
			player.current_steps_taken++;
		}
	} else {
		final_position.x = player.pos[0];
		final_position.y = player.pos[2];
	}

	tween_restart(player.movement_tween, &player, &easing_exponential_out, MOVEMENT_SPEED,
				  &movement_end_callback, path_is_blocked, false);
	Position p = {player.pos[0], player.pos[2]};
	tween_set_to_position(player.movement_tween, p, final_position, &movement_callback);
}

void movement_callback(void *target_object, Position current_value) {
	player.pos[0] = current_value.x;
	player.pos[2] = current_value.y;
}

void movement_end_callback(void *target) {
	if (player.current_steps_taken >= player.next_combat_at) {
		current_state = GM_TO_COMBAT;
		screen_transition_y = SCREEN_HT - 1;
		reset_combat();

		return SCREEN_PLAY;
	}
}

void view_callback(void *target_object, float current_value) {
	player.angle = current_value;
	get_forward_vector_from_angle(player.angle, &player.forward[0], &player.forward[2]);
}
