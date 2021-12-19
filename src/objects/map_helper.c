#include "map_helper.h"

#include <nustd/math.h>

#include "walls.h"
#include "../boot.h"
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

	int player_current_tile_x = player->current_tile % map->width;
	int player_current_tile_z = (int)floorf(player->current_tile / map->width);
	int start_x = player_current_tile_x - TILE_VIEW_DISTANCE;
	int end_x = player_current_tile_x + TILE_VIEW_DISTANCE;
	int start_z = player_current_tile_z - TILE_VIEW_DISTANCE;
	int end_z = player_current_tile_z + TILE_VIEW_DISTANCE;

	int map_height = map->size / map->width;
	if (start_x < 0)
		start_x = 0;
	if (start_z < 0)
		start_z = 0;
	if (end_x > map->width)
		end_x = map->width;
	if (end_z > map_height)
		end_z = map_height;

	// ground
	gSPDisplayList((*glistp)++, ground_texture_setup_dl);
	gSPTexture((*glistp)++, 2048, 2048, 0, G_TX_RENDERTILE, G_ON);
	gDPLoadTextureBlock((*glistp)++, map->spr_ground, G_IM_FMT_RGBA, G_IM_SIZ_16b, 64, 32, 0,
						G_TX_WRAP, G_TX_WRAP, 6, 5, G_TX_NOLOD, G_TX_NOLOD);

	for (u32 tile_x = start_x; tile_x < end_x; ++tile_x) {
		for (u32 tile_z = start_z; tile_z < end_z; ++tile_z) {
			u32 i = tile_x + (tile_z * map->width);
			if (map->tiles[i] >= TL_Ground_Start && map->tiles[i] <= TL_Objects_End) {
				u32 x = tile_x * TILE_SIZE;
				u32 z = tile_z * TILE_SIZE;

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
	}

	// walls
	gSPTexture((*glistp)++, 2048, 2048, 0, G_TX_RENDERTILE, G_ON);
	int wall_type = -1;

	for (u32 tile_x = start_x; tile_x < end_x; ++tile_x) {
		for (u32 tile_z = start_z; tile_z < end_z; ++tile_z) {
			u32 i = tile_x + (tile_z * map->width);

			if (map->tiles[i] < TL_Wall_Start || map->tiles[i] > TL_Wall_End)
				continue;

			if (map->tiles[i] >= TT_Wall_Full && map->tiles[i] < TT_Upstairs_East &&
				wall_type != 0) {
				gDPLoadTextureBlock((*glistp)++, map->spr_wall, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32,
									0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
				wall_type = 0;
			} else if (map->tiles[i] >= TT_Upstairs_East && map->tiles[i] <= TT_Upstairs_North &&
					   wall_type != 1) {
				gDPLoadTextureBlock((*glistp)++, map->spr_wall_upstairs, G_IM_FMT_RGBA,
									G_IM_SIZ_16b, 32, 32, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD,
									G_TX_NOLOD);
				wall_type = 1;
			} else if (map->tiles[i] >= TT_Downstairs_East &&
					   map->tiles[i] <= TT_Downstairs_North && wall_type != 2) {
				gDPLoadTextureBlock((*glistp)++, map->spr_wall_exit, G_IM_FMT_RGBA, G_IM_SIZ_16b,
									32, 32, 0, G_TX_WRAP, G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);
				wall_type = 2;
			}

			u32 x = tile_x * TILE_SIZE;
			u32 z = tile_z * TILE_SIZE;
			switch (map->tiles[i]) {
				case TT_Wall_Full:
					DRAW_WALL_SQUARE(x, z);
					break;
				case TT_Wall_East:
				case TT_Upstairs_East:
				case TT_Downstairs_East:
					DRAW_WALL_EAST(x, z);
					break;
				case TT_Wall_South:
				case TT_Upstairs_South:
				case TT_Downstairs_South:
					DRAW_WALL_SOUTH(x, z);
					break;
				case TT_Wall_West:
				case TT_Upstairs_West:
				case TT_Downstairs_West:
					DRAW_WALL_WEST(x, z);
					break;
				case TT_Wall_North:
				case TT_Upstairs_North:
				case TT_Downstairs_North:
					DRAW_WALL_NORTH(x, z);
					break;
				case TT_Wall_South_East:
					DRAW_WALL_SOUTH_EAST(x, z);
					break;
				case TT_Wall_South_West:
					DRAW_WALL_SOUTH_WEST(x, z);
					break;
				case TT_Wall_South_North:
					DRAW_WALL_SOUTH_NORTH(x, z);
					break;
				case TT_Wall_North_East:
					DRAW_WALL_NORTH_EAST(x, z);
					break;
				case TT_Wall_North_West:
					DRAW_WALL_NORTH_WEST(x, z);
					break;
				case TT_Wall_East_West:
					DRAW_WALL_EAST_WEST(x, z);
					break;
				case TT_Wall_No_West:
					DRAW_WALL_NO_WEST(x, z);
					break;
				case TT_Wall_No_East:
					DRAW_WALL_NO_EAST(x, z);
					break;
				case TT_Wall_No_North:
					DRAW_WALL_NO_NORTH(x, z);
					break;
				case TT_Wall_No_South:
					DRAW_WALL_NO_SOUTH(x, z);
					break;
				default:
					break;
			}
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

typedef enum MapOverviewIcon {
	MO_Ground = 0,
	MO_Wall_Full,
	MO_Wall_East,
	MO_Wall_South,
	MO_Wall_West,
	MO_Wall_North,
	MO_Wall_South_East,
	MO_Wall_South_West,
	MO_Wall_South_North,
	MO_Wall_North_East,
	MO_Wall_North_West,
	MO_Wall_East_West,
	MO_Wall_No_West,
	MO_Wall_No_East,
	MO_Wall_No_North,
	MO_Wall_No_South,
	MO_Stairs_Down,
	MO_Stairs_Up,
	MO_Player_Position,
	MO_Empty = 24,
	MO_Max,
} MapOverviewIcon;

const int map_overview_icon_positions_x[] = {
	0, 1, 2, 3, 4, /**/
	0, 1, 2, 3, 4, /**/
	0, 1, 2, 3, 4, /**/
	0, 1, 2, 3, 4, /**/
	0, 1, 2, 3, 4, /**/
};
const int map_overview_icon_positions_y[] = {
	0, 0, 0, 0, 0, /**/
	1, 1, 1, 1, 1, /**/
	2, 2, 2, 2, 2, /**/
	3, 3, 3, 3, 3, /**/
	4, 4, 4, 4, 4, /**/
};

#define MAP_DRAW_ICON(icon)                                                                        \
	{                                                                                              \
		int __s_x = tile_size * map_overview_icon_positions_x[icon];                               \
		int __s_y = tile_size * map_overview_icon_positions_y[icon];                               \
		gSPTextureRectangle((*gfx)++, (int)(x - tile_size_half) << 2,                              \
							(int)(y - tile_size_half) << 2, (int)(x + tile_size_half) << 2,        \
							(int)(y + tile_size_half) << 2, G_TX_RENDERTILE, __s_x << 5,           \
							__s_y << 5, 1 << 10, 1 << 10);                                         \
	}

void map_overview_render(Map *map, Gfx **gfx, Player *player, int begin_x, int end_x, int begin_y,
						 int end_y, int render_x, int render_y) {
	gSPDisplayList((*gfx)++, ui_setup_dl);
	gSPTexture((*gfx)++, 2048, 2048, 0, G_TX_RENDERTILE, G_ON);
	gDPLoadTextureBlock((*gfx)++, spr_map_ui, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_WRAP,
						G_TX_WRAP, 6, 5, G_TX_NOLOD, G_TX_NOLOD);

	const int tile_size = 6;
	const int tile_size_half = 3;

	int start_x, start_y;
	if (render_x >= 0) {
		start_x = render_x;
		start_y = render_y;
	} else {
		start_x = (SCREEN_WD - (map->width * tile_size)) / 2;
		start_y = (SCREEN_HT - ((map->size / map->width) * tile_size)) / 2;
		begin_x = 0;
		begin_y = 0;
	}

	for (u32 i = 0; i < map->size; ++i) {
		int tile_x = i % map->width;
		int tile_y = i / map->width;

		// limits when rendering
		if (render_x >= 0) {
			if (tile_x < begin_x || tile_x > end_x || tile_y < begin_y || tile_y > end_y)
				continue;
		}

		int x = start_x + ((tile_x - begin_x) * tile_size);
		int y = start_y + ((tile_y - begin_y) * tile_size);

		if (i == player->current_tile) {
			int blink_timer = (OS_CYCLES_TO_NSEC(osGetTime()) / 500000000);
			if (blink_timer % 2 == 0) {
				MAP_DRAW_ICON(MO_Player_Position);
			} else {
				MAP_DRAW_ICON(MO_Ground);
			}
		} else if (map->tiles[i] >= TT_Ground_1 && map->tiles[i] < TT_StartPos) {
			MAP_DRAW_ICON(MO_Ground);
		} else if (map->tiles[i] >= TT_Wall_Full && map->tiles[i] < TT_Upstairs_East) {
			MAP_DRAW_ICON(map->tiles[i]);
		} else if (map->tiles[i] >= TT_Upstairs_East && map->tiles[i] < TT_Downstairs_East) {
			MAP_DRAW_ICON(MO_Stairs_Up);
		} else if (map->tiles[i] >= TT_Downstairs_East && map->tiles[i] <= TT_Downstairs_North) {
			MAP_DRAW_ICON(MO_Stairs_Down);
		} else {
			MAP_DRAW_ICON(MO_Empty);
		}
	}
}
