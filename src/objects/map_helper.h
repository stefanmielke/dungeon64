#pragma once

#include "../types.h"

Vec3 map_get_start_position(unsigned short map[], unsigned long size, unsigned long width);
Vec3 map_get_position_from_map_coord(unsigned long map_coord, unsigned long size,
									 unsigned long width);
