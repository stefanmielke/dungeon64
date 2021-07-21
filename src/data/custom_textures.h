#pragma once

static Gfx spr_ground_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_ground[] = {
#include "../res/gfx/textures/ground.gen.h"
};

static Gfx spr_plant_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_plant[] = {
#include "../res/gfx/textures/plant.gen.h"
};

static Gfx spr_wall_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_wall[] = {
#include "../res/gfx/textures/wall.gen.h"
};

static Gfx spr_wall_upstairs_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_wall_upstairs[] = {
#include "../res/gfx/textures/wall_upstairs.gen.h"
};

static Gfx spr_wall_exit_dummy_aligner[] = {gsSPEndDisplayList()};
unsigned short spr_wall_exit[] = {
#include "../res/gfx/textures/wall_exit.gen.h"
};
