#pragma once

#include <ultra64.h>

#include "../libs/ultra64-extensions/include/range.h"

#define TILE_SIZE 10

typedef struct Map {
	u16 *tiles;
	u32 size;
	u32 width;
	RangeInt steps_to_combat;
} Map;
