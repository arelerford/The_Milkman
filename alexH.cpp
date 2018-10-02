//Alex Hamilton
//Milkman Credits
#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
#include <math.h>
using namespace std;

//extern void show_credits();
//extern void render(void);
//extern checkKeys(XEvent *e);


void show_AlexCredits(Rect r, int x, int y)
{
        r.bot = 600 - 30;
	r.left = 10;
	r.center = 0;

	ggprint8b(&r, x, y, "Alex Hamilton"); 
}

void show_AlexPicture(int x, int y, GLuint texid)
{
//show Alex picture (dog)
	static float angle = 0.0f;
	float fx = (float)x;
	float fy = (float)y;
	fx += sin(angle) * 10.0f;
	fy += sin(angle) * 10.0f;
	angle += 0.2f;	
        glColor3ub(255, 255, 255);
        int wid=40;
        glPushMatrix();
        glTranslatef(x, y, 0);
        glBindTexture(GL_TEXTURE_2D, texid);
        glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
                glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
        glEnd();
        glPopMatrix();
        ////////////////
	
}
