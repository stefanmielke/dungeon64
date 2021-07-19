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
	void *submenus;
} Menu;

Menu *menu_init(MemZone *memory_pool, u8 total_items);
int menu_tick(Menu *menu);
void menu_add_item(Menu *menu, char *text, int x, int y, bool enabled);
void menu_render(Menu *menu, Gfx **gfx);

void menu_init_submenus(Menu *menu, MemZone *memory_pool, u8 total_submenus, u8 max_submenu_items);
void menu_submenu_add_item(Menu *menu, int submenu_index, char *text, int x, int y, bool enabled);
