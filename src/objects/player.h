#pragma once

#include "../types.h"

typedef struct {
	Vec3f pos;
	float view_speed;
	float move_forward;
	float move_lateral;
	Vec3f forward;
	float angle;
	u32 current_tile;
} Player;
