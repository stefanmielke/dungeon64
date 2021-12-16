#pragma once

static Gfx spr_ui_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_ui[] = {
#include "../res/gfx/ui/ui.gen.h"
};

static Gfx spr_map_ui_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_map_ui[] = {
#include "../res/gfx/textures/map_ui.gen.h"
};

static Gfx spr_font_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_font[] = {
#include "../res/gfx/font/font.gen.h"
};

static Gfx spr_font_palette_white_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_font_palette_white[] = {0x0000, 0xffff, 0x0000, 0x0000, 0x0000, 0x0000,
										   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
										   0x0000, 0x0000, 0x0000, 0x0000};

static Gfx spr_font_palette_blue_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_font_palette_blue[] = {0x0000, 0x3cff, 0x0000, 0x0000, 0x0000, 0x0000,
										  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
										  0x0000, 0x0000, 0x0000, 0x0000};

static Gfx spr_font_palette_green_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_font_palette_green[] = {0x0000, 0x3fcf, 0x0000, 0x0000, 0x0000, 0x0000,
										   0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
										   0x0000, 0x0000, 0x0000, 0x0000};

static Gfx spr_font_palette_grey_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_font_palette_grey[] = {0x0000, 0x6319, 0x0000, 0x0000, 0x0000, 0x0000,
										  0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
										  0x0000, 0x0000, 0x0000, 0x0000};

static Gfx spr_font_palette_red_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_font_palette_red[] = {0x0000, 0xf9cf, 0x0000, 0x0000, 0x0000, 0x0000,
										 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
										 0x0000, 0x0000, 0x0000, 0x0000};

static Gfx spr_font_palette_yellow_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_font_palette_yellow[] = {0x0000, 0xffcf, 0x0000, 0x0000, 0x0000, 0x0000,
											0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
											0x0000, 0x0000, 0x0000, 0x0000};
