#pragma once

#include "boot.h"
#include "types.h"
#include "graphics.h"
#include "fonts/font_ext.h"
#include "combat/player.h"
#include "../libs/ultra64-extensions/include/mem_pool.h"

/*
 * Message queues
 */
extern OSMesgQueue rdpMessageQ, retraceMessageQ;

/*
 * global variables
 */
extern int rdp_flag;
extern char *staticSegment;
extern char *textureSegment;
extern char *position_str;

/* buffer size for RDP DL */
#define RDP_OUTPUT_LEN (4096 * 16)

/* used for matrix stack */
extern u64 dram_stack[];

/* buffer for RDP DL */
extern u64 rdp_output[];

extern Dynamic dynamic;
extern GameData gd;
extern RenderData rd;
extern Gfx *glistp;
extern MemZone memory_pool;
extern Player player;

/* FONT */
extern int fontcol[4];
#define FONT_COL_WHITE 255, 255, 255, 255
#define FONT_COL_GREY 100, 100, 100, 255
#define FONT_COL_YELLOW 255, 255, 55, 255
#define FONT_COL_GREEN 55, 255, 55, 255
#define FONT_COL_RED 255, 55, 55, 255
#define FONT_COL 55, 155, 255, 255
#define FONTCOL(r, g, b, a)                                                                        \
	{                                                                                              \
		fontcol[0] = r;                                                                            \
		fontcol[1] = g;                                                                            \
		fontcol[2] = b;                                                                            \
		fontcol[3] = a;                                                                            \
	}
#define FONTCOLEX(colors)                                                                          \
	{                                                                                              \
		fontcol[0] = colors[0];                                                                    \
		fontcol[1] = colors[1];                                                                    \
		fontcol[2] = colors[2];                                                                    \
		fontcol[3] = colors[3];                                                                    \
	}
#define FONTCOLM(c) FONTCOL(c)
#define SHOWFONT(glp, str, x, y)                                                                   \
	{                                                                                              \
		font_set_color(0, 0, 0, 255);                                                              \
		font_set_pos((x) + (1), (y) + (0));                                                        \
		font_show_string(glp, str);                                                                \
		font_set_pos((x) + (0), (y) + (1));                                                        \
		font_show_string(glp, str);                                                                \
		font_set_color(fontcol[0], fontcol[1], fontcol[2], fontcol[3]);                            \
		font_set_pos(x, y);                                                                        \
		font_show_string(glp, str);                                                                \
	}

/*
 * frame buffer symbols
 */
extern u16 zbuffer[];					 /* RAM address */
extern u16 cfb[][SCREEN_WD * SCREEN_HT]; /* RAM address */
extern u16 rsp_cfb[];					 /* segment address */
