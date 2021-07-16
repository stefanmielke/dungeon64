#pragma once

#include "../billboards.h"

#define DRAW_BLUE_DRAGON(x, z)                                                                     \
	gDPPipeSync((*glistp)++);                                                                      \
	DRAW_BILLBOARD_MULTI(x, z, 5, 5, blue_dragon_dl, spr_blue_dragon_idle1);
