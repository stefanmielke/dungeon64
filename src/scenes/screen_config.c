#include "screen_config.h"

#include <ultra64.h>

#include "../../libs/ultra64-extensions/include/scene_manager.h"

#include "game_screen.h"
#include "main_screen.h"
// #include "no_save_screen.h"

extern SceneManager *scene_manager;

void change_screen(short curr_screen, short next_screen) {
	switch (next_screen) {
		// case SCREEN_NOSAVE:
		// 	scene_manager_set_callbacks(scene_manager, NULL, &no_save_screen_tick,
		// 								&no_save_screen_display, NULL);
		// 	break;
		case SCREEN_MAIN:
			scene_manager_set_callbacks(scene_manager, &main_screen_create, &main_screen_tick,
										&main_screen_display, NULL);
			break;
		// // case SCREEN_MAIN_MENU:
		// // 	scene_manager_set_callbacks(scene_manager, &main_menu_screen_create,
		// // 								&main_menu_screen_tick, &main_menu_screen_display,
		// NULL);
		// // 	break;
		case SCREEN_PLAY:
			scene_manager_set_callbacks(scene_manager, &game_screen_create, &game_screen_tick,
										&game_screen_display, NULL);
			break;
		default:
			break;
	}
}