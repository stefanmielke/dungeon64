#include <PR/sp.h>

void font_init(Gfx **glistp) {
	Gfx *gxp = *glistp;
	spInit(&gxp);
	*glistp = gxp;
}
