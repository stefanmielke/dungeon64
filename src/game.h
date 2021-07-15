#pragma once

#include "boot.h"
#include "types.h"
#include "fonts/font_ext.h"
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

#define GLIST_LEN 2048

/* buffer size for RDP DL */
#define RDP_OUTPUT_LEN (4096 * 16)

/* used for matrix stack */
extern u64 dram_stack[];

/* buffer for RDP DL */
extern u64 rdp_output[];

#define MAX_OBJECTS 200
#define MAX_BILLBOARDS 100
/*
 * Layout of dynamic data.
 *
 * This structure holds the things which change per frame. It is advantageous
 * to keep dynamic data together so that we may selectively write back dirty
 * data cache lines to DRAM prior to processing by the RCP.
 *
 */
typedef struct {
	Mtx projection;
	Mtx modeling;
	Mtx ballmodel;
	Mtx ballshadowmodel;
	Mtx ballshadowmodel2;
	Mtx viewing;
	Mtx billboard_rotation[MAX_BILLBOARDS];
	Mtx object_position[MAX_OBJECTS];
	Gfx glist[GLIST_LEN];
} Dynamic;

typedef struct {
	OSContPad **pad;
} GameData;

typedef struct {
	OSTask *theadp;
	Dynamic *dynamicp;
	u16 perspnorm;
	Mat4 modmat;
	Mat4 m1, m2;
	Mat4 allmat;
} RenderData;

extern Dynamic dynamic;
extern GameData gd;
extern RenderData rd;
extern Gfx *glistp;
extern MemZone memory_pool;

/* FONT */
extern int fontcol[4];
#define FONT_COL_WHITE 255, 255, 255, 255
#define FONT_COL_GREY 100, 100, 100, 255
#define FONT_COL 55, 155, 255, 255
#define FONTCOL(r, g, b, a)                                                                        \
	{                                                                                              \
		fontcol[0] = r;                                                                            \
		fontcol[1] = g;                                                                            \
		fontcol[2] = b;                                                                            \
		fontcol[3] = a;                                                                            \
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
