/*
 * Author: Isaac Lara
 *   File: isaacL.cpp
 */

#include "fonts.h"
#include <GL/glx.h>
#include <stdio.h>      //For debugging w/ printf()
#include <stdlib.h>     //For rand() and seed()
#include <cassert>

#define NUM_CLOUDS 20
#define SIZE_CLOUDS 150

void show_isaac_name(Rect *r) 
{   
    ggprint16(r, 150, 0, "Isaac Lara");
}

void show_isaac_pic(int x, int y, GLuint texid)
{
    int wid = 40;
    glColor3f (1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(x, y, 0);    
    glBindTexture(GL_TEXTURE_2D, texid);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i (-wid, -wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i (wid, -wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i (wid, wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i (-wid, wid);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

class Cloud {
    private:
        // xpos and ypos refers to bottom left corner of image
        int xpos;
        int ypos;
        int dx = rand() % 2 + 3;    // Horizontal Speed
        int dy = 2;                 // Vertical Speed
        int size = SIZE_CLOUDS;
        bool init;
        GLuint texid;

    public:

        Cloud() {
            init = true;
        }

        void draw() {
            glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glTranslatef(xpos, ypos, 0);    
            glBindTexture(GL_TEXTURE_2D, texid);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glBegin(GL_QUADS);
                glTexCoord2f(0.0f, 1.0f); glVertex2i (   0,    0);
                glTexCoord2f(1.0f, 1.0f); glVertex2i (size,    0);
                glTexCoord2f(1.0f, 0.0f); glVertex2i (size, size);
                glTexCoord2f(0.0f, 0.0f); glVertex2i (   0, size);
            glEnd();
            glPopMatrix();
            glBindTexture(GL_TEXTURE_2D, 0);
            glDisable(GL_ALPHA_TEST);

        }
        
        void setAttr(GLuint id, int x, int y) {
            texid = id;
            xpos = x * size;
            ypos = y * size;
            init = false;
        }

        void refreshX(GLuint randTex, int yres) {
            texid = randTex;
            xpos = -size;
            ypos = size * (rand() % (yres/size) );
        }

        void refreshY(GLuint randTex, int xres) {
            texid = randTex;
            xpos = size * (rand() % (xres/size) );
            ypos = -size;
        }

        void move() {
            ypos += dy;
            xpos += dx;
        }

        int getX() {return xpos;}

        int getY() {return ypos;}

        bool needInit() {return init;}
};

Cloud cloud[NUM_CLOUDS];
void showClouds(int size, GLuint texid[], int xres, int yres)
{   
    int    num_tex = size;

    for (int i = 0; i < NUM_CLOUDS; i++) {
        if (cloud[i].needInit() ) {
            GLuint randTex = texid[rand() % num_tex];
            GLuint randX = rand() % (xres / SIZE_CLOUDS);
            GLuint randY = i % (xres / SIZE_CLOUDS);
            cloud[i].setAttr(randTex,randX, randY);
        }

        int cloudX = cloud[i].getX();
        int cloudY = cloud[i].getY();

        if (cloudX > xres) {
            GLuint randTex = texid[rand() % num_tex];
            cloud[i].refreshX(randTex, yres);
        }

        if (cloudY > yres) {
            GLuint randTex = texid[rand() % num_tex];
            cloud[i].refreshY(randTex, xres);
        }

        cloud[i].draw();
        cloud[i].move();
    }  
}

void showText(GLuint text_tex[], int xres, int yres) {
    
    static float width = 14;
    static float height = 2;
    GLuint title = text_tex[0];
    int max_width = 700;

    glColor3f (1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(xres/2, yres*(5.0 / 6.0), 0);    
    glBindTexture(GL_TEXTURE_2D, title);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i (-width/2, -height/2);
        glTexCoord2f(1.0f, 1.0f); glVertex2i ( width/2, -height/2);
        glTexCoord2f(1.0f, 0.0f); glVertex2i ( width/2,  height/2);
        glTexCoord2f(0.0f, 0.0f); glVertex2i (-width/2,  height/2);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);

    float rate = 1.4;
    if (width < max_width) {
         width *= rate;
        height *= rate;
    } else {
         width = max_width;
        height = width / 7;
    }
}