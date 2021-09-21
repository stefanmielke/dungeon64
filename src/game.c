#include "game.h"

#include <ultra64.h>

#include "static.h"
#include "controller.h"
#include "scenes/screen_config.h"

#include "debug.h"

#include "../libs/ultra64-extensions/include/scene_manager.h"

// Task header
OSTask taskHeader = {
	M_GFXTASK,						 /* type of task */
	OS_TASK_DP_WAIT,				 /* flags - wait for DP to be available */
	NULL,							 /* ucode boot (fill in later) */
	0,								 /* ucode boot size (fill in later) */
	NULL,							 /* ucode (fill in later) */
	SP_UCODE_SIZE,					 /* ucode size */
	NULL,							 /* ucode data (fill in later) (to init DMEM) */
	SP_UCODE_DATA_SIZE,				 /* ucode data size */
	&dram_stack[0],					 /* stack used by ucode */
	SP_DRAM_STACK_SIZE8,			 /* size of stack */
	&rdp_output[0],					 /* output fifo buffer start */
	&rdp_output[0] + RDP_OUTPUT_LEN, /* output fifo buffer end */
	NULL,							 /* display list pointer (fill in later) */
	0,								 /* display list size (ignored) */
	NULL,							 /* yield buffer (used if yield will occur) */
	0								 /* yield buffer length */
};

// global variables
Gfx *glistp;		 /* RSP display list pointer */
Dynamic dynamic;	 /* dynamic data */
int draw_buffer = 0; /* frame buffer being updated (0 or 1) */
int fontcol[4];

GameData gd;
RenderData rd;
Player player;

// Viewport
Vp vp = {
	SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0, /* scale */
	SCREEN_WD * 2, SCREEN_HT * 2, G_MAXZ / 2, 0, /* translate */
};

#define GLOBAL_MEM_POOL_SIZE (1024)
abort();
#define MEM_POOL_SIZE (4 * 1024)
char global_memory[GLOBAL_MEM_POOL_SIZE];
MemZone global_memory_pool;
char scene_memory[MEM_POOL_SIZE];
MemZone memory_pool;
SceneManager *scene_manager;

// called once to setup the game
void setup();

// called every frame to update data
void update();

// called every frame to render data to screen
void render();

// This is the main routine of the app.
void game(void) {
	setup();

	// Main game loop
	while (1) {
		update();
		render();
	}
}

void setup() {
#if DEBUG_MODE
	debug_initialize();
#endif

	mem_zone_init(&global_memory_pool, global_memory, GLOBAL_MEM_POOL_SIZE);
	mem_zone_init(&memory_pool, scene_memory, MEM_POOL_SIZE);

	scene_manager = scene_manager_init(&global_memory_pool, &memory_pool, &change_screen);

	scene_manager_change_scene(scene_manager, SCREEN_MAIN);

	player_game_init(&player);
}

void update() {
	scene_manager_tick(scene_manager);
}

void render() {
	// pointers to build the display list.
	rd.theadp = &taskHeader;
	rd.dynamicp = &dynamic;
	glistp = rd.dynamicp->glist;

	// Tell RCP where each segment is
	gSPSegment(glistp++, 0, 0x0); /* physical segment */
	gSPSegment(glistp++, STATIC_SEGMENT, OS_K0_TO_PHYSICAL(staticSegment));
	gSPSegment(glistp++, CFB_SEGMENT, OS_K0_TO_PHYSICAL(cfb[draw_buffer]));
	gSPSegment(glistp++, TEXTURE_SEGMENT, OS_K0_TO_PHYSICAL(textureSegment));

	// Clear z and color framebuffer.
	gSPDisplayList(glistp++, clearzbuffer_dl);
	gSPDisplayList(glistp++, clearcfb_dl);

	// Modify & specify Viewport
	gSPViewport(glistp++, &vp);

	// Initialize RCP state.
	gSPDisplayList(glistp++, init_dl);

	if (scene_manager->current_scene_id != scene_manager->next_scene_id) {
		font_init(&glistp);
		font_set_transparent(1);
		font_set_scale(1.0, 1.0);
		font_set_win(200, 1);
		FONTCOLM(FONT_COL_WHITE);
		SHOWFONT(&glistp, "Loading", 30, 200);
		font_finish(&glistp);
	} else {
		scene_manager_display(scene_manager);
	}

	// Finish rendering
	gDPFullSync(glistp++);
	gSPEndDisplayList(glistp++);

	// Build graphics task
	rd.theadp->t.ucode_boot = (u64 *)rspbootTextStart;
	rd.theadp->t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;

	// RSP output over XBUS to RDP:
	rd.theadp->t.ucode = (u64 *)gspF3DEX2_xbusTextStart;
	rd.theadp->t.ucode_data = (u64 *)gspF3DEX2_xbusDataStart;

	// initial display list
	rd.theadp->t.data_ptr = (u64 *)rd.dynamicp->glist;
	rd.theadp->t.data_size = (u32)((glistp - rd.dynamicp->glist) * sizeof(Gfx));

	// Write back dirty cache lines that need to be read by the RCP.
	osWritebackDCache(&dynamic, sizeof(dynamic));

	// start up the RSP task
	osSpTaskStart(rd.theadp);

	// wait for DP completion
	(void)osRecvMesg(&rdpMessageQ, NULL, OS_MESG_BLOCK);

	// setup to swap buffers
	osViSwapBuffer(cfb[draw_buffer]);

	// Make sure there isn't an old retrace in queue (assumes queue has a depth of 1)
	if (MQ_IS_FULL(&retraceMessageQ))
		(void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);

	// Wait for Vertical retrace to finish swap buffers
	(void)osRecvMesg(&retraceMessageQ, NULL, OS_MESG_BLOCK);
	draw_buffer ^= 1;
}
