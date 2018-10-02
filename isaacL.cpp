#include "fonts.h"
#include <GL/glx.h>

void show_isaac(Rect r, int x, int y) 
{
    r.bot = 600 - 40;
	r.left = 10;
	r.center = 0;
	
	ggprint8b(&r, x, y, "Isaac Lara");
}

