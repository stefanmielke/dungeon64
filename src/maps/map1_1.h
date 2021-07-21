#pragma once

#include "map_def.h"

MapDef map_1_1 = {
	.size = 9 * 9,
	.width = 9,
	.steps_to_combat = {.start = 10, .end = 20},
	.events =
		{
			.event_count = 3,
			.data =
				{
					[0].type = MET_Spawn,
					[0].tile_position = 64,

					[1].type = MET_Exit,
					[1].tile_position = 73,

					[2].type = MET_Stairs,
					[2].tile_position = 75,
					[2].arg1 = 2,	// to map 1_2
					[2].arg2 = 66,	// position
				},
		},
	.tiles =
		{
			0, 3,	3,	 3,	  3,   3,	3,	 3,	  0, /**/
			2, 100, 100, 100, 100, 100, 100, 100, 4, /**/
			0, 5,	5,	 1,	  100, 1,	1,	 100, 4, /**/
			0, 0,	2,	 100, 100, 100, 1,	 100, 4, /**/
			0, 0,	2,	 100, 100, 100, 1,	 100, 4, /**/
			0, 0,	2,	 100, 100, 100, 1,	 100, 4, /**/
			0, 3,	3,	 1,	  100, 1,	1,	 100, 4, /**/
			2, 100, 100, 100, 100, 100, 100, 100, 4, /**/
			0, 9,	5,	 13,  5,   5,	5,	 5,	  0, /**/
		},
};
