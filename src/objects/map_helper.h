#pragma once

#include <stdbool.h>

#include "../definitions.h"
#include "../game.h"
#include "../types.h"
#include "../combat/player.h"
#include "../../libs/ultra64-extensions/include/position.h"

Vec3 map_get_start_position(Map *map, u32 *tile_position, float *angle);
Vec3 map_get_position_from_map_coord(u32 map_coord, u32 size, u32 width);
void map_render(Map *map, Gfx **glistp, Dynamic *dynamicp, Player *player);

bool map_is_tile_blocked(Map *map, u32 tile);
bool map_is_position_blocked(Map *map, Position position);

MapEvent *map_get_event_on_tile(Map *map, u32 tile_position);
void map_overview_render(Map *map, Gfx **gfx, Player *player);
