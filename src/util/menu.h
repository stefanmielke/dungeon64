#pragma once

#include "../game.h"

typedef struct MenuItem {
	char *text;
	int x;
	int y;
} MenuItem;

typedef struct Menu {
	MenuItem *items;
	u8 total_items;
	int current_menu_option;
} Menu;

Menu *menu_init(MemZone *memory_pool, u8 total_items);
int menu_tick(Menu *menu);
void menu_add_item(Menu *menu, u8 index, char *text, int x, int y);
void menu_render(Menu *menu, Gfx **gfx);
