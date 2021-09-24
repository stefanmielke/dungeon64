#pragma once

#include "../game.h"

typedef struct MenuItem {
	char *text;
	int x;
	int y;
	bool enabled;
} MenuItem;

typedef struct Menu {
	MenuItem *items;
	u8 total_items;
	u8 current_add_index;
	int current_menu_option;

	int active_submenu;
	struct Menu **submenus;

	bool is_horizontal;
	int move_vertical_skip;
} Menu;

Menu *menu_init(MemZone *memory_pool, u8 total_items);
void menu_reset_items(Menu *menu);
void menu_set_horizontal(Menu *menu, int move_vertical_skip);
int menu_tick(Menu *menu, bool should_read_controller);
void menu_add_item(Menu *menu, char *text, int x, int y, bool enabled);
void menu_render(Menu *menu, Gfx **gfx);

void menu_init_submenus(Menu *menu, MemZone *memory_pool, u8 total_submenus);
