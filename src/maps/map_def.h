#pragma once

#include <ultratypes.h>
#include "../../libs/ultra64-extensions/include/range.h"

typedef struct MapDef {
	u32 size;
	u32 width;
	RangeInt steps_to_combat;
	u8 map_data[];
} MapDef;