// Author: Justin Selsor (Sep. 25, 2018)
// For the Milkman Project. Software Enginering.

#include <GL/glx.h>
#include "fonts.h"
void show_credits_justin(Rect r, int x, int y)
{
    r.bot = 600 - 50;
	r.left = 10;
	r.center = 0;

    ggprint8b(&r, x, y, "Justin S.");
}
