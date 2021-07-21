#include "map_helper.h"

#include <nustd/math.h>

#include "walls.h"
#include "../math.h"
#include "../static.h"
#include "../data/texture.h"
#include "../objects/billboards.h"
#include "../maps/maps.h"

Vec3 map_get_start_position(Map *map, u32 *tile_position) {
	for (u8 i = 0; i < map->events.event_count; ++i) {
		if (map->events.data[i].type == MET_Spawn) {
			*tile_position = map->events.data[i].tile_position;
			return map_get_position_from_map_coord(map->events.data[i].tile_position, map->size,
												   map->width);
		}
	}

	Vec3 result = {0, 0, 0};
	return result;
}

Vec3 map_get_position_from_map_coord(u32 map_coord, u32 size, u32 width) {
	Vec3 result;
	result.y = 0;
	result.x = ((map_coord % width) * TILE_SIZE) + (TILE_SIZE / 2);
	result.z = ((map_coord / (size / width)) * TILE_SIZE) + (TILE_SIZE / 2);
	return result;
}

MapEvent *map_get_event_on_tile(Map *map, u32 tile_position) {
	for (u8 i = 0; i < map->events.event_count; ++i) {
		if (map->events.data[i].tile_position == tile_position)
			return &map->events.data[i];
	}

	return NULL;
}

void map_render(Map *map, Gfx **glistp, Dynamic *dynamicp, Player *player) {
	int obj_count = 0; /* count of used objects on current frame */
	// int billboard_count = 0; /* count of used billboards on current frame */

	// ground
	gSPDisplayList((*glistp)++, ground_texture_setup_dl);
	gSPTexture((*glistp)++, 2048, 2048, 0, G_TX_RENDERTILE, G_ON);
	gDPLoadTextureBlock((*glistp)++, spr_ground, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_WRAP,
						G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

	for (unsigned long i = 0; i < map->size; ++i) {
		if (map->tiles[i] >= TL_Ground_Start && map->tiles[i] <= TL_Objects_End) {
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
	gSPTexture((*glistp)++, 2048, 2048, 0, G_TX_RENDERTILE, G_ON);
	int wall_type = -1;
	for (unsigned long i = 0; i < map->size; ++i) {
		if (map->tiles[i] >= TT_Wall_Full && map->tiles[i] <= TT_Wall_North && wall_type != 0) {
			gDPLoadTextureBlock((*glistp)++, spr_wall, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
								G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
			wall_type = 0;
		} else if (map->tiles[i] >= TT_Upstairs_East && map->tiles[i] <= TT_Upstairs_North &&
				   wall_type != 1) {
			gDPLoadTextureBlock((*glistp)++, spr_wall_upstairs, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32,
								0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
			wall_type = 1;
		} else if (map->tiles[i] >= TT_Downstairs_East && map->tiles[i] <= TT_Downstairs_North &&
				   wall_type != 2) {
			gDPLoadTextureBlock((*glistp)++, spr_wall_exit, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
								G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
			wall_type = 2;
		}

		if (map->tiles[i] == TT_Wall_Full) {  // full wall
			u32 x = ((i % map->width) * TILE_SIZE);
			u32 z = ((i / (map->size / map->width)) * TILE_SIZE);
			DRAW_WALL_SQUARE(x, z);
		} else if (map->tiles[i] == TT_Wall_East || map->tiles[i] == TT_Upstairs_East ||
				   map->tiles[i] == TT_Downstairs_East) {  // east wall
			u32 x = ((i % map->width) * TILE_SIZE);
			u32 z = ((i / (map->size / map->width)) * TILE_SIZE);
			DRAW_WALL_EAST(x, z);
		} else if (map->tiles[i] == TT_Wall_South || map->tiles[i] == TT_Upstairs_South ||
				   map->tiles[i] == TT_Downstairs_South) {	// south wall
			u32 x = ((i % map->width) * TILE_SIZE);
			u32 z = ((i / (map->size / map->width)) * TILE_SIZE);
			DRAW_WALL_SOUTH(x, z);
		} else if (map->tiles[i] == TT_Wall_West || map->tiles[i] == TT_Upstairs_West ||
				   map->tiles[i] == TT_Downstairs_West) {  // west wall
			u32 x = ((i % map->width) * TILE_SIZE);
			u32 z = ((i / (map->size / map->width)) * TILE_SIZE);
			DRAW_WALL_WEST(x, z);
		} else if (map->tiles[i] == TT_Wall_North || map->tiles[i] == TT_Upstairs_North ||
				   map->tiles[i] == TT_Downstairs_North) {	// north wall
			u32 x = ((i % map->width) * TILE_SIZE);
			u32 z = ((i / (map->size / map->width)) * TILE_SIZE);
			DRAW_WALL_NORTH(x, z);
		}
	}

	// billboard setup
	gSPDisplayList((*glistp)++, billboard_texture_setup_dl);
	gSPTexture((*glistp)++, 2048, 2048, 0, G_TX_RENDERTILE, G_ON);

	// DRAW_PLANT(10, 10);
}

bool map_is_tile_blocked(Map *map, u32 tile) {
	return map->tiles[tile] <= TL_Wall_End;
}

bool map_is_position_blocked(Map *map, Position position) {
	u32 norm_x = (((u32)position.x - (TILE_SIZE / 2)) / TILE_SIZE);
	u32 norm_y = (((u32)position.y - (TILE_SIZE / 2)) / TILE_SIZE);
	u32 tile = norm_x + (norm_y * map->width);

	return map_is_tile_blocked(map, tile);
}

MapDef *map_get_def_by_id(u16 map_id) {
	switch (map_id) {
		case 1:
			return &map_1_1;
		case 2:
			return &map_1_2;
		default:
			return NULL;
	}
}
