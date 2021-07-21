#pragma once

#include <ultra64.h>
#include "../libs/ultra64-extensions/include/range.h"

#include "maps/map_def.h"

#define TILE_SIZE 10

typedef struct Map {
	u8 *tiles;
	u32 size;
	u32 width;
	RangeInt steps_to_combat;
	MapEvents events;
} Map;
