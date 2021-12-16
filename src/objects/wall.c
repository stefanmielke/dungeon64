#include <ultra64.h>

// one side
static Vtx wall_east_vtx[] = {
	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_east_dl[] = {
	gsSPVertex(&(wall_east_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

static Vtx wall_south_vtx[] = {
	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_south_dl[] = {
	gsSPVertex(&(wall_south_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

static Vtx wall_west_vtx[] = {
	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_west_dl[] = {
	gsSPVertex(&(wall_west_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

static Vtx wall_north_vtx[] = {
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_north_dl[] = {
	gsSPVertex(&(wall_north_vtx[0]), 4, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSPEndDisplayList(),
};

// two sides
static Vtx wall_south_east_vtx[] = {
	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},

	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_south_east_dl[] = {
	gsSPVertex(&(wall_south_east_vtx[0]), 6, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSP2Triangles(3, 5, 4, 3, 5, 3, 0, 3),
	gsSPEndDisplayList(),
};

static Vtx wall_south_west_vtx[] = {
	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},

	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_south_west_dl[] = {
	gsSPVertex(&(wall_south_west_vtx[0]), 6, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSP2Triangles(4, 1, 2, 4, 1, 4, 5, 4),
	gsSPEndDisplayList(),
};

static Vtx wall_south_north_vtx[] = {
	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},

	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_south_north_dl[] = {
	gsSPVertex(&(wall_south_north_vtx[0]), 8, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSP2Triangles(4, 6, 5, 4, 6, 4, 7, 4),
	gsSPEndDisplayList(),
};

static Vtx wall_north_east_vtx[] = {
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},

	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_north_east_dl[] = {
	gsSPVertex(&(wall_north_east_vtx[0]), 6, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSP2Triangles(4, 1, 2, 4, 1, 4, 5, 4),
	gsSPEndDisplayList(),
};

static Vtx wall_north_west_vtx[] = {
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},

	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_north_west_dl[] = {
	gsSPVertex(&(wall_north_west_vtx[0]), 6, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSP2Triangles(3, 5, 4, 3, 5, 3, 0, 3),
	gsSPEndDisplayList(),
};

static Vtx wall_east_west_vtx[] = {
	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},

	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_east_west_dl[] = {
	gsSPVertex(&(wall_east_west_vtx[0]), 8, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSP2Triangles(4, 6, 5, 4, 6, 4, 7, 4),
	gsSPEndDisplayList(),
};

// three sides
static Vtx wall_no_west_vtx[] = {
	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},

	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},

	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_no_west_dl[] = {
	gsSPVertex(&(wall_no_west_vtx[0]), 8, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSP2Triangles(3, 5, 4, 3, 5, 3, 0, 3),
	gsSP2Triangles(6, 4, 5, 6, 4, 6, 7, 6),
	gsSPEndDisplayList(),
};

static Vtx wall_no_east_vtx[] = {
	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},

	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},

	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_no_east_dl[] = {
	gsSPVertex(&(wall_no_east_vtx[0]), 8, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSP2Triangles(4, 1, 2, 4, 1, 4, 5, 4),
	gsSP2Triangles(5, 7, 6, 5, 7, 5, 4, 5),
	gsSPEndDisplayList(),
};

static Vtx wall_no_north_vtx[] = {
	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},

	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},

	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_no_north_dl[] = {
	gsSPVertex(&(wall_no_north_vtx[0]), 8, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSP2Triangles(3, 5, 4, 3, 5, 3, 0, 3),
	gsSP2Triangles(6, 1, 2, 6, 1, 6, 7, 6),
	gsSPEndDisplayList(),
};

static Vtx wall_no_south_vtx[] = {
	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},

	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_no_south_dl[] = {
	gsSPVertex(&(wall_no_south_vtx[0]), 8, 0),
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),
	gsSP2Triangles(4, 6, 5, 4, 6, 4, 7, 4),
	gsSP2Triangles(7, 1, 2, 7, 1, 7, 4, 7),
	gsSPEndDisplayList(),
};

// four sides
static Vtx wall_square_vtx[] = {
	{-0, -0, -0, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, -0, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, 10, -0, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, 10, -0, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},

	{-0, 10, 10, 0, 32 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{-0, -0, 10, 0, 32 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},

	{10, 10, 10, 0, 00 << 10, 32 << 10, 0xff, 0xff, 0xff, 0xff},
	{10, -0, 10, 0, 00 << 10, 00 << 10, 0xff, 0xff, 0xff, 0xff},
};
Gfx wall_square_dl[] = {
	gsSPVertex(&(wall_square_vtx[0]), 8, 0), /**/
	gsSP2Triangles(0, 2, 1, 0, 2, 0, 3, 0),	 /**/
	gsSP2Triangles(3, 5, 4, 3, 5, 3, 0, 3),	 /**/
	gsSP2Triangles(6, 1, 2, 6, 1, 6, 7, 6),	 /**/
	gsSP2Triangles(7, 4, 5, 7, 4, 7, 6, 7),	 /**/
	gsSPEndDisplayList(),					 /**/
};
