#pragma once

#define DRAW_BILLBOARD_MULTI(x, z, width, height, dl, texture)                                     \
	{                                                                                              \
		gDPLoadMultiTile((*glistp)++, texture, 0, G_TX_RENDERTILE, G_IM_FMT_RGBA, G_IM_SIZ_16b,    \
						 64, 64, 32, 32, 63, 63, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD,        \
						 G_TX_NOLOD);                                                              \
		float angle = (atan2f(z - player->pos[2], x - player->pos[0]) + RAD_90) * RAD_MULT;        \
		guRotate(&(dynamic.billboard_rotation[billboard_count]), -angle, 0, 1, 0);                 \
		guTranslate(&(dynamic.object_position[obj_count]), x, 0, z);                               \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.object_position[obj_count])),           \
				  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);                                    \
		gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamic.billboard_rotation[billboard_count])),  \
				  G_MTX_MODELVIEW | G_MTX_MUL | G_MTX_NOPUSH);                                     \
		obj_count++;                                                                               \
		gSPDisplayList((*glistp)++, dl);                                                           \
                                                                                                   \
		gDPLoadMultiTile((*glistp)++, texture, 0, G_TX_RENDERTILE, G_IM_FMT_RGBA, G_IM_SIZ_16b,    \
						 64, 64, 32, 0, 63, 31, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD,         \
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
		gSPDisplayList((*glistp)++, dl);                                                           \
                                                                                                   \
		gDPLoadMultiTile((*glistp)++, texture, 0, G_TX_RENDERTILE, G_IM_FMT_RGBA, G_IM_SIZ_16b,    \
						 64, 64, 0, 32, 31, 63, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD,         \
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
		gSPDisplayList((*glistp)++, dl);                                                           \
                                                                                                   \
		gDPLoadMultiTile((*glistp)++, texture, 0, G_TX_RENDERTILE, G_IM_FMT_RGBA, G_IM_SIZ_16b,    \
						 64, 64, 0, 0, 31, 31, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD,          \
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

#define DRAW_BILLBOARD_Z(x, y, z)                                                                  \
	{                                                                                              \
		float angle = (atan2f(y - player->pos[2], x - player->pos[0]) + RAD_90) * RAD_MULT;        \
		guRotate(&(dynamic.billboard_rotation[billboard_count]), -angle, 0, 1, 0);                 \
		guTranslate(&(dynamic.object_position[obj_count]), x, y, z);                               \
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
	DRAW_BILLBOARD(x, z);                                                                          \
	gSPDisplayList((*glistp)++, plant_dl);
