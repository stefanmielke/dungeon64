#include "game_load_map_screen.h"

#include "scene_defs.h"

void game_load_map_screen_create() {
}

short game_load_map_screen_tick() {
	return SCREEN_PLAY;
}

void game_load_map_screen_display() {
	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);
}
