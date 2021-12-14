#pragma once

#include <gbi.h>

typedef enum FontColorPalette {
	FCP_WHITE,
	FCP_BLUE,
	FCP_GREEN,
	FCP_GREY,
	FCP_RED,
	FCP_YELLOW,
	FCP_MAX,
} FontColorPalette;

void font_renderer_begin(Gfx **glistp);
void font_renderer_end(Gfx **glistp);

void font_renderer_set_color(Gfx **glistp, FontColorPalette palette);
void font_renderer_text(Gfx **glistp, int x, int y, char *text);
