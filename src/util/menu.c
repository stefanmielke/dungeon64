#include "menu.h"

#include "../scenes/scene_defs.h"

Menu *menu_init(MemZone *memory_pool, u8 total_items) {
	Menu *menu = mem_zone_alloc(memory_pool, sizeof(Menu));
	menu->total_items = total_items;
	menu->items = mem_zone_alloc(memory_pool, sizeof(MenuItem) * total_items);
	menu->current_menu_option = 0;
	menu_reset_items(menu);

	menu->is_horizontal = false;
	menu->move_vertical_skip = 1;

	menu->active_submenu = -1;
	menu->submenus = NULL;

	return menu;
}

void menu_reset_items(Menu *menu) {
	menu->current_add_index = 0;
	for (u8 i = 0; i < menu->total_items; ++i) {
		menu->items[i].enabled = false;
		menu->items[i].text = NULL;
		menu->items[i].x = 0;
		menu->items[i].y = 0;
	}
}

void menu_set_horizontal(Menu *menu, int move_vertical_skip) {
	menu->is_horizontal = true;
	menu->move_vertical_skip = move_vertical_skip;
}

void menu_add_item(Menu *menu, char *text, int x, int y, bool enabled) {
	if (menu->current_add_index >= menu->total_items)
		return;

	MenuItem *item = &menu->items[menu->current_add_index];
	item->text = text;
	item->x = x;
	item->y = y;
	item->enabled = enabled;

	menu->current_add_index++;
}

int _menu_tick_internal(Menu *menu) {
	if (menu->active_submenu >= 0 && menu->submenus) {
		Menu **menus = menu->submenus;
		Menu *active_submenu = menus[menu->active_submenu];
		return _menu_tick_internal(active_submenu);
	}
	if (menu->current_add_index <= 0)
		return -1;

	if (!menu->items[menu->current_menu_option].enabled) {
		for (u8 i = 0; i < menu->current_add_index; ++i) {
			menu->current_menu_option += 1;
			if (menu->current_menu_option >= menu->current_add_index)
				menu->current_menu_option = 0;

			if (menu->items[menu->current_menu_option].enabled)
				break;
		}
	}

	if (IS_BUTTON_PRESSED(U_JPAD)) {
		for (u8 i = 0; i < menu->current_add_index; ++i) {
			menu->current_menu_option -= menu->move_vertical_skip;
			if (menu->current_menu_option < 0)
				menu->current_menu_option = menu->current_add_index + menu->current_menu_option;

			if (menu->items[menu->current_menu_option].enabled)
				break;
		}
	} else if (IS_BUTTON_PRESSED(D_JPAD)) {
		for (u8 i = 0; i < menu->current_add_index; ++i) {
			menu->current_menu_option += menu->move_vertical_skip;
			if (menu->current_menu_option >= menu->current_add_index)
				menu->current_menu_option = menu->current_menu_option - menu->current_add_index;

			if (menu->items[menu->current_menu_option].enabled)
				break;
		}
	} else if (IS_BUTTON_PRESSED(L_JPAD)) {
		for (u8 i = 0; i < menu->current_add_index; ++i) {
			menu->current_menu_option -= 1;
			if (menu->current_menu_option < 0)
				menu->current_menu_option = menu->current_add_index - 1;

			if (menu->items[menu->current_menu_option].enabled)
				break;
		}
	} else if (IS_BUTTON_PRESSED(R_JPAD)) {
		for (u8 i = 0; i < menu->current_add_index; ++i) {
			menu->current_menu_option += 1;
			if (menu->current_menu_option >= menu->current_add_index)
				menu->current_menu_option = 0;

			if (menu->items[menu->current_menu_option].enabled)
				break;
		}
	} else if (IS_BUTTON_PRESSED(A_BUTTON) || IS_BUTTON_PRESSED(START_BUTTON)) {
		if (menu->items[menu->current_menu_option].enabled)
			return menu->current_menu_option;
	}
	return -1;
}

int menu_tick(Menu *menu, bool should_read_controller) {
	if (should_read_controller)
		gd.pad = ReadController(START_BUTTON | A_BUTTON | B_BUTTON | U_JPAD | D_JPAD | L_JPAD |
								R_JPAD);

	return _menu_tick_internal(menu);
}

void menu_render(Menu *menu, Gfx **gfx) {
	if (menu->active_submenu >= 0 && menu->submenus) {
		Menu **menus = menu->submenus;
		Menu *active_submenu = menus[menu->active_submenu];

		return menu_render(active_submenu, gfx);
	}

	for (u8 i = 0; i < menu->current_add_index; ++i) {
		if (i == menu->current_menu_option) {
			FONTCOLM(FONT_COL);
		} else if (menu->items[i].enabled) {
			FONTCOLM(FONT_COL_WHITE);
		} else {
			FONTCOLM(FONT_COL_GREY);
		}

		SHOWFONT(gfx, menu->items[i].text, menu->items[i].x, menu->items[i].y);

		if (i == menu->current_menu_option)
			FONTCOLM(FONT_COL_GREY);
	}
}

void menu_init_submenus(Menu *menu, MemZone *memory_pool, u8 total_submenus, u8 max_submenu_items) {
	// do not recreate submenus
	if (menu->submenus)
		return;

	Menu **menus = mem_zone_alloc(memory_pool, sizeof(Menu *) * total_submenus);
	for (u8 i = 0; i < total_submenus; ++i) {
		menus[i] = menu_init(memory_pool, max_submenu_items);
	}
	menu->submenus = (void *)menus;
}

void menu_add_submenu(Menu *parent, Menu *submenu, int submenu_index) {
	parent->submenus[submenu_index] = submenu;
}

void menu_submenu_set_horizontal(Menu *menu, int submenu_index, int move_vertical_skip) {
	if (!menu->submenus)
		return;

	menu_set_horizontal(menu->submenus[submenu_index], move_vertical_skip);
}

void menu_submenu_add_item(Menu *menu, int submenu_index, char *text, int x, int y, bool enabled) {
	if (!menu->submenus)
		return;

	menu_add_item(menu->submenus[submenu_index], text, x, y, enabled);
}
