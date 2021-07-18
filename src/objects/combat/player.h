#pragma once

#include "../billboards.h"

#define DRAW_CLASS_ANIM(x, z, pov_x, pov_z, animation, cur_frame, max_frames, class, gender)       \
	gDPPipeSync((*glistp)++);                                                                      \
	DRAW_BILLBOARD_MULTI(x, z, 4, 4, player_dl, player_dl2, player_dl3, player_dl4,                \
						 spr_##class##_##gender##_##animation[cur_frame % max_frames], pov_x,      \
						 pov_z);

#define DRAW_CLASS(class, gender, x, z, pov_x, pov_z, anim_frame, frame_count, animation)          \
	switch (class) {                                                                               \
		case PC_Warrior:                                                                           \
			if (gender == G_Male) {                                                                \
				DRAW_CLASS_ANIM(x, z, pov_x, pov_z, animation, anim_frame, frame_count, warrior,   \
								male);                                                             \
			} else {                                                                               \
				DRAW_CLASS_ANIM(x, z, pov_x, pov_z, animation, anim_frame, frame_count, warrior,   \
								female);                                                           \
			}                                                                                      \
			break;                                                                                 \
		case PC_Wizard:                                                                            \
			if (gender == G_Male) {                                                                \
				DRAW_CLASS_ANIM(x, z, pov_x, pov_z, animation, anim_frame, frame_count, wizard,    \
								male);                                                             \
			} else {                                                                               \
				DRAW_CLASS_ANIM(x, z, pov_x, pov_z, animation, anim_frame, frame_count, wizard,    \
								female);                                                           \
			}                                                                                      \
			break;                                                                                 \
		case PC_Cleric:                                                                            \
			if (gender == G_Male) {                                                                \
				DRAW_CLASS_ANIM(x, z, pov_x, pov_z, animation, anim_frame, frame_count, cleric,    \
								male);                                                             \
			} else {                                                                               \
				DRAW_CLASS_ANIM(x, z, pov_x, pov_z, animation, anim_frame, frame_count, cleric,    \
								female);                                                           \
			}                                                                                      \
			break;                                                                                 \
		case PC_Thief:                                                                             \
			if (gender == G_Male) {                                                                \
				DRAW_CLASS_ANIM(x, z, pov_x, pov_z, animation, anim_frame, frame_count, thief,     \
								male);                                                             \
			} else {                                                                               \
				DRAW_CLASS_ANIM(x, z, pov_x, pov_z, animation, anim_frame, frame_count, thief,     \
								female);                                                           \
			}                                                                                      \
			break;                                                                                 \
		default:                                                                                   \
			break;                                                                                 \
	}
