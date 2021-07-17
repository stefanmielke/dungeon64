#pragma once

#include "../billboards.h"

#define DRAW_WARRIOR(x, z, pov_x, pov_z, anim_frame)                                               \
	gDPPipeSync((*glistp)++);                                                                      \
	DRAW_BILLBOARD_MULTI(x, z, 4, 4, player_dl, player_dl2, player_dl3, player_dl4,                \
						 spr_warrior_back_idle[anim_frame % 4], pov_x, pov_z);

#define DRAW_CLASS(class, x, z, pov_x, pov_z, anim_frame)                                          \
	switch (class) {                                                                               \
		case PC_Warrior:                                                                           \
			DRAW_WARRIOR(x, z, pov_x, pov_z, anim_frame);                                          \
			break;                                                                                 \
		default:                                                                                   \
			break;                                                                                 \
	}
