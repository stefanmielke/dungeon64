#include <ultra64.h>

#define PLAYER_SIZE 4
static Vtx player_vtx[] = {
	{-0, PLAYER_SIZE, 0, 0,               32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{PLAYER_SIZE, PLAYER_SIZE, 0, 0,      00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{PLAYER_SIZE, -0, -0, 0,              00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0,                       32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};

Gfx player_dl[] = {
	gsSPVertex(&(player_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};
