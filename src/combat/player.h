#pragma once

#include "party.h"
#include "../types.h"
#include "../../libs/ultra64-extensions/include/tween.h"

typedef struct Player {
	Vec3f pos;
	float view_speed;
	float move_forward;
	float move_lateral;
	Vec3f forward;
	float angle;
	u32 current_tile;

	u32 current_steps_taken;
	u32 next_combat_at;

	Tween *movement_tween;
	Tween *view_tween;

	Party party;
} Player;

void player_init(Player *player, Vec3 position);