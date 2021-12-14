#pragma once

// enemies
extern unsigned short spr_goblin_boss_idle1[];
extern unsigned short spr_goblin_minion1_idle1[];
extern unsigned short spr_goblin_minion2_idle1[];

// player
#define CLASS_ANIMS(class, gender)                                                                 \
	extern unsigned short spr_##class##_##gender##_idle1[];                                        \
	extern unsigned short spr_##class##_##gender##_idle2[];                                        \
	extern unsigned short spr_##class##_##gender##_idle3[];                                        \
	extern unsigned short spr_##class##_##gender##_attack_11[];                                    \
	extern unsigned short spr_##class##_##gender##_attack_12[];                                    \
	extern unsigned short spr_##class##_##gender##_attack_13[];                                    \
	extern unsigned short spr_##class##_##gender##_attack_21[];                                    \
	extern unsigned short spr_##class##_##gender##_attack_22[];                                    \
	extern unsigned short spr_##class##_##gender##_attack_23[];                                    \
	extern unsigned short spr_##class##_##gender##_attack_31[];                                    \
	extern unsigned short spr_##class##_##gender##_attack_32[];                                    \
	extern unsigned short spr_##class##_##gender##_attack_33[];                                    \
	extern unsigned short spr_##class##_##gender##_dead1[];                                        \
	extern unsigned short spr_##class##_##gender##_hit1[];                                         \
	extern unsigned short spr_##class##_##gender##_win1[];                                         \
	extern unsigned short spr_##class##_##gender##_win2[];                                         \
	extern unsigned short spr_##class##_##gender##_win3[];                                         \
	extern unsigned short spr_##class##_##gender##_win4[];

CLASS_ANIMS(warrior, male)
CLASS_ANIMS(warrior, female)
CLASS_ANIMS(wizard, male)
CLASS_ANIMS(wizard, female)
CLASS_ANIMS(cleric, male)
CLASS_ANIMS(cleric, female)
CLASS_ANIMS(thief, male)
CLASS_ANIMS(thief, female)

// textures
extern unsigned short spr_plant[];
extern unsigned short spr_ground[];
extern unsigned short spr_wall[];
extern unsigned short spr_wall_upstairs[];
extern unsigned short spr_wall_exit[];
extern unsigned short spr_ground_2[];
extern unsigned short spr_wall_2[];
extern unsigned short spr_wall_upstairs_2[];
extern unsigned short spr_wall_exit_2[];

// ui
extern unsigned short spr_ui[];
extern unsigned short spr_font[];
extern unsigned short spr_font_palette_white[];
extern unsigned short spr_font_palette_blue[];
extern unsigned short spr_font_palette_green[];
extern unsigned short spr_font_palette_grey[];
extern unsigned short spr_font_palette_red[];
extern unsigned short spr_font_palette_yellow[];
