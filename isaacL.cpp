/*
 * Author: Isaac Lara
 *   File: isaacL.cpp
 */

#include "fonts.h"
#include <GL/glx.h>
#include <stdio.h>      //For debugging w/ printf()
#include <stdlib.h>     //For rand() and seed()
#include "isaacL.h"

extern Screens screen;

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

void Cloud::draw()
{
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
};

void Cloud::setAttr(GLuint id, int x, int y)
{
    texid = id;
    xpos = x * size;
    ypos = y * size;
    init = false;
};

void Cloud::refreshX(GLuint randTex, int yres) 
{
    texid = randTex;
    xpos = -size;
    ypos = size * (rand() % (yres/size) );
};

void Cloud::refreshY(GLuint randTex, int xres)
{
    texid = randTex;
    xpos = size * (rand() % (xres/size) );
    ypos = -size;
};

void Cloud::move()
{
    ypos += dy;
    xpos += dx;
};

Cloud cloud[NUM_CLOUDS];

void showClouds(GLuint texid[], int size, int xres, int yres)
{
    int num_tex = size;

    for (int i = 0; i < NUM_CLOUDS; i++) {
        if (cloud[i].needInit() ) {
            GLuint randTex = texid[rand() % num_tex];
            GLuint randX = rand() % (xres / SIZE_CLOUDS);
            GLuint randY = i % (xres / SIZE_CLOUDS );
            cloud[i].setAttr(randTex, randX, randY);
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

bool showTitle(GLuint title_tex, int xres, int yres)
{
    static float width = 14;
    static float height = 2;
    GLuint title = title_tex;
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

    float rate = 1.2;

    static int d_width = 200;
    if (width < max_width + d_width) {
         width *= rate;
        height *= rate;
        d_width = 0;
        return false;
    } else {
         width = max_width;
        height = width / 7;
        return true;
    }
}


Text::Text() 
{
    this->width = 122;
    this->height = width / 7;
}

void Text::setAttr(GLuint id, int x, int y)
{
    texid = id;
     xpos = x;
     ypos = y;
}

void Text::draw()
{
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(xpos, ypos, 0);
    glBindTexture(GL_TEXTURE_2D, texid);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i (-width/2,-height/2);
        glTexCoord2f(1.0f, 1.0f); glVertex2i ( width/2,-height/2);
        glTexCoord2f(1.0f, 0.0f); glVertex2i ( width/2, height/2);
        glTexCoord2f(0.0f, 0.0f); glVertex2i (-width/2, height/2);
    glEnd();
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_ALPHA_TEST);
}

void Text::setActive(bool active)
{
    if (active) {
         width = 168;
        height = width/7;
    } else {
         width = 112;
        height = width/7;            
    }
}

void Text::setLoc(int x, int y)
{
    xpos = x;
    ypos = y;
}

Text words[4];

void showText(GLuint text_tex[], int opt[], int xres, int yres)
{
    for (int i = 0; i < 4; i++) {
        words[i].setAttr(text_tex[i], xres/2, yres/3-(i*50) );
        words[i].setActive(opt[i]);
        words[i].draw();
    }
};

Start::Start()
{
    cloud_tex = new GLuint[cloud_num];
     text_tex = new GLuint[text_num];

    // Generate textures and bind images for clouds
    for (int i = 0; i < cloud_num; i++) {
        glGenTextures(1, &cloud_tex[i]);
        glBindTexture(GL_TEXTURE_2D, cloud_tex[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        unsigned char *new_img = buildAlphaData(&cloud_img[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cloud_img[i].width, cloud_img[i].height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, new_img);
        free(new_img);
    }

    // Generate textures and bind images for text
    for (int i = 0; i < text_num; i++) {
        glGenTextures(1, &text_tex[i]);
        glBindTexture(GL_TEXTURE_2D, text_tex[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        unsigned char *new_img = buildAlphaData(&text_img[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, text_img[i].width, text_img[i].height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, new_img);
        free(new_img);
    }

    // Generate textures and bind image for title
    glGenTextures(1, &title_tex);
    glBindTexture(GL_TEXTURE_2D, title_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *new_img = buildAlphaData(&title_img);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, title_img.width, title_img.height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, new_img);
    free(new_img);
};

void Start::Display(Global *obj)
{
    if (display) {

        // Clear the screen to specific color
        float   red =  78.0 / 255;
        float green = 173.0 / 255;
        float  blue = 245.0 / 255;

        glClearColor(red, green, blue, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        
        showClouds(cloud_tex, cloud_num, obj->xres, obj->yres);
        bool done_title = showTitle(title_tex, obj->xres, obj->yres);
        if (done_title) {
            showText(text_tex, opt, obj->xres, obj->yres);
        }
    }
};

int Start::checkKey(int key)
{
    switch (key) {
        case XK_Escape:
            return 1;

        case XK_Up:
            for (int i = 0; i < 4; i++) {
                if (opt[i]) {
                    opt[(i+3) % 4] = 1;
                    opt[i] = 0;
                    break;
                }
            }
            break;

        case XK_Down:
            for (int i = 0; i < 4; i++) {
                if (opt[i]) {
                    opt[(i+1) % 4] = 1;
                    opt[i] = 0;
                    break;
                }
            }
        break;

        case XK_Return:
            testFunct(screen, opt, opt_size);
        break;

        case XK_space:
            testFunct(screen, opt, opt_size);
        break;

    }
    return 0;
};

Credits::Credits()
{
    team_tex = new GLuint[team_num];

    //Generate textures and bind images for team photos
    for (int i = 0; i < team_num; i++) {
        glGenTextures(1, &team_tex[i]);
        glBindTexture(GL_TEXTURE_2D, team_tex[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, team_img[i].width, team_img[i].height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, team_img[i].data);
    }
};

void Credits::Display(Global *obj)
{
    if (display) {
        //Clear the screen
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // External Files
        extern void show_credits_justin (Rect*);
        extern void show_austin(Rect*);
        extern void show_isaac_name(Rect*);
        extern void show_AlexCredits (Rect*);

        extern void show_isaac_pic(int, int, GLuint);        
        extern void show_justin_image(int, int, GLuint);
        extern void show_AlexPicture(int, int, GLuint);
        extern void show_austin_pic(int, int, GLuint);

        Rect r;

        r.bot = obj->yres - 80;
        r.left = obj->xres / 3;
        int pic_column = 2 * r.left;

        show_AlexPicture(pic_column, r.bot + 10, team_tex[0]);
        show_AlexCredits(&r);

        show_isaac_pic(pic_column, r.bot + 10, team_tex[0]);    
        show_isaac_name(&r);

        show_austin_pic(pic_column, r.bot + 10, team_tex[0]);
        show_austin(&r);

        show_justin_image(pic_column, r.bot + 10, team_tex[1]);
        show_credits_justin(&r);
    }
};

int Credits::checkKey(int key)
{   
    switch (key) {
        case XK_Escape:
        {
            int tmp[4] = {1, 0, 0, 0};
            testFunct(screen, tmp, 4);
        }
        break;
    }
    return 0;
};



Level_1::Level_1()
{
	frame = 0;
	
    //Generate textures
	glGenTextures(1, &background_tex);
	glBindTexture(GL_TEXTURE_2D, background_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, background_img.width, background_img.height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, background_img.data);
	
	glGenTextures(1, &foreground_tex);
	glBindTexture(GL_TEXTURE_2D, foreground_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	unsigned char *new_img = buildAlphaData(&foreground_img);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, foreground_img.width, foreground_img.height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, new_img);
	free(new_img);
};

void Level_1::Display(Global *obj)
{
    if (display) {
        //Clear the screen
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
		// Draws Background
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(0, 0, 0);
		glBindTexture(GL_TEXTURE_2D, background_tex);
		glBegin(GL_QUADS);
			glTexCoord2f(0.2*frame      , 1);  glVertex2i(        0,         0);
			glTexCoord2f(0.2*frame + 0.2, 1);  glVertex2i(obj->xres,         0);
			glTexCoord2f(0.2*frame + 0.2, 0);  glVertex2i(obj->xres, obj->yres);
			glTexCoord2f(0.2*frame      , 0);  glVertex2i(        0, obj->yres);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		
		
		
		// Draws foreground
		glColor3f(1.0f, 1.0f, 1.0f);
		glPushMatrix();
		glTranslatef(0, 0, 0);
		glBindTexture(GL_TEXTURE_2D, foreground_tex);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glBegin(GL_QUADS);
			glTexCoord2f(0.2*frame      , 1);  glVertex2i(        0,         0);
			glTexCoord2f(0.2*frame + 0.2, 1);  glVertex2i(obj->xres,         0);
			glTexCoord2f(0.2*frame + 0.2, 0);  glVertex2i(obj->xres, obj->yres);
			glTexCoord2f(0.2*frame      , 0);  glVertex2i(        0, obj->yres);
		glEnd();
		glPopMatrix();
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
    }
};

int Level_1::checkKey(int key)
{
    switch (key) {            
        case XK_Escape:
        {
            int tmp[4] = {1, 0, 0, 0};
            testFunct(screen, tmp, 4);
        }
        break;

        case XK_Left:

        break;

        case XK_Right:

        break;
    }
    return 0;
};

///alex's code will move later///
Controls::Controls()
{

}
void Controls::Display()
{
    if (display) {
        //Clear the screen
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

    }
};

int Controls::checkKey(int key)
{
    switch (key) {
        case XK_Escape:
        {
            int tmp[4] = {1, 0, 0, 0 };
            testFunct(screen, tmp, 4);
        }
        break;
    }
    return 0;
};
//////////////////////////////////////////////
void testFunct(Screens screen, int opt[], int size)
{
    for (int i = 0; i < size; i++) {
        // if (opt[i]) *screen.displays[i] = true;
        if (opt[i]) *screen.displays[i] = !(*screen.displays[i]);
        else *screen.displays[i] = false;
    }
}
