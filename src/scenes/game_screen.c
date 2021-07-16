#include "game_screen.h"

#include "scene_defs.h"

#include <os_host.h>
#include <nustd/math.h>

#include "../math.h"
#include "../data/texture.h"
#include "../fonts/font_ext.h"
#include "../objects/map_helper.h"
#include "../objects/player.h"
#include "../maps/maps.h"

#include "../../libs/ultra64-extensions/include/easing.h"
#include "../../libs/ultra64-extensions/include/mem_pool.h"
#include "../../libs/ultra64-extensions/include/tween.h"

Map current_map;
Player player;

// helper functions for movement
void set_angle(float angle_diff);
void move_to(s32 h_speed, s32 forward_speed);

// tween callbacks
void movement_callback(void *target_object, Position current_value);
void view_callback(void *target_object, float current_value);

void game_screen_create() {
	player.movement_tween = tween_init(&memory_pool);
	player.view_tween = tween_init(&memory_pool);

	current_map.tiles = map1_1;
	current_map.size = map1_1_size;
	current_map.width = map1_1_width;

	Vec3 player_start = map_get_start_position(&current_map, &player.current_tile);
	player.pos[0] = player_start.x;
	player.pos[1] = player_start.y;
	player.pos[2] = player_start.z;
	player.angle = 0;

	set_angle(0);
	move_to(0, 0);
}

short game_screen_tick() {
	gd.pad = ReadController(START_BUTTON);

	tween_tick(player.movement_tween);
	tween_tick(player.view_tween);

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

	return SCREEN_PLAY;
}

void game_screen_display() {
	// set up matrices
	guPerspectiveF(rd.allmat, &rd.perspnorm, 80.0, 320.0 / 240.0, 1.0, 1024.0, 1.0);
	guPerspective(&(rd.dynamicp->projection), &rd.perspnorm, 80.0, 320.0 / 240.0, 1.0, 1024.0, 1.0);

	Vec3f forward = {player.pos[0] + player.forward[0], player.pos[1] + 5.0,
					 player.pos[2] + player.forward[2]};
	guLookAtF(rd.m2, player.pos[0], forward[1], player.pos[2], forward[0], forward[1], forward[2],
			  0.0, 1.0, 0.0);
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
	sprintf(position, "Tile: %d Dir: %.2f, %.2f", player.current_tile, player.forward[0],
			player.forward[2]);
	SHOWFONT(&glistp, position, 20, 210);
	font_finish(&glistp);
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
		}
	} else {
		final_position.x = player.pos[0];
		final_position.y = player.pos[2];
	}

	tween_restart(player.movement_tween, &player, &easing_exponential_out, MOVEMENT_SPEED, NULL,
				  path_is_blocked, false);
	Position p = {player.pos[0], player.pos[2]};
	tween_set_to_position(player.movement_tween, p, final_position, &movement_callback);
}

void movement_callback(void *target_object, Position current_value) {
	player.pos[0] = current_value.x;
	player.pos[2] = current_value.y;
}

void view_callback(void *target_object, float current_value) {
	player.angle = current_value;
	get_forward_vector_from_angle(player.angle, &player.forward[0], &player.forward[2]);
}
