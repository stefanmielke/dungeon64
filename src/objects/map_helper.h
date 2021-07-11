#pragma once

#include "../definitions.h"
#include "../game.h"
#include "../types.h"

Vec3 map_get_start_position(Map *map);
Vec3 map_get_position_from_map_coord(unsigned long map_coord, unsigned long size,
									 unsigned long width);
void map_render(Map *map, Gfx **glistp, Dynamic *dynamicp);
