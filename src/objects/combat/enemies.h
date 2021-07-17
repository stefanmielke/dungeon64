#pragma once

#include "../billboards.h"

#define DRAW_ENEMY_32(x, z, pov_x, pov_z, frame, frame_count, sprite)                              \
	gDPPipeSync((*glistp)++);                                                                      \
	DRAW_BILLBOARD_SINGLE(x, z, 4, 4, enemy_32_dl, sprite[frame % frame_count], pov_x, pov_z);

#define DRAW_ENEMY_64(x, z, pov_x, pov_z, frame, frame_count, sprite)                              \
	gDPPipeSync((*glistp)++);                                                                      \
	DRAW_BILLBOARD_MULTI(x, z, 5, 5, enemy_64_dl, enemy_64_dl2, enemy_64_dl3, enemy_64_dl4,        \
						 sprite[frame % frame_count], pov_x, pov_z);

#define DRAW_ENEMY(type, x, z, pov_x, pov_z, frame)                                                \
	switch (type) {                                                                                \
		case ET_GoblinBoss:                                                                        \
			DRAW_ENEMY_64(x, z, pov_x, pov_z, frame, 1, spr_goblin_boss_idle);                     \
			break;                                                                                 \
		case ET_GoblinMinion1:                                                                     \
			DRAW_ENEMY_32(x, z, pov_x, pov_z, frame, 1, spr_goblin_minion1_idle);                  \
			break;                                                                                 \
		case ET_GoblinMinion2:                                                                     \
			DRAW_ENEMY_32(x, z, pov_x, pov_z, frame, 1, spr_goblin_minion2_idle);                  \
			break;                                                                                 \
		default:                                                                                   \
			break;                                                                                 \
	}
