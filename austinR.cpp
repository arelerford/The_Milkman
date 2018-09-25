#include "fonts.h"
using namespace std;

extern void render();
extern void initOpengl();
extern void init();
extern void physics();
extern void render();
void show_austin();

void show_austin(int x, int y, Rect r) 
{
    ggprint8b(&r, 16, 0x00ff0000, "Austin Relerford");


}
