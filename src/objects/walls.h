#pragma once

#define DRAW_WALL_BASE(x, y, dl)                                                                     \
	gDPPipeSync((*glistp)++);                                                                      \
	guTranslate(&dynamicp->object_position[obj_count], x, 0, y);                                   \
	gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamicp->object_position[obj_count])),             \
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                        \
	gSPDisplayList((*glistp)++, dl);                                                   \
	obj_count++;

#define DRAW_WALL_SQUARE(x, y)                                                                     \
	DRAW_WALL_BASE(x, y, wall_square_dl)

#define DRAW_WALL_EAST(x, y)                                                                       \
	DRAW_WALL_BASE(x, y, wall_east_dl)
#define DRAW_WALL_SOUTH(x, y)                                                                      \
	DRAW_WALL_BASE(x, y, wall_south_dl)
#define DRAW_WALL_WEST(x, y)                                                                       \
	DRAW_WALL_BASE(x, y, wall_west_dl)
#define DRAW_WALL_NORTH(x, y)                                                                      \
	DRAW_WALL_BASE(x, y, wall_north_dl)

#define DRAW_WALL_SOUTH_EAST(x, y)                                                                      \
	DRAW_WALL_BASE(x, y, wall_south_east_dl)
#define DRAW_WALL_SOUTH_WEST(x, y)                                                                      \
	DRAW_WALL_BASE(x, y, wall_south_west_dl)
#define DRAW_WALL_SOUTH_NORTH(x, y)                                                                      \
	DRAW_WALL_BASE(x, y, wall_south_north_dl)
#define DRAW_WALL_NORTH_EAST(x, y)                                                                      \
	DRAW_WALL_BASE(x, y, wall_north_east_dl)
#define DRAW_WALL_NORTH_WEST(x, y)                                                                      \
	DRAW_WALL_BASE(x, y, wall_north_west_dl)
#define DRAW_WALL_EAST_WEST(x, y)                                                                      \
	DRAW_WALL_BASE(x, y, wall_east_west_dl)

#define DRAW_WALL_NO_WEST(x, y)                                                                      \
	DRAW_WALL_BASE(x, y, wall_no_west_dl)
#define DRAW_WALL_NO_EAST(x, y)                                                                      \
	DRAW_WALL_BASE(x, y, wall_no_east_dl)
#define DRAW_WALL_NO_NORTH(x, y)                                                                      \
	DRAW_WALL_BASE(x, y, wall_no_north_dl)
#define DRAW_WALL_NO_SOUTH(x, y)                                                                      \
	DRAW_WALL_BASE(x, y, wall_no_south_dl)
