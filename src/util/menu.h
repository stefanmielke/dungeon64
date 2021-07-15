#pragma once

#include "../game.h"

typedef struct {
	char *text;
	int x;
	int y;
} MenuItem;

typedef struct {
	MenuItem *items;
	u8 total_items;
} Menu;

Menu *menu_init(MemZone *memory_pool, u8 total_items);
void menu_add_item(Menu *menu, u8 index, char *text, int x, int y);
void menu_render(Menu *menu, Gfx **gfx, u8 selected_item);
