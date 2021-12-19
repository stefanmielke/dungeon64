#pragma once

#define DRAW_BILLBOARD_MULTI(x, z, width, height, dl, dl2, dl3, dl4, texture, pov_x, pov_z)        \
	{                                                                                              \
		gDPLoadMultiTile((*glistp)++, texture, 0, G_TX_RENDERTILE, G_IM_FMT_RGBA, G_IM_SIZ_16b,    \
						 64, 64, 32, 32, 63, 63, 0, G_TX_MIRROR, G_TX_MIRROR, 5, 5, G_TX_NOLOD,    \
						 G_TX_NOLOD);                                                              \
		float angle = (atan2f(z - pov_z, x - pov_x) + RAD_90) * RAD_MULT;                          \
		guRotate(&(dynamic.billboard_rotation[billboard_count]), -angle, 0, 1, 0);                 \
		guTranslate(&(dynamic.object_position[obj_count]), x, 0, z);                               \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),           \
				  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                    \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.billboard_rotation[billboard_count])),  \
				  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                     \
		obj_count++;                                                                               \
		gSPDisplayList((*glistp)++, dl4);                                                          \
                                                                                                   \
		gDPLoadMultiTile((*glistp)++, texture, 0, G_TX_RENDERTILE, G_IM_FMT_RGBA, G_IM_SIZ_16b,    \
						 64, 64, 32, 0, 63, 31, 0, G_TX_MIRROR, G_TX_MIRROR, 5, 5, G_TX_NOLOD,     \
						 G_TX_NOLOD);                                                              \
		guTranslate(&(dynamic.object_position[obj_count]), x, 0, z);                               \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),           \
				  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                    \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.billboard_rotation[billboard_count])),  \
				  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                     \
		obj_count++;                                                                               \
		guTranslate(&(dynamic.object_position[obj_count]), 0, height, 0);                          \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),           \
				  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                     \
		obj_count++;                                                                               \
		gSPDisplayList((*glistp)++, dl2);                                                          \
                                                                                                   \
		gDPLoadMultiTile((*glistp)++, texture, 0, G_TX_RENDERTILE, G_IM_FMT_RGBA, G_IM_SIZ_16b,    \
						 64, 64, 0, 32, 31, 63, 0, G_TX_MIRROR, G_TX_MIRROR, 5, 5, G_TX_NOLOD,     \
						 G_TX_NOLOD);                                                              \
		guTranslate(&(dynamic.object_position[obj_count]), x, 0, z);                               \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),           \
				  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                    \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.billboard_rotation[billboard_count])),  \
				  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                     \
		obj_count++;                                                                               \
		guTranslate(&(dynamic.object_position[obj_count]), -width, 0, 0);                          \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),           \
				  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                     \
		obj_count++;                                                                               \
		gSPDisplayList((*glistp)++, dl3);                                                          \
                                                                                                   \
		gDPLoadMultiTile((*glistp)++, texture, 0, G_TX_RENDERTILE, G_IM_FMT_RGBA, G_IM_SIZ_16b,    \
						 64, 64, 0, 0, 31, 31, 0, G_TX_MIRROR, G_TX_MIRROR, 5, 5, G_TX_NOLOD,      \
						 G_TX_NOLOD);                                                              \
		guTranslate(&(dynamic.object_position[obj_count]), x, 0, z);                               \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),           \
				  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                    \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.billboard_rotation[billboard_count])),  \
				  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                     \
		obj_count++;                                                                               \
		guTranslate(&(dynamic.object_position[obj_count]), -width, height, 0);                     \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),           \
				  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                     \
		obj_count++;                                                                               \
		gSPDisplayList((*glistp)++, dl);                                                           \
                                                                                                   \
		billboard_count++;                                                                         \
	}

#define DRAW_BILLBOARD_SINGLE(x, z, width, height, dl, texture, pov_x, pov_z)                      \
	{                                                                                              \
		gDPLoadTextureBlock((*glistp)++, texture, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,          \
							G_TX_MIRROR, G_TX_MIRROR, 5, 5, G_TX_NOLOD, G_TX_NOLOD);               \
		float angle = (atan2f(z - pov_z, x - pov_x) + RAD_90) * RAD_MULT;                          \
		guRotate(&(dynamic.billboard_rotation[billboard_count]), -angle, 0, 1, 0);                 \
		guTranslate(&(dynamic.object_position[obj_count]), x, 0, z);                               \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),           \
				  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                    \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.billboard_rotation[billboard_count])),  \
				  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                     \
		obj_count++;                                                                               \
		gSPDisplayList((*glistp)++, dl);                                                           \
		billboard_count++;                                                                         \
	}

#define DRAW_BILLBOARD_Z(x_pos, y, z_pos)                                                          \
	{                                                                                              \
		float angle = (atan2f(z_pos - (player->pos.z - player->forward.z * 5),                     \
							  x_pos - (player->pos.x - player->forward.x * 5)) +                   \
					   RAD_90) *                                                                   \
					  RAD_MULT;                                                                    \
		guRotate(&(dynamic.billboard_rotation[billboard_count]), -angle, 0, 1, 0);                 \
		guTranslate(&(dynamic.object_position[obj_count]), x_pos, y, z_pos);                       \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),           \
				  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                    \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.billboard_rotation[billboard_count])),  \
				  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                     \
		billboard_count++;                                                                         \
		obj_count++;                                                                               \
	}

#define DRAW_BILLBOARD(x, z) DRAW_BILLBOARD_Z(x, 0, z)

#define DRAW_PLANT(x, z)                                                                           \
	gDPPipeSync((*glistp)++);                                                                      \
	gDPLoadTextureBlock((*glistp)++, spr_plant, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,            \
						G_TX_MIRROR, G_TX_MIRROR, 5, 5, G_TX_NOLOD, G_TX_NOLOD);                   \
	DRAW_BILLBOARD(x, z);                                                                          \
	gSPDisplayList((*glistp)++, plant_dl);
