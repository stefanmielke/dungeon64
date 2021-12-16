#include "map_helper.h"

#include <nustd/math.h>

#include "walls.h"
#include "../math.h"
#include "../static.h"
#include "../data/texture.h"
#include "../objects/billboards.h"
#include "../maps/maps.gen.h"

Vec3 map_get_start_position(Map *map, u32 *tile_position, float *angle) {
	for (u8 i = 0; i < map->events.event_count; ++i) {
		if (map->events.data[i].type == MET_Spawn) {
			*tile_position = map->events.data[i].tile_position;
			*angle = map->events.data[i].args.spawn.angle;
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
	result.z = ((map_coord / width) * TILE_SIZE) + (TILE_SIZE / 2);
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

	const int view_distance = 4;

	int player_current_tile_x = player->current_tile % map->width;
	int player_current_tile_z = (int)floorf(player->current_tile / map->width);
	int start_x = player_current_tile_x - view_distance;
	int end_x = player_current_tile_x + view_distance;
	int start_z = player_current_tile_z - view_distance;
	int end_z = player_current_tile_z + view_distance;

	if (start_x < 0)
		start_x = 0;
	if (start_z < 0)
		start_z = 0;

	// ground
	gSPDisplayList((*glistp)++, ground_texture_setup_dl);
	gSPTexture((*glistp)++, 2048, 2048, 0, G_TX_RENDERTILE, G_ON);
	gDPLoadTextureBlock((*glistp)++, map->spr_ground, G_IM_FMT_RGBA, G_IM_SIZ_16b, 64, 32, 0,
						G_TX_WRAP, G_TX_WRAP, 6, 5, G_TX_NOLOD, G_TX_NOLOD);

	for (unsigned long i = 0; i < map->size; ++i) {
		if (map->tiles[i] >= TL_Ground_Start && map->tiles[i] <= TL_Objects_End) {
			u32 tile_x = i % map->width;
			u32 tile_z = (u32)floorf(i / map->width);

			u32 x = tile_x * TILE_SIZE;
			u32 z = tile_z * TILE_SIZE;

			if (tile_x < start_x || tile_x > end_x)
				continue;
			if (tile_z < start_z || tile_z > end_z)
				continue;

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
		u32 tile_x = i % map->width;
		u32 tile_z = (u32)floorf(i / map->width);

		u32 x = tile_x * TILE_SIZE;
		u32 z = tile_z * TILE_SIZE;

		if (tile_x < start_x || tile_x > end_x)
			continue;
		if (tile_z < start_z || tile_z > end_z)
			continue;

		if (map->tiles[i] >= TT_Wall_Full && map->tiles[i] <= TT_Wall_North && wall_type != 0) {
			gDPLoadTextureBlock((*glistp)++, map->spr_wall, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0,
								G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
			wall_type = 0;
		} else if (map->tiles[i] >= TT_Upstairs_East && map->tiles[i] <= TT_Upstairs_North &&
				   wall_type != 1) {
			gDPLoadTextureBlock((*glistp)++, map->spr_wall_upstairs, G_IM_FMT_RGBA, G_IM_SIZ_16b,
								32, 32, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
			wall_type = 1;
		} else if (map->tiles[i] >= TT_Downstairs_East && map->tiles[i] <= TT_Downstairs_North &&
				   wall_type != 2) {
			gDPLoadTextureBlock((*glistp)++, map->spr_wall_exit, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32,
								32, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
			wall_type = 2;
		}

		if (map->tiles[i] == TT_Wall_Full) {  // full wall
			DRAW_WALL_SQUARE(x, z);
		} else if (map->tiles[i] == TT_Wall_East || map->tiles[i] == TT_Upstairs_East ||
				   map->tiles[i] == TT_Downstairs_East) {  // east wall
			DRAW_WALL_EAST(x, z);
		} else if (map->tiles[i] == TT_Wall_South || map->tiles[i] == TT_Upstairs_South ||
				   map->tiles[i] == TT_Downstairs_South) {	// south wall
			DRAW_WALL_SOUTH(x, z);
		} else if (map->tiles[i] == TT_Wall_West || map->tiles[i] == TT_Upstairs_West ||
				   map->tiles[i] == TT_Downstairs_West) {  // west wall
			DRAW_WALL_WEST(x, z);
		} else if (map->tiles[i] == TT_Wall_North || map->tiles[i] == TT_Upstairs_North ||
				   map->tiles[i] == TT_Downstairs_North) {	// north wall
			DRAW_WALL_NORTH(x, z);
		}
	}

	// billboard setup
	// gSPDisplayList((*glistp)++, billboard_texture_setup_dl);
	// gSPTexture((*glistp)++, 2048, 2048, 0, G_TX_RENDERTILE, G_ON);

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
