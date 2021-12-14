#include "font_renderer.h"

#include <ultra64.h>

#include "../data/texture.h"

void font_renderer_begin(Gfx **glistp) {
	gDPSetTextureLUT((*glistp)++, G_TT_RGBA16);
	gDPLoadTLUT_pal256((*glistp)++, spr_font_palette_white);
	gDPLoadTextureBlock_4b((*glistp)++, spr_font, G_IM_FMT_CI, 64, 64, 0, G_TX_WRAP, G_TX_WRAP,
						   G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
}

void font_renderer_end(Gfx **glistp) {
	gDPPipeSync((*glistp)++);
	gDPSetTextureLUT((*glistp)++, G_TT_NONE);
}

void font_renderer_set_color(Gfx **glistp, FontColorPalette palette) {
	switch (palette) {
		case FCP_WHITE:
			gDPLoadTLUT_pal256((*glistp)++, spr_font_palette_white);
			break;
		case FCP_BLUE:
			gDPLoadTLUT_pal256((*glistp)++, spr_font_palette_blue);
			break;
		case FCP_GREEN:
			gDPLoadTLUT_pal256((*glistp)++, spr_font_palette_green);
			break;
		case FCP_GREY:
			gDPLoadTLUT_pal256((*glistp)++, spr_font_palette_grey);
			break;
		case FCP_RED:
			gDPLoadTLUT_pal256((*glistp)++, spr_font_palette_red);
			break;
		case FCP_YELLOW:
			gDPLoadTLUT_pal256((*glistp)++, spr_font_palette_yellow);
			break;
		default:
			break;
	}
}

void font_renderer_text(Gfx **glistp, int x, int y, char *text) {
	int length = strlen(text);
	for (u16 i = 0; i < length; ++i) {
		int cur_x = x + (i * 8);
		u8 c = text[i];

		// if out of texture bounds, render space
		if (c < 32 || c > 95)
			c = 32;

		c -= 32;

		const int font_size = 8, font_half = 4;

		int __tile_x = font_size * (c % 8);
		int __tile_y = font_size * (c / 8);
		gSPTextureRectangle((*glistp)++, (int)((cur_x)-font_half) << 2, (int)((y)-font_half) << 2,
							(int)((cur_x) + font_half) << 2, (int)((y) + font_half) << 2,
							G_TX_RENDERTILE, __tile_x << 5, __tile_y << 5, 1 << 10, 1 << 10);
	}
}
