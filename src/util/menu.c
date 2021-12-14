#include "menu.h"

#include "../graphics.h"
#include "../static.h"
#include "../data/texture.h"
#include "../scenes/scene_defs.h"
#include "../util/font_renderer.h"

typedef enum MenuIcon {
	MI_ABUTTON = 0,
	MI_BBUTTON,
	MI_CBUTTON_UP,
	MI_CBUTTON_RIGHT,
	MI_CBUTTON_LEFT,
	MI_CBUTTON_DOWN,
	MI_ZBUTTON,
	MI_STICK,
	MI_COIN,
	MI_HAND,
	MI_STARTBUTTON,
	MI_RBUTTON,
} MenuIcon;

void menu_draw_ui_icon(Gfx **gfx, MenuIcon icon, int x, int y);

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

void menu_set_hand(Menu *menu, int hand_position_x) {
	menu->hand_position_x = hand_position_x;
}

void menu_add_item(Menu *menu, char *text, int x, int y, bool enabled) {
	if (menu->current_add_index >= menu->total_items)
		return;

	MenuItem *item = &menu->items[menu->current_add_index];
	item->text = text;
	item->x = x;
	item->y = y;
	item->enabled = enabled;
	item->has_custom_colors = false;

	menu->current_add_index++;
}

void menu_add_item_colored(Menu *menu, char *text, int x, int y, bool enabled,
						   FontColorPalette color_selected, FontColorPalette color_enabled,
						   FontColorPalette color_disabled) {
	if (menu->current_add_index >= menu->total_items)
		return;

	MenuItem *item = &menu->items[menu->current_add_index];
	item->text = text;
	item->x = x;
	item->y = y;
	item->enabled = enabled;
	item->has_custom_colors = true;

	item->color_selected = color_selected;
	item->color_enabled = color_enabled;
	item->color_disabled = color_disabled;

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
		return menu_render(menu->submenus[menu->active_submenu], gfx);
	}

	for (u8 i = 0; i < menu->current_add_index; ++i) {
		if (i == menu->current_menu_option) {
			if (menu->items[i].has_custom_colors) {
				font_renderer_set_color(gfx, menu->items[i].color_selected);
			} else {
				font_renderer_set_color(gfx, FCP_BLUE);
			}
		} else if (menu->items[i].enabled) {
			if (menu->items[i].has_custom_colors) {
				font_renderer_set_color(gfx, menu->items[i].color_enabled);
			} else {
				font_renderer_set_color(gfx, FCP_WHITE);
			}
		} else {
			if (menu->items[i].has_custom_colors) {
				font_renderer_set_color(gfx, menu->items[i].color_disabled);
			} else {
				font_renderer_set_color(gfx, FCP_GREY);
			}
		}

		font_renderer_text(gfx, menu->items[i].x, menu->items[i].y, menu->items[i].text);
	}
	font_renderer_set_color(gfx, FCP_GREY);
}

void menu_render_images(Menu *menu, Gfx **gfx) {
	if (menu->active_submenu >= 0 && menu->submenus) {
		return menu_render_images(menu->submenus[menu->active_submenu], gfx);
	}

	if (menu->current_add_index <= 0)
		return;

	gSPDisplayList((*gfx)++, ui_setup_dl);
	gDPLoadTextureBlock((*gfx)++, spr_ui, G_IM_FMT_RGBA, G_IM_SIZ_16b, 48, 36, 0, G_TX_WRAP,
						G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);

	int option = menu->current_menu_option;
	if (!menu->is_horizontal && menu->current_add_index > 0)
		menu_draw_ui_icon(gfx, MI_HAND, menu->hand_position_x - 2, menu->items[option].y + 3);
}

void menu_init_submenus(Menu *menu, MemZone *memory_pool, u8 total_submenus) {
	// do not recreate submenus
	if (menu->submenus)
		return;

	menu->submenus = mem_zone_alloc(memory_pool, sizeof(Menu *) * total_submenus);
}

void menu_draw_ui_icon(Gfx **gfx, MenuIcon icon, int x, int y) {
	int __s_x = 12 * (icon % 4);
	int __s_y = 12 * (icon / 4);
	gSPTextureRectangle((*gfx)++, (int)(x - 6) << 2, (int)(y - 6) << 2, (int)(x + 6) << 2,
						(int)(y + 6) << 2, G_TX_RENDERTILE, __s_x << 5, __s_y << 5, 1 << 10,
						1 << 10);
}
