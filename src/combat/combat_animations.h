#pragma once

#define EXTERN_ANIM_CLASS(class, gender)                                                           \
	extern unsigned short *spr_##class##_##gender##_idle[];                                        \
	extern unsigned short *spr_##class##_##gender##_attack_1[];                                    \
	extern unsigned short *spr_##class##_##gender##_attack_2[];                                    \
	extern unsigned short *spr_##class##_##gender##_attack_3[];                                    \
	extern unsigned short *spr_##class##_##gender##_dead[];                                        \
	extern unsigned short *spr_##class##_##gender##_hit[];                                         \
	extern unsigned short *spr_##class##_##gender##_win[];

EXTERN_ANIM_CLASS(warrior, male)
EXTERN_ANIM_CLASS(warrior, female)
EXTERN_ANIM_CLASS(wizard, male)
EXTERN_ANIM_CLASS(wizard, female)
EXTERN_ANIM_CLASS(cleric, male)
EXTERN_ANIM_CLASS(cleric, female)
EXTERN_ANIM_CLASS(thief, male)
EXTERN_ANIM_CLASS(thief, female)

extern unsigned short *spr_goblin_boss_idle[];
extern unsigned short *spr_goblin_minion1_idle[];
extern unsigned short *spr_goblin_minion2_idle[];
