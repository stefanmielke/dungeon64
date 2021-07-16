#include <ultra64.h>

#define BLUE_DRAGON_SIZE 5
static Vtx blue_dragon_vtx[] = {
	{-0, BLUE_DRAGON_SIZE, 0, 0,               32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{BLUE_DRAGON_SIZE, BLUE_DRAGON_SIZE, 0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{BLUE_DRAGON_SIZE, -0, -0, 0,              00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0,                            32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};

Gfx blue_dragon_dl[] = {
	gsSPVertex(&(blue_dragon_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};