#pragma once

#include <ultra64.h>
#include "../libs/ultra64-extensions/include/range.h"

#include "maps/map_def.h"

#define TILE_SIZE 10
#define TILE_VIEW_DISTANCE 3
#define VIEW_DISTANCE ((TILE_VIEW_DISTANCE + 2) * 10)

typedef struct Map {
	u8 *tiles;
	u32 size;
	u32 width;
	RangeInt steps_to_combat;
	bool has_random_encounters;
	MapEvents events;

	u16 *spr_ground;
	u16 *spr_wall;
	u16 *spr_wall_exit;
	u16 *spr_wall_upstairs;
} Map;
