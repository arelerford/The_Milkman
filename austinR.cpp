
//Author: Austin Relerford
//Milkman credits/picture
//

#include "fonts.h"
#include <GL/glx.h>
#include <cstdlib>
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
void shoot() {
   struct timespec bt;
   clock_gettime(CLOCK_REALTIME, &bt);
   double ts = timers.timeDiff(&g.bulletTimer, &bt);
   if (ts > 0.1) {
       timers.timeCopy(&g.bulletTimer, &bt);
       //shoot
       if (g.nbullets < MAX_BULLETS) {
           Bullet *b = &g.barr[g.nbullets];
           timers.timeCopy(&b->time, &bt);
           b->pos[0] = player.x;
           b->pos[1] = player.y;
           b->vel[0] = 30;
           b->vel[1] = 0;
           b->color[0] = 1.0f;
           b->color[1] = 1.0f;
           b->color[2] = 1.0f;
           ++g.nbullets;
           }

    }
};
void render_bullets()
{
    Bullet *b = &g.barr[0];
    for (int i=0; i<g.nbullets; i++) {
        glColor3f(1.0, 1.0, 1.0);
            glPushMatrix();
        glTranslatef(b->pos[0], b->pos[1], 0);
        glBindTexture(GL_TEXTURE_2D,b->texid);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i (   0,    0);
           glTexCoord2f(1.0f, 1.0f); glVertex2i (150,    0);
           glTexCoord2f(1.0f, 0.0f); glVertex2i (150, 150);
           glTexCoord2f(0.0f, 0.0f); glVertex2i (   0, 150);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D,0);
        glDisable(GL_ALPHA_TEST);

                glBegin(GL_POINTS);
            glVertex2f(b->pos[0],      b->pos[1]);
            glVertex2f(b->pos[0]-1.0f, b->pos[1]);
            glVertex2f(b->pos[0]+1.0f, b->pos[1]);
            glVertex2f(b->pos[0],      b->pos[1]-1.0f);
            glVertex2f(b->pos[0],      b->pos[1]+1.0f);
            glColor3f(0.8, 0.8, 0.8);
            glVertex2f(b->pos[0]-1.0f, b->pos[1]-1.0f);
            glVertex2f(b->pos[0]-1.0f, b->pos[1]+1.0f);
            glVertex2f(b->pos[0]+1.0f, b->pos[1]-1.0f);
            glVertex2f(b->pos[0]+1.0f, b->pos[1]+1.0f);
        glEnd();
        ++b;
    }
};
void Spawn_powerup()
{
  Powerup p;
  p.type = (rand() % 2);
  p.pos[0] = (rand() % gl.xres)
  p.pos[1] = (rand() % (gl.yres-100))
};
void check_powerup()
{
  if (player->xpos == p.pos[0] && player->ypos == p.pos[1]){
    p.active = true;
    if (p.type == 0) {
      SpeedUp();
    }
    if (p.type == 1) {
      DmgUp();
    }
  }

};
