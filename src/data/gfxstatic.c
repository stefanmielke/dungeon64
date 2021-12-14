/*
 * File:	gfxstatic.c
 *
 * This file holds display list segments that are 'static' data.
 *
 */

#include <ultra64.h>
#include "../boot.h"
#include "../game.h"
#include "texture.h"

static Vtx ground_vtx[] = {
	{00, 0, 00, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 0, 00, 0, 31 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 0, 10, 0, 31 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{00, 0, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};

static Vtx combat_ground_vtx[] = {
	{00, 0, 00, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{100, 0, 00, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{100, 0, 100, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{00, 0, 100, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};

static Vtx combat_selection_vtx[] = {
	{-1, 02, -1, 00, 00 << 10, 00 << 10, 0xff, 0x00, 0x00, 0xff},
	{01, 02, -1, 00, 32 << 10, 00 << 10, 0xff, 0x00, 0x00, 0xff},
	{01, 02, 01, 00, 32 << 10, 32 << 10, 0xff, 0x00, 0x00, 0xff},
	{-1, 02, 01, 00, 00 << 10, 32 << 10, 0xff, 0x00, 0x00, 0xff},
	{00, 00, 00, 00, 00 << 10, 00 << 10, 0xff, 0x00, 0x00, 0xff},
};
static Vtx ceiling_vtx[] = {
	{00, 0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 0, 10, 0, 63 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 0, 00, 0, 63 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{00, 0, 00, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};

Gfx ground_texture_setup_dl[] = {
	gsDPPipeSync(),

	gsDPSetCycleType(G_CYC_1CYCLE),
	gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_ZBUFFER | G_CULL_BACK),
	gsDPSetRenderMode(G_RM_ZB_OPA_SURF, G_RM_ZB_OPA_SURF2),
	gsDPSetCombineMode(G_CC_BLENDRGBA, G_CC_BLENDRGBA),
	gsDPSetTexturePersp(G_TP_PERSP),

	gsSPEndDisplayList(),
};

Gfx combat_ground_dl[] = {
	gsSPVertex(&(combat_ground_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

Gfx combat_selection_setup_dl[] = {
	gsDPPipeSync(),

	gsDPSetCycleType(G_CYC_1CYCLE),
	gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
	gsSPSetGeometryMode(G_SHADE | G_CULL_BACK),
	gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
	gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),
	gsDPSetTexturePersp(G_TP_PERSP),

	gsSPEndDisplayList(),
};
Gfx combat_selection_dl[] = {
	gsSPVertex(&(combat_selection_vtx[0]), 5, 0), /**/
	gsSP2Triangles(0, 4, 3, 0, 3, 4, 2, 0),		  /**/
	gsSP2Triangles(2, 4, 1, 0, 1, 4, 0, 0),		  /**/
	gsSPEndDisplayList(),						  /**/
};

Gfx ground_dl[] = {
	gsSPVertex(&(ground_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

Gfx ceiling_dl[] = {
	gsSPVertex(&(ceiling_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

Gfx ui_setup_dl[] = {
	gsDPSetCycleType(G_CYC_1CYCLE),
	gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA),
	gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE),
	gsDPSetDepthImage(OS_K0_TO_PHYSICAL(zbuffer)),
	gsDPSetDepthSource(G_ZS_PRIM),
	gsDPSetPrimDepth(0, 0),
	gsDPSetTexturePersp(G_TP_NONE),
	gsDPPipeSync(),
	gsSPEndDisplayList(),
};
