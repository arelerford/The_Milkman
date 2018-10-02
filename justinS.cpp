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

void show_justin_image (int x, int y, GLuint textid)
{
    glColor3f (1.0f, 1.0f, 1.0f);
    
    int wid = 40;

    glPushMatrix();

    glTranslatef(x, y, 0);
    
    glBindTexture(GL_TEXTURE_2D, textid);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i (-wid, -wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i (-wid, wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i (wid, wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i (-wid, wid);
    glEnd();

    glPopMatrix();
}
