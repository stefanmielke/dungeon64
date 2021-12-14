#pragma once

#include "font_renderer.h"
#include "../game.h"

typedef struct MenuItem {
	char *text;
	int x;
	int y;
	bool enabled;
	bool has_custom_colors;
	FontColorPalette color_enabled;
	FontColorPalette color_disabled;
	FontColorPalette color_selected;
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

	int hand_position_x;
} Menu;

Menu *menu_init(MemZone *memory_pool, u8 total_items);
void menu_reset_items(Menu *menu);
void menu_set_horizontal(Menu *menu, int move_vertical_skip);
void menu_set_hand(Menu *menu, int hand_position_x);
int menu_tick(Menu *menu, bool should_read_controller);
void menu_add_item(Menu *menu, char *text, int x, int y, bool enabled);
void menu_add_item_colored(Menu *menu, char *text, int x, int y, bool enabled,
						   FontColorPalette color_selected, FontColorPalette color_enabled,
						   FontColorPalette color_disabled);
void menu_render(Menu *menu, Gfx **gfx);
void menu_render_images(Menu *menu, Gfx **gfx);

void menu_init_submenus(Menu *menu, MemZone *memory_pool, u8 total_submenus);
