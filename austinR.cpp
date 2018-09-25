#include "fonts.h"
using namespace std;

extern void render();
extern void initOpengl();
extern void init();
extern void physics();
extern void render();
void show_austin();

void show_austin(Rect r, int x, int y) 
{
    r.bot = 600 - 20;
	r.left = 10;
	r.center = 0;

    ggprint8b(&r, x, y, "Austin Relerford");
}
