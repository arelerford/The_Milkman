//Alex Hamilton
//Milkman Credits
#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
using namespace std;

//extern void show_credits();
//extern void render(void);
//extern checkKeys(XEvent *e);


void show_credits(Rect r, int x, int y)
{
    r.bot = 600 - 30;
	r.left = 10;
	r.center = 0;

	ggprint8b(&r, x, y, "Alex Hamilton"); 
}
