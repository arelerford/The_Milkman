#include "fonts.h"
#include <GL/glx.h>

void show_isaac_name(Rect *r) 
{	
	ggprint16(r, 150, 0, "Isaac Lara");
}

void show_isaac_pic(int x, int y, GLuint textid)
{
    glColor3f (1.0f, 1.0f, 1.0f);
    
    int wid = 40;

    glPushMatrix();

    glTranslatef(x, y, 0);
    
    glBindTexture(GL_TEXTURE_2D, textid);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i (-wid, -wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i (wid, -wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i (wid, wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i (-wid, wid);
    glEnd();

    glPopMatrix();
}