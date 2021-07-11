#include "map_helper.h"

#include "walls.h"
#include "../static.h"
#include "../data/texture.h"

Vec3 map_get_start_position(Map *map) {
	for (unsigned long i = 0; i < map->size; ++i) {
		if (map->data[i] == 255) {
			return map_get_position_from_map_coord(i, map->size, map->width);
		}
	}

	Vec3 result = {0, 0, 0};
	return result;
}

Vec3 map_get_position_from_map_coord(unsigned long map_coord, unsigned long size,
									 unsigned long width) {
	Vec3 result;
	result.y = 0;
	result.x = ((map_coord % width) * TILE_SIZE) + (TILE_SIZE / 2);
	result.z = ((map_coord / (size / width)) * TILE_SIZE) + (TILE_SIZE / 2);
	return result;
}

void map_render(Map *map, Gfx **glistp, Dynamic *dynamicp) {
	int obj_count;		 /* count of used objects on current frame */
	int billboard_count; /* count of used billboards on current frame */

	// ground
	gSPDisplayList((*glistp)++, ground_texture_setup_dl);
	gSPTexture((*glistp)++, 1024 * 10, 1024 * 10, 0, G_TX_RENDERTILE, G_ON);
	gDPLoadTextureBlock((*glistp)++, spr_ground, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_WRAP,
						G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

	for (unsigned long i = 0; i < map->size; ++i) {
		if (map->data[i] == 0 || map->data[i] == 255) {
			u32 x = ((i % map->width) * TILE_SIZE);
			u32 z = ((i / (map->size / map->width)) * TILE_SIZE);

			// ground
			gDPPipeSync((*glistp)++);
			guTranslate(&dynamicp->object_position[obj_count], x, 0, z);
			gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamicp->object_position[obj_count])),
					  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
			gSPDisplayList((*glistp)++, ground_dl);
			obj_count++;

			// ceiling (uses the same texture for now)
			gDPPipeSync((*glistp)++);
			guTranslate(&dynamicp->object_position[obj_count], x, 10, z);
			gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamicp->object_position[obj_count])),
					  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
			gSPDisplayList((*glistp)++, ceiling_dl);
			obj_count++;
		}
	}

	// walls
	gSPTexture((*glistp)++, 1024 * 10, 1024 * 10, 0, G_TX_RENDERTILE, G_ON);
	gDPLoadTextureBlock((*glistp)++, spr_wall, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_WRAP,
						G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
	for (unsigned long i = 0; i < map->size; ++i) {
		if (map->data[i] == 1) {
			u32 x = ((i % map->width) * TILE_SIZE);
			u32 z = ((i / (map->size / map->width)) * TILE_SIZE);
			DRAW_WALL_SQUARE(x, z);
		}
	}
}
