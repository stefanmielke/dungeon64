#pragma once

#include "../billboards.h"

#define DRAW_BLUE_DRAGON(x, z, pov_x, pov_z, frame)                                                \
	gDPPipeSync((*glistp)++);                                                                      \
	DRAW_BILLBOARD_MULTI(x, z, 5, 5, blue_dragon_dl, blue_dragon_dl2, blue_dragon_dl3,             \
						 blue_dragon_dl4, spr_blue_dragon_idle[frame % 4], pov_x, pov_z);
