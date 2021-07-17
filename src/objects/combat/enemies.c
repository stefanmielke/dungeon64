#include <ultra64.h>

#define ENEMY_32_SIZE 4
static Vtx enemy_32_vtx[] = {
	{-0, ENEMY_32_SIZE, 0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{ENEMY_32_SIZE, ENEMY_32_SIZE, 0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{ENEMY_32_SIZE, -0, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};

#define ENEMY_64_SIZE 5
static Vtx enemy_64_vtx[] = {
	{-0, ENEMY_64_SIZE, 0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{ENEMY_64_SIZE, ENEMY_64_SIZE, 0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{ENEMY_64_SIZE, -0, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};
static Vtx enemy_64_2_vtx[] = {
	{-0, ENEMY_64_SIZE, 0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{ENEMY_64_SIZE, ENEMY_64_SIZE, 0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{ENEMY_64_SIZE, -0, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};
static Vtx enemy_64_3_vtx[] = {
	{-0, ENEMY_64_SIZE, 0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{ENEMY_64_SIZE, ENEMY_64_SIZE, 0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{ENEMY_64_SIZE, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};
static Vtx enemy_64_4_vtx[] = {
	{-0, ENEMY_64_SIZE, 0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{ENEMY_64_SIZE, ENEMY_64_SIZE, 0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{ENEMY_64_SIZE, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};

Gfx enemy_32_dl[] = {
	gsSPVertex(&(enemy_32_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

Gfx enemy_64_dl[] = {
	gsSPVertex(&(enemy_64_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

Gfx enemy_64_dl2[] = {
	gsSPVertex(&(enemy_64_2_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

Gfx enemy_64_dl3[] = {
	gsSPVertex(&(enemy_64_3_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

Gfx enemy_64_dl4[] = {
	gsSPVertex(&(enemy_64_4_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};
