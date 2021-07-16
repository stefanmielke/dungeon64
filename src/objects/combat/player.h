#pragma once

#include "../billboards.h"

#define DRAW_WARRIOR(x, z)                                                                         \
	gDPPipeSync((*glistp)++);                                                                      \
	DRAW_BILLBOARD_MULTI(x, z, 4, 4, player_dl, spr_warrior_back_idle1);
