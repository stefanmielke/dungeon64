#pragma once

#include <ultra64.h>

#include "screen_config.h"
#include "../controller.h"
#include "../game.h"

// macros
#define AXISTHRESH 30
#define PADTHRESH(num)                                                                             \
	((num > AXISTHRESH) ? num - AXISTHRESH : (num < -AXISTHRESH) ? num + AXISTHRESH : 0)
#define IS_BUTTON_PRESSED(btn) (gd.pad[0]->button & btn)
#define MOVEMENT_SPEED 400
