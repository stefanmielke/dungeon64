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
} Menu;

Menu *menu_init(MemZone *memory_pool, u8 total_items);
int menu_tick(Menu *menu);
void menu_add_item(Menu *menu, char *text, int x, int y, bool enabled);
void menu_render(Menu *menu, Gfx **gfx);
