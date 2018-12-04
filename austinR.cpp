
//Author: Austin Relerford
//Milkman credits/picture
//

#include "fonts.h"
#include <GL/glx.h>
using namespace std;

extern void render();
extern void initOpengl();
extern void init();
extern void physics();
extern void render();
void show_austin();

void show_austin(Rect *r)
{
    ggprint16(r, 150, 0, "Austin Relerford");
}

void show_austin_pic (int x, int y, GLuint textid)
{
    glColor3f (1.0f, 1.0f, 1.0f);

    int wid = 60;

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
