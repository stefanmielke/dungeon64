#include "map_helper.h"

#include "../definitions.h"

Vec3 map_get_start_position(unsigned short map[], unsigned long size, unsigned long width) {
	for (unsigned long i = 0; i < size; ++i) {
		if (map[i] == 255) {
			return map_get_position_from_map_coord(i, size, width);
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