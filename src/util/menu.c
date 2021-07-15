#include "menu.h"

Menu *menu_init(MemZone *memory_pool, u8 total_items) {
	Menu *menu = mem_zone_alloc(memory_pool, sizeof(Menu));
	menu->total_items = total_items;
	menu->items = mem_zone_alloc(memory_pool, sizeof(MenuItem) * total_items);

	return menu;
}

void menu_add_item(Menu *menu, u8 index, char *text, int x, int y) {
	menu->items[index].text = text;
	menu->items[index].x = x;
	menu->items[index].y = y;
}

void menu_render(Menu *menu, Gfx **gfx, u8 selected_item) {
	FONTCOLM(FONT_COL_GREY);
	for (u8 i = 0; i < menu->total_items; ++i) {
		if (i == selected_item)
			FONTCOLM(FONT_COL);

		SHOWFONT(gfx, menu->items[i].text, menu->items[i].x, menu->items[i].y);

		if (i == selected_item)
			FONTCOLM(FONT_COL_GREY);
	}
}
