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

/*
 * frame buffer symbols
 */
extern u16 zbuffer[];					 /* RAM address */
extern u16 cfb[][SCREEN_WD * SCREEN_HT]; /* RAM address */
extern u16 rsp_cfb[];					 /* segment address */
