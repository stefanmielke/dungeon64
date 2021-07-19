#include "menu.h"

#include "../scenes/scene_defs.h"

Menu *menu_init(MemZone *memory_pool, u8 total_items) {
	Menu *menu = mem_zone_alloc(memory_pool, sizeof(Menu));
	menu->total_items = total_items;
	menu->items = mem_zone_alloc(memory_pool, sizeof(MenuItem) * total_items);
	menu->current_add_index = 0;

	return menu;
}

void menu_add_item(Menu *menu, char *text, int x, int y) {
	if (menu->current_add_index >= menu->total_items)
		return;

	MenuItem *item = &menu->items[menu->current_add_index];
	item->text = text;
	item->x = x;
	item->y = y;

	menu->current_add_index++;
}

int menu_tick(Menu *menu) {
	gd.pad = ReadController(START_BUTTON | A_BUTTON | B_BUTTON | U_JPAD | D_JPAD);
	if (IS_BUTTON_PRESSED(U_JPAD)) {
		menu->current_menu_option--;
		if (menu->current_menu_option < 0)
			menu->current_menu_option = menu->total_items - 1;
	} else if (IS_BUTTON_PRESSED(D_JPAD)) {
		menu->current_menu_option++;
		if (menu->current_menu_option >= menu->total_items)
			menu->current_menu_option = 0;
	} else if (IS_BUTTON_PRESSED(A_BUTTON) || IS_BUTTON_PRESSED(START_BUTTON)) {
		return menu->current_menu_option;
	}

	return -1;
}

void menu_render(Menu *menu, Gfx **gfx) {
	FONTCOLM(FONT_COL_GREY);
	for (u8 i = 0; i < menu->total_items; ++i) {
		if (i == menu->current_menu_option)
			FONTCOLM(FONT_COL);

		SHOWFONT(gfx, menu->items[i].text, menu->items[i].x, menu->items[i].y);

		if (i == menu->current_menu_option)
			FONTCOLM(FONT_COL_GREY);
	}
}
