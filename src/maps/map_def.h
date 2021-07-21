#pragma once

#include <ultratypes.h>
#include "../../libs/ultra64-extensions/include/range.h"

typedef enum MapEventType {
	MET_None,	 // No event, shouldn't be used
	MET_Spawn,	 // Where player spawns (if first level)
	MET_Exit,	 // Where player exit the map (should be a wall)
	MET_Stairs,	 // Where a player moves to another dungeon map (should be a wall)
	MET_Warp,	 // Where a player warps to another position inside the map
} MapEventType;

typedef struct MapEvent {
	MapEventType type;
	u32 tile_position;
} MapEvent;

#define MAX_MAP_EVENTS 20
typedef struct MapEvents {
	u8 event_count;
	MapEvent data[MAX_MAP_EVENTS];
} MapEvents;

typedef struct MapDef {
	u32 size;
	u32 width;
	RangeInt steps_to_combat;
	MapEvents events;
	u8 tiles[];
} MapDef;