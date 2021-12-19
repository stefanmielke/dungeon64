#include <ultra64.h>

#define PLANT_SIZE 5
static Vtx plant_vtx[] = {
	{-2, 4, 0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{2, 4, 00, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{2, 0, 00, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-2, 0, 0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};

Gfx plant_dl[] = {
	gsSPVertex(&(plant_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

Gfx billboard_texture_setup_dl[] = {
	gsDPPipeSync(),

	gsDPSetRenderMode(G_RM_FOG_SHADE_A, G_RM_AA_ZB_TEX_EDGE2),
	gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_PASS2),

	gsSPEndDisplayList(),
};
