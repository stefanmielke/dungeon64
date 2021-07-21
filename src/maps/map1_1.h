#pragma once

#include "map_def.h"

// unsigned long map1_1_size = 9 * 9;
// unsigned long map1_1_width = 9;
// unsigned short map1_1[] = {
// 	0, 3,	3,	 3,	  3,   3,	3,	 3,	  0, /**/
// 	2, 100, 100, 100, 100, 100, 100, 100, 4, /**/
// 	2, 100, 1,	 1,	  100, 1,	1,	 100, 4, /**/
// 	2, 100, 1,	 100, 100, 100, 1,	 100, 4, /**/
// 	2, 100, 1,	 100, 100, 100, 1,	 100, 4, /**/
// 	2, 100, 1,	 100, 100, 100, 1,	 100, 4, /**/
// 	2, 100, 1,	 1,	  100, 1,	1,	 100, 4, /**/
// 	2, 255, 100, 100, 100, 100, 100, 100, 4, /**/
// 	0, 9,	5,	 5,	  5,   5,	5,	 5,	  0, /**/
// };
MapDef map_1_1 = {
	.size = 9 * 9,
	.width = 9,
	.steps_to_combat = {.start = 10, .end = 20},
	.tiles =
		{
			0, 3,	3,	 3,	  3,   3,	3,	 3,	  0, /**/
			2, 100, 100, 100, 100, 100, 100, 100, 4, /**/
			2, 100, 1,	 1,	  100, 1,	1,	 100, 4, /**/
			2, 100, 1,	 100, 100, 100, 1,	 100, 4, /**/
			2, 100, 1,	 100, 100, 100, 1,	 100, 4, /**/
			2, 100, 1,	 100, 100, 100, 1,	 100, 4, /**/
			2, 100, 1,	 1,	  100, 1,	1,	 100, 4, /**/
			2, 255, 100, 100, 100, 100, 100, 100, 4, /**/
			0, 9,	5,	 5,	  5,   5,	5,	 5,	  0, /**/
		},
};