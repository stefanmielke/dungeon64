#pragma once

#include <stdbool.h>
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
	bool has_random_encounters;
	MapEvents events;

	u16 *spr_ground;
	u16 *spr_wall;
	u16 *spr_wall_exit;
	u16 *spr_wall_upstairs;
	u8 tiles[];
} MapDef;

typedef enum {
	TT_Empty_Space = 0,
	// walls
	TT_Wall_Full = 1,
	TT_Wall_East = 2,
	TT_Wall_South,
	TT_Wall_West,
	TT_Wall_North,
	TT_Wall_South_East = 6,
	TT_Wall_South_West,
	TT_Wall_South_North,
	TT_Wall_North_East,
	TT_Wall_North_West,
	TT_Wall_East_West,
	TT_Wall_No_West = 12,
	TT_Wall_No_East,
	TT_Wall_No_North,
	TT_Wall_No_South,

	TT_Upstairs_East = 20,
	TT_Upstairs_South,
	TT_Upstairs_West,
	TT_Upstairs_North,

	TT_Downstairs_East = 30,
	TT_Downstairs_South,
	TT_Downstairs_West,
	TT_Downstairs_North,

	// grounds
	TT_Ground_1 = 100,
	TT_Ground_2,
	TT_Ground_3,
	TT_Ground_4,
	TT_Ground_5,
	TT_Ground_6,

	// objects
	TT_StartPos = 255,
} TileTypes;

typedef enum {
	TL_Empty_Space = 0,

	TL_Wall_Start = 1,
	TL_Wall_End = TT_Ground_1 - 1,

	TL_Ground_Start = TT_Ground_1,
	TL_Ground_End = TT_StartPos - 1,

	TL_Objects_Start = TT_StartPos,
	TL_Objects_End = 65535,
} TileLimits;
