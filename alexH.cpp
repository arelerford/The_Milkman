//Alex Hamilton
//Last modified: October 7th
//Purpose: Show Milkman Alex Credits and Picture
#include <iostream>
#include "fonts.h"
#include <GL/glx.h>
#include <math.h>
using namespace std;



void show_AlexCredits(Rect *r)
{
        ggprint16(r, 150, 0, "Alex Hamilton"); 
}

void show_AlexPicture(int x, int y, GLuint texid)
{
	//show Alex picture and animation (dog)
	static float angle = 0.0f;
	static float angle2 = 0.0f;
	static float fx = 0.0f;
	angle += 0.2;
	fx = sin(angle);
	//fx += sin(angle) * 10.0f;
	angle += 0.2f;	
        glColor3ub(255, 255, 255);
        int wid=40;
        glPushMatrix();
        glTranslatef(x + (int)(fx*20.0), y, 0);
	glRotatef(angle2, 0.0f, 0.0f, 1.0f);
	angle2 += 50;
        glBindTexture(GL_TEXTURE_2D, texid);
        glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2i(-wid, -wid);
                glTexCoord2f(0.0f, 0.0f); glVertex2i(-wid, wid);
                glTexCoord2f(1.0f, 0.0f); glVertex2i(wid, wid);
                glTexCoord2f(1.0f, 1.0f); glVertex2i(wid, -wid);
        glEnd();
        glPopMatrix();
	
}
