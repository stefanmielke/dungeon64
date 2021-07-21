#pragma once

#include <ultratypes.h>
#include "../../libs/ultra64-extensions/include/range.h"

typedef enum MapEventType {
	MET_None,	 // No event, shouldn't be used
	MET_Spawn,	 // Where player spawns (if first level)
	MET_Exit,	 // Player exit the map (should be a wall)
	MET_Stairs,	 // Player moves to another map (should be a wall). Args: [map_id, tile_to_spawn]
	MET_Warp,	 // Player warps to another position inside the map. Args [tile_to_warp]
	MET_MAX,
} MapEventType;

typedef struct MapEvent {
	MapEventType type;
	u32 tile_position;
	u32 arg1;
	u32 arg2;
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