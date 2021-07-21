#pragma once

#include "map_def.h"

MapDef map_1_2 = {
	.size = 9 * 9,
	.width = 9,
	.steps_to_combat = {.start = 10, .end = 20},
	.events =
		{
			.event_count = 1,
			.data =
				{
					[0].type = MET_Stairs,
					[0].tile_position = 75,
					[0].args.stairs.map_id = 1,	 // to map 1_1
					[0].args.stairs.tile_to_spawn = 66,
					[0].args.stairs.angle = LOOK_NORTH,
				},
		},
	.tiles =
		{
			0, 3,	3,	 3,	  3,   3,	3,	 3,	  0, /**/
			2, 100, 100, 100, 100, 100, 100, 100, 4, /**/
			2, 100, 1,	 1,	  100, 1,	1,	 100, 4, /**/
			2, 100, 1,	 100, 100, 100, 1,	 100, 4, /**/
			2, 100, 1,	 100, 100, 100, 1,	 100, 4, /**/
			2, 100, 1,	 100, 100, 100, 1,	 100, 4, /**/
			2, 100, 1,	 1,	  100, 1,	1,	 100, 4, /**/
			2, 100, 100, 100, 100, 100, 100, 100, 4, /**/
			0, 5,	5,	 9,	  5,   5,	5,	 5,	  0, /**/
		},
};