#include "combat_helper.h"

#include <nustd/math.h>

#include "../static.h"
#include "../data/texture.h"
#include "../math.h"
#include "../objects/billboards.h"
#include "../objects/combat/enemies.h"
#include "../objects/combat/player.h"

void combat_render(Gfx **glistp, Dynamic *dynamicp, int pov_x, int pov_z) {
	int obj_count;		 /* count of used objects on current frame */
	int billboard_count; /* count of used billboards on current frame */

	static float frame_counter = 0;
	frame_counter += 0.09f;

	// ground
	gSPDisplayList((*glistp)++, ground_texture_setup_dl);
	gSPTexture((*glistp)++, 1024 * 100, 1024 * 100, 0, G_TX_RENDERTILE, G_ON);
	gDPLoadTextureBlock((*glistp)++, spr_ground, G_IM_FMT_RGBA, G_IM_SIZ_16b, 32, 32, 0, G_TX_WRAP,
						G_TX_WRAP, 5, 5, G_TX_NOLOD, G_TX_NOLOD);

	gDPPipeSync((*glistp)++);
	guTranslate(&dynamicp->object_position[obj_count], -50, 0, -50);
	gSPMatrix((*glistp)++, OS_K0_TO_PHYSICAL(&(dynamicp->object_position[obj_count])),
			  G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
	gSPDisplayList((*glistp)++, combat_ground_dl);
	obj_count++;

	// billboard setup
	gSPDisplayList((*glistp)++, billboard_texture_setup_dl);
	gSPTexture((*glistp)++, 2048, 2048, 0, G_TX_RENDERTILE, G_ON);

	static unsigned short *spr_warrior_back_idle[] = {
		spr_warrior_back_idle1,
		spr_warrior_back_idle2,
		spr_warrior_back_idle3,
		spr_warrior_back_idle2,
	};
	static unsigned short *spr_blue_dragon_idle[] = {
		spr_blue_dragon_idle1,
		spr_blue_dragon_idle2,
		spr_blue_dragon_idle3,
		spr_blue_dragon_idle2,
	};
	for (int i = 0; i < 4; ++i) {
		DRAW_BLUE_DRAGON(-3 - (3 * i), -5 + i * 3, pov_x, pov_z, (int)frame_counter);
	}

	for (int i = 0; i < 4; ++i) {
		DRAW_WARRIOR(3 + (3 * i), 5 - i * 3, pov_x, pov_z, (int)frame_counter);
	}

	// render text
	font_init(glistp);
	font_set_transparent(1);
	font_set_scale(1.0, 1.0);
	font_set_win(200, 1);
	FONTCOLM(FONT_COL);
	char position[100];
	sprintf(position, "Combat");
	SHOWFONT(glistp, position, 20, 210);
	font_finish(glistp);
}
