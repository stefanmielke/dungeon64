#pragma once

#include <ultratypes.h>

// id of the map to load
extern u16 map_to_load;
extern s32 forced_position_to_load;

void game_screen_create();
short game_screen_tick();
void game_screen_display();
