//Alex Hamilton
//Milkman Credits
#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
using namespace std;

//extern void show_credits();
//extern void render(void);
//extern checkKeys(XEvent *e);


void show_credits(Rect r, int x, int y);
{
	ggprint8b(&r, x, y, "Alex Hamilton"); 
}
