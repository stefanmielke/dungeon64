#pragma once

#include <ultratypes.h>
#include "../../libs/ultra64-extensions/include/range.h"

#include "../math.h"

#define LOOK_NORTH (RAD_90 * 3)
#define LOOK_SOUTH (RAD_90)
#define LOOK_EAST (0)
#define LOOK_WEST (RAD_90 * 2)

typedef enum MapEventType {
	MET_None,	 // No event, shouldn't be used
	MET_Spawn,	 // Where player spawns (if first level)
	MET_Exit,	 // Player exit the map (should be a wall)
	MET_Stairs,	 // Player warp to another map (should be a wall).
	MET_Warp,	 // Player warps to another position inside the map.
	MET_MAX,
} MapEventType;

typedef struct MapEventSpawnArgs {
	float angle;
} MapEventSpawnArgs;
typedef struct MapEventStairsArgs {
	u16 map_id;
	u32 tile_to_spawn;
	float angle;
} MapEventStairsArgs;
typedef struct MapEventWarpArgs {
	u32 tile_to_warp;
	float angle;
} MapEventWarpArgs;

union MapEventArgs {
	MapEventSpawnArgs spawn;
	MapEventStairsArgs stairs;
	MapEventWarpArgs warp;
};

typedef struct MapEvent {
	MapEventType type;
	u32 tile_position;
	union MapEventArgs args;
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
	u16 *spr_ground;
	u16 *spr_wall;
	u16 *spr_wall_exit;
	u16 *spr_wall_upstairs;
	u8 tiles[];
} MapDef;