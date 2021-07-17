#pragma once

#include <ultra64.h>

typedef f32 Mat4[4][4];

// X, Y, Z, where Y is up
typedef struct Vec3f {
	f32 x;
	f32 y;
	f32 z;
} Vec3f;

// X, Y, Z, where Y is up
typedef struct {
	u32 x;
	u32 y;
	u32 z;
} Vec3;