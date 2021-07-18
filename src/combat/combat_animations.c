#include "combat_animations.h"

#include "../data/texture.h"

#define ANIM_1_FRAMES(class, gender, name)                                                         \
	unsigned short *spr_##class##_##gender##_##name[] = {                                          \
		spr_##class##_##gender##_##name##1,                                                        \
	};
#define ANIM_2_FRAMES(class, gender, name)                                                         \
	unsigned short *spr_##class##_##gender##_##name[] = {                                          \
		spr_##class##_##gender##_##name##1,                                                        \
		spr_##class##_##gender##_##name##2,                                                        \
	};
#define ANIM_3_FRAMES(class, gender, name)                                                         \
	unsigned short *spr_##class##_##gender##_##name[] = {                                          \
		spr_##class##_##gender##_##name##1,                                                        \
		spr_##class##_##gender##_##name##2,                                                        \
		spr_##class##_##gender##_##name##3,                                                        \
	};
#define ANIM_4_FRAMES(class, gender, name)                                                         \
	unsigned short *spr_##class##_##gender##_##name[] = {                                          \
		spr_##class##_##gender##_##name##1,                                                        \
		spr_##class##_##gender##_##name##2,                                                        \
		spr_##class##_##gender##_##name##3,                                                        \
		spr_##class##_##gender##_##name##4,                                                        \
	};

#define ANIM_CLASS(class, gender)                                                                  \
	ANIM_3_FRAMES(class, gender, idle)                                                             \
	ANIM_3_FRAMES(class, gender, attack_1)                                                         \
	ANIM_3_FRAMES(class, gender, attack_2)                                                         \
	ANIM_3_FRAMES(class, gender, attack_3)                                                         \
	ANIM_1_FRAMES(class, gender, dead)                                                             \
	ANIM_1_FRAMES(class, gender, hit)                                                              \
	ANIM_4_FRAMES(class, gender, win)

ANIM_CLASS(warrior, male)
ANIM_CLASS(warrior, female)
ANIM_CLASS(wizard, male)
ANIM_CLASS(wizard, female)
ANIM_CLASS(cleric, male)
ANIM_CLASS(cleric, female)
ANIM_CLASS(thief, male)
ANIM_CLASS(thief, female)

ANIM_1_FRAMES(goblin, boss, idle)
ANIM_1_FRAMES(goblin, minion1, idle)
ANIM_1_FRAMES(goblin, minion2, idle)
