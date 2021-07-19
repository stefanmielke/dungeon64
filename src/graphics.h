#pragma once

#include <ultra64.h>
#include "types.h"

#define MAX_OBJECTS 200
#define MAX_BILLBOARDS 100
#define GLIST_LEN 2048

/*
 * Layout of dynamic data.
 *
 * This structure holds the things which change per frame. It is advantageous
 * to keep dynamic data together so that we may selectively write back dirty
 * data cache lines to DRAM prior to processing by the RCP.
 *
 */
typedef struct Dynamic {
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

typedef struct GameData {
	OSContPad **pad;
} GameData;

typedef struct RenderData {
	OSTask *theadp;
	Dynamic *dynamicp;
	u16 perspnorm;
	Mat4 modmat;
	Mat4 m1, m2;
	Mat4 allmat;
} RenderData;