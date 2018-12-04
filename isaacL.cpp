/*
 * Author: Isaac Lara
 *   File: isaacL.cpp
 */

#include "isaacL.h"

extern Screens screen;

void show_isaac_name(Rect *r)
{
	ggprint16(r, 150, 0, "Isaac Lara");
}

void show_isaac_pic(int x, int y, GLuint texid)
{
    int wid = 40;
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glBindTexture(GL_TEXTURE_2D, texid);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i (-wid, -wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i ( wid, -wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i ( wid,  wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i (-wid,  wid);
    glEnd();

    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
}

Cloud::Cloud()
{
      dx = rand() % 3 + 2;
      dy = 1;
    init = true;
    size = SIZE_CLOUDS;
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
}

void Cloud::setAttr(GLuint id, int x, int y)
{
	texid = id;
	 xpos = x * size;
	 ypos = y * size;
	 init = false;
}

void Cloud::refreshX(GLuint randTex, int yres) 
{
	texid = randTex;
	 xpos = -size;
	 ypos =  size * ( rand() % (yres / size) );
}

void Cloud::refreshY(GLuint randTex, int xres)
{
	texid = randTex;
	 xpos =  size * ( rand() % (xres / size) );
	 ypos = -size;
}

void Cloud::move()
{
	ypos += dy;
	xpos += dx;
}

Text::Text() 
{
	 width = 122;
	height = width / 7;
    active = false;
}

void Text::setTex(GLuint id)
{
	texid = id;
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
		 //width = 168;
		 width = 168 * gl.yres/500;
		height = width / 7;
	} else {
		 //width = 112;
		 width = 112 * gl.yres/500;
		height = width / 7;            
	}
}

void Text::setLoc(int x, int y)
{
    xpos = x;
    ypos = y;
}

Start::Start()
{
       clouds = new Cloud[NUM_CLOUDS];
    cloud_num = sizeof(cloud_img) / sizeof(*cloud_img);
    cloud_tex = new GLuint[cloud_num];

         text = new Text[3];
     text_num = sizeof(text_img) / sizeof(*text_img);
     text_tex = new GLuint[text_num];

    display = true;
    restart = false;

    opt = new int[text_num];
    for (int i = 0; i < text_num; i++) {
        opt[i] = !i;
    }
    opt_size = text_num;

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
    initClouds();

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
    initText();

    // Generate textures and bind image for title
    glGenTextures(1, &title_tex);
    glBindTexture(GL_TEXTURE_2D, title_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *new_img = buildAlphaData(&title_img);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, title_img.width, title_img.height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, new_img);
    free(new_img);
}

void Start::showClouds()
{
    for (int i = 0; i < NUM_CLOUDS; i++) {        
        int cloudX = clouds[i].getX();
        int cloudY = clouds[i].getY();

        if (cloudX > gl.xres) {
            GLuint randTex = cloud_tex[rand() % cloud_num];
            clouds[i].refreshX(randTex, gl.yres);
        }
        if (cloudY > gl.yres) {
            GLuint randTex = cloud_tex[rand() % cloud_num];
            clouds[i].refreshY(randTex, gl.xres);
        }
        clouds[i].draw();
        clouds[i].move();
    }
}

void Start::initClouds()
{
    for (int i = 0; i < NUM_CLOUDS; i++) {
        int randX = rand() % (gl.xres / SIZE_CLOUDS);
        int randY = i % (gl.xres / SIZE_CLOUDS );
        GLuint randTex = cloud_tex[rand() % cloud_num];
        clouds[i].setAttr(randTex, randX, randY);
    }
}

void Start::initText()
{
    for (int i = 0; i < opt_size; i++) {
        text[i].setTex(text_tex[i]);
    }  
}

void Start::showText()
{
    for (int i = 0; i < opt_size; i++) {
        text[i].setLoc(gl.xres/2, gl.yres/3-(i * gl.yres/9) );
        text[i].setActive(opt[i]);
        text[i].draw();
    }
}

bool Start::showTitle(bool reset)
{
    static float  width = 14;
    static float height =  2;

    int max_width = 1050 * gl.yres/ 700;

    if (reset) {
          width = 14;
         height =  2;
        restart = false;
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(gl.xres/2, gl.yres*(5.0 / 6.0), 0);    
    glBindTexture(GL_TEXTURE_2D, title_tex);
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

    // Not sure what this does
    // Dont mess with it
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

void Start::Display()
{
    if (display) {
        // Clear the screen to specific color
        float   red =  78.0 / 255;
        float green = 173.0 / 255;
        float  blue = 245.0 / 255;

        glClearColor(red, green, blue, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        
        showClouds();
        bool done_title = showTitle(restart);
        if (done_title) {
            showText();
        }
    }
}

int Start::checkKey(int key)
{
    switch (key) {
        case XK_Escape:
            return 1;

        case XK_Up:
            for (int i = 0; i < 3; i++) {
                if (opt[i]) {
                    opt[(i+2) % 3] = 1;
                    opt[i] = 0;
                    break;
                }
            }
            break;

        case XK_Down:
            for (int i = 0; i < 3; i++) {
                if (opt[i]) {
                    opt[(i+1) % 3] = 1;
                    opt[i] = 0;
                    break;
                }
            }
        break;

        case XK_Return:
            menuSelect(screen, opt, opt_size);
        break;

        case XK_space:
            menuSelect(screen, opt, opt_size);
        break;

        case XK_r:
            restart = true;
        break;
    }
    return 0;
}

Credits::Credits()
{
    team_num = sizeof(team_img) / sizeof(*team_img);
    team_tex = new GLuint[team_num];

    display = false;

    //Generate textures and bind images for team photos
    for (int i = 0; i < team_num; i++) {
        glGenTextures(1, &team_tex[i]);
        glBindTexture(GL_TEXTURE_2D, team_tex[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, team_img[i].width, team_img[i].height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, team_img[i].data);
    }
}

void Credits::Display()
{
    if (display) {
        //Clear the screen
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // External Files
        extern void show_credits_justin(Rect*);
        extern void         show_austin(Rect*);
        extern void    show_AlexCredits(Rect*);

        extern void show_justin_image(int, int, GLuint);
        extern void  show_AlexPicture(int, int, GLuint);
        extern void   show_austin_pic(int, int, GLuint);

        Rect r;

        r.left = gl.xres /  3;
        r.bot  = gl.yres - 80;
        int pic_column = 2 * r.left;
        int index = 0;

        show_AlexPicture(pic_column, r.bot + 10, team_tex[index]);
        show_AlexCredits(&r);
        // index++;

        show_isaac_pic(pic_column, r.bot + 10, team_tex[index]);    
        show_isaac_name(&r);
        index++;

        show_austin_pic(pic_column, r.bot + 10, team_tex[index]);
        show_austin(&r);
        // index++;

        show_justin_image(pic_column, r.bot + 10, team_tex[index]);
        show_credits_justin(&r);
        index++;
    }
}

int Credits::checkKey(int key)dw
{   
    switch (key) {
        case XK_Escape:
            int tmp[3] = {0, 1, 0};
            menuSelect(screen, tmp, 3);
        break;
    }
    return 0;
}

Level_1::Level_1(Player *playa)
{   
    // Variables
        display = false;
          frame = 0;    

    // Object Variables
    player  = playa;
    // Enemies
    int enemPerLev[5] = {1, 1, 2, 2, 3};
    enemies_num = 0;
    for (int i = 0; i < 5; i++) enemies_num += enemPerLev[i];
    enemies = new Enemy[enemies_num];
    int index = 0;

    for (int i = 0; i < enemies_num; i++) {
        int randX = rand() % (2*gl.xres/3) + (gl.xres/3);
        int randY = rand() % (gl.yres / 2);
        if (!enemPerLev[index]) index++; 
        enemies[i].setLoc(randX, randY, index);
        enemPerLev[index]--;
    }
    
    //Generate textures and bind images for team photos
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
}

void Level_1::Display()
{
    if (display) {
        player->display = true;
        //Clear the screen
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        //---------------------------------------------------------------------
        // Checks to see if player is far left or far right        
        if (player->xpos > gl.xres) {
            if (frame == 4) {
                display = false;
                screen.level_2->display = true;
                player->frame = 0;
                player->xpos = 0;
                player->ypos = 100;
            } else {
                frame++;
                player->frame++;
                player->xpos = 0;
                refreshBottles();
            }
        };
        //   mov = false;
        
        if (player->xpos < 0 && frame) {
            player->xpos = 0;
            // Go Back To previous Frame
            // frame--;
            // player->frame--;
            // player->xpos = gl.xres;
            // refreshBottles();
        }
        //---------------------------------------------------------------------
        // Draw Background
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(0, 0, 1);
        glBindTexture(GL_TEXTURE_2D, background_tex);
        glBegin(GL_QUADS);
            glTexCoord2f(      0.2 * frame, 1);  glVertex2i (      0,       0);
            glTexCoord2f(0.2 * frame + 0.2, 1);  glVertex2i (gl.xres,       0);
            glTexCoord2f(0.2 * frame + 0.2, 0);  glVertex2i (gl.xres, gl.yres);
            glTexCoord2f(      0.2 * frame, 0);  glVertex2i (      0, gl.yres);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        //---------------------------------------------------------------------
        // Draw Dead Enemies
        for (int i = 0; i < enemies_num; i++) {
            if (!enemies[i].alive && enemies[i].frame == frame) enemies[i].Display();
        }
        //---------------------------------------------------------------------
        // Display back enemies
        for (int i = 0; i < enemies_num; i++) {
            if (enemies[i].frame == frame && enemies[i].ypos >= player->ypos) {
                enemies[i].Display();
            }
            enemies[i].Move(player->xpos, player->ypos, player->frame);
        }
        //---------------------------------------------------------------------
        // Draws Player
        player->Display();        
        //---------------------------------------------------------------------
        // Draws front enemies
        for (int i = 0; i < enemies_num; i++) {
            if (enemies[i].frame == frame && enemies[i].ypos < player->ypos && enemies[i].alive) {
                enemies[i].Display();
            }
            // enemies[i].Move(player->xpos, player->ypos, player->frame);
        }
        //---------------------------------------------------------------------
        // Draw Bottle
        player->showAttack();
        //---------------------------------------------------------------------
        // Calls key bindings
        checkMove();
        //---------------------------------------------------------------------
        // Draw foreground (aka the bushes in front)
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(0, 0, 0);
        glBindTexture(GL_TEXTURE_2D, foreground_tex);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
        glBegin(GL_QUADS);
            glTexCoord2f(      0.2 * frame, 1);  glVertex2i (      0,       0);
            glTexCoord2f(0.2 * frame + 0.2, 1);  glVertex2i (gl.xres,       0);
            glTexCoord2f(0.2 * frame + 0.2, 0);  glVertex2i (gl.xres, gl.yres);
            glTexCoord2f(      0.2 * frame, 0);  glVertex2i (      0, gl.yres);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
        
    }
};

void Level_1::checkMove()
{
    int step = player->step;
    int mainMenu[] = {0, 1, 0};

    int    up = XK_w;
    int  down = XK_s;
    int  left = XK_a;
    int right = XK_d;
    
    int  escape = XK_Escape;
    int key_two = XK_2;

    unsigned char *keys = gl.keys;
    
    // Change Levels
    if (keys[key_two]) {
		display = false;
		screen.level_2->display = true;
	}
	//--------------------------------------------------------------------------
	// Player Motion
    if (keys[up]) {
		if ( (player->ypos + player->size/3) < gl.yres / 2) {
	        player->ypos += step;
		}
        player->mov = true;
    }
    if (keys[down]) {
		if (player->ypos > 0) {
	        player->ypos -= step;
		}
        player->mov = true;
    }
    if (keys[left]) {
		if ( !(!frame && player->xpos < 0)  ) {
	        player->xpos -= step;
		}
        player->mov = true;
        player->bwd = true;
        player->fwd = false;
    }
    if (keys[right]) {
        player->xpos += step;
        player->mov = true;
        player->fwd = true;
        player->bwd = false;
    }
    //--------------------------------------------------------------------------
	// Escape to Main Menu
    if (keys[escape]) menuSelect(screen, mainMenu, 3);

	// Check for any kind of movement
    if ( !keys[up] && !keys[down] && !keys[left] && !keys[right] ) {
        player->mov = false;
    }

    // Check for attack
    int    throw_up = XK_Up;
    int  throw_down = XK_Down;
    int  throw_left = XK_Left;
    int throw_right = XK_Right;

    if (keys[throw_right]) {
        player->Attack(1);
    } else if (keys[throw_left]) {
        player->Attack(2);
    } else if (keys[throw_up]) {
        player->Attack(3);
    } else if (keys[throw_down]) {
        player->Attack(4);
    }

    int right_bracket = XK_bracketright;
    int  left_bracket = XK_bracketleft;

    if (keys[right_bracket]) {
        if (player->fireRate > 10){
            player->fireRate -= 1;
            if (player->time >= player->fireRate) {
                player->time  = player->fireRate;
            }
        }
    }
    if (keys[left_bracket]) {
        if (player->fireRate < 30) {
            player->fireRate += 1;
        }
    }

    checkHit();

}

void Level_1::refreshBottles()
{
    for (int i = 0; i < NUM_BOTTLES; i++) {
        player->bottles[i].active = false;
    }
}

void Level_1::checkHit()
{
    for (int i = 0; i < NUM_BOTTLES; i++) {
        int xpos_b = player->bottles[i].xpos;
        int ypos_b = player->bottles[i].ypos;
        int active = player->bottles[i].active;

        if (active) {
            for (int j = 0; j < enemies_num; j++) {
                int xpos_e = enemies[j].xpos;
                int ypos_e = enemies[j].ypos;
                int size_e = enemies[j].size;
                bool alive = enemies[j].alive;
                bool stood = enemies[j].stood;

                bool sameFrame = (frame == enemies[j].frame);

                bool inX = (xpos_b > xpos_e-size_e) && (xpos_b < xpos_e+(size_e/2));
                bool inY = (ypos_b > ypos_e) && (ypos_b < ypos_e+2*size_e);

                if (inX && inY && active && alive && sameFrame && stood) {
					player->bottles[i].active = false;
                    enemies[j].health -= 1;
                }
            }
        }
    }
}

int Level_1::checkKey(int key)
{
    switch (key) {
        case XK_l:
        {
            printf("Level_1 Debug\n");
            printf("Frame: %d\n", frame);
            for (int i = 0; i < enemies_num; i++) {
                printf("\tEnemy: %d\n", i);
                printf("\t xpos: %d\n", enemies[i].xpos);
                printf("\t ypos: %d\n", enemies[i].ypos);
                printf("\tframe: %d\n", enemies[i].frame);
            }

        }
        break;
    }
    return 0;
}

void menuSelect(Screens screen, int opt[], int size)
{
    for (int i = 0; i < size; i++) {
        if (opt[i]) {
            *screen.displays[i] = true;
        } else {
            *screen.displays[i] = false;
        }
    }
}

Bottle::Bottle()
{
    direction = 0;
       active = false;
        frame = 0;
           ds = 0;

    glGenTextures(1, &bottle_tex);
    glBindTexture(GL_TEXTURE_2D, bottle_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    unsigned char *new_img = buildAlphaData(&bottle_img);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bottle_img.width, bottle_img.height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, new_img);
    free(new_img);
}

void Bottle::Attack(int x, int y, int dir)
{
    xpos = x;
    ypos = y;
    size = gl.yres / 35;

    direction = dir;
    active = true;
}

void Bottle::Display()
{
    if (active) {

        drawHitBox();

        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(xpos, ypos, 0);
        glRotatef( ds, 0.0f, 0.0f, 1.0f);
        glBindTexture(GL_TEXTURE_2D, bottle_tex);
        glEnable(GL_ALPHA_TEST);
        glAlphaFunc(GL_GREATER, 0.0f);
        glBegin(GL_QUADS);
            glTexCoord2f( 0, 1); glVertex2i(-size, -size);
            glTexCoord2f( 1, 1); glVertex2i( size, -size);
            glTexCoord2f( 1, 0); glVertex2i( size,  size);
            glTexCoord2f( 0, 0); glVertex2i(-size,  size);
        glEnd();
        glPopMatrix();
        glDisable(GL_ALPHA_TEST);
        glBindTexture(GL_TEXTURE_2D, 0);
        
        frame += 0.125;
        switch(direction) {
            case 1:
                xpos += step;
                ds -= 25;
            break;
            case 2:
                xpos -= step;
                ds += 25;
            break;
            case 3:
                ypos += step;
                ds -= 25;
            break;
            case 4:
                ypos -= step;
                ds += 25;
            break;
        }

        if (xpos > gl.xres || xpos < 0 || ypos < 0 || ypos > gl.yres) {
            active = false;
            direction = 0;
        }
    }
}

void Bottle::drawHitBox()
{
    if (debug) {
        float color[3] =  {40.0 / 255, 255.0 / 255, 0.0};

        for (int i = 0; i < 4; i++) {
            hitbox[i][0] = (i==0||i==3) ? -size : size;
            hitbox[i][1] = (i < 2) ? -size : size;
        }

        glPushMatrix();
        glTranslatef(xpos, ypos, 0);
        glDisable(GL_TEXTURE_2D);
        glColor3fv(color);
        glBegin(GL_LINES);
            glVertex2iv(hitbox[0]); glVertex2iv(hitbox[1]);
            glVertex2iv(hitbox[1]); glVertex2iv(hitbox[2]);
            glVertex2iv(hitbox[2]); glVertex2iv(hitbox[3]);
            glVertex2iv(hitbox[3]); glVertex2iv(hitbox[0]);
        glEnd();
        glPopMatrix();
        glEnable(GL_TEXTURE_2D);
    }
}

void Bottle::Shatter()
{
	active = false;
	xpos = gl.xres * 2;
	ypos = gl.yres * 2;
}

Player::Player()
{
    //  Texture
    player_num = sizeof(player_img) / sizeof(*player_img);
    player_tex = new GLuint[player_num];

	// Player Variables
    xpos = 0;
    ypos = 100;
    size = gl.yres / 7;
    step = size / 10;;

    // Display Variables
    display = false;
        fwd = true;
        bwd = false;
        mov = false;
      frame = 0;    

    // Attack Weapon Variables
      timer = 0;
       time = 0;
      shoot = true;
    bottles = new Bottle[NUM_BOTTLES];

    // Ablilities
     fireRate = 30;
    fireSpeed = size / 7;

    for (int i = 0; i < player_num; i++) {
        glGenTextures(1, &player_tex[i]);
        glBindTexture(GL_TEXTURE_2D, player_tex[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        unsigned char *new_img = buildAlphaData(&player_img[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, player_img[i].width, player_img[i].height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, new_img);
        free(new_img);
    }
}

void Player::Display()
{
    if (display) {

        drawHitBox();

        timer = (timer + 1) % fireRate;
        if (timer == time) {
            shoot = true;
        }

        step = size / 10;
        
        float inc = 0.125;
        static int count = 0;
        int    old_count = count;

        int depth = gl.yres - ypos;
        float depthRatio = ((float)depth + 2*gl.yres) / (3 * gl.yres); 
        size = gl.yres / 7 * depthRatio;

        if (mov) {
            count++;
            count = count % 8;
        }

        float frame = inc * count;

        if (old_count == count) {
            count = 0;
        }

        GLuint texture;
        if (mov) {

            if (fwd) texture = player_tex[2];
            else     texture = player_tex[3];

            glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glTranslatef(xpos, ypos, 0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glBegin(GL_QUADS);
                glTexCoord2f(frame       , 1); glVertex2i(-size,      0);
                glTexCoord2f(frame + .125, 1); glVertex2i( size,      0);
                glTexCoord2f(frame + .125, 0); glVertex2i( size, 2*size);
                glTexCoord2f(frame       , 0); glVertex2i(-size, 2*size);
            glEnd();
            glPopMatrix();
            glDisable(GL_ALPHA_TEST);
            glBindTexture(GL_TEXTURE_2D, 0);
        } else {

            if (fwd) texture = player_tex[0];
            else     texture = player_tex[1];

            glColor3f(1.0f, 1.0f, 1.0f);
            glPushMatrix();
            glTranslatef(xpos, ypos, 0);
            glBindTexture(GL_TEXTURE_2D, texture);
            glEnable(GL_ALPHA_TEST);
            glAlphaFunc(GL_GREATER, 0.0f);
            glBegin(GL_QUADS);
                glTexCoord2f(0, 1); glVertex2i(-size,      0);
                glTexCoord2f(1, 1); glVertex2i( size,      0);
                glTexCoord2f(1, 0); glVertex2i( size, 2*size);
                glTexCoord2f(0, 0); glVertex2i(-size, 2*size);
            glEnd();
            glPopMatrix();
            glDisable(GL_ALPHA_TEST);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        // showAttack();
    }
}

void Player::drawHitBox()
{
    if (debug) {
        float color[3] =  {40.0 / 255, 255.0 / 255, 0.0};

        for (int i = 0; i < 4; i++) {
            hitbox[i][0] = (i==0||i==3) ? -size/2 : size/2;
            hitbox[i][1] = (i < 2) ? 0 : 2*size;
        }

        glPushMatrix();
        glTranslatef(xpos, ypos, 0);
        glDisable(GL_TEXTURE_2D);
        glColor3fv(color);
        glBegin(GL_LINES);
            glVertex2iv(hitbox[0]); glVertex2iv(hitbox[1]);
            glVertex2iv(hitbox[1]); glVertex2iv(hitbox[2]);
            glVertex2iv(hitbox[2]); glVertex2iv(hitbox[3]);
            glVertex2iv(hitbox[3]); glVertex2iv(hitbox[0]);
        glEnd();
        glPopMatrix();
        glEnable(GL_TEXTURE_2D);
    }
}

void Player::Attack(int dir)
{   
    if (shoot) {
        shoot = false;
        int x = xpos;
        int y = ypos + size;
        time = timer;

        for (int i = 0; i < NUM_BOTTLES; i++) {
            if (!bottles[i].active) {
                bottles[i].step = fireSpeed;
                bottles[i].Attack(x, y, dir);
                break;
            }
        }
    }
}

void Player::showAttack()
{
    for (int i = 0; i < NUM_BOTTLES; i++) {
        if (bottles[i].active) bottles[i].Display();
    }	
}

// SetUp Level_2
Level_2::Level_2(Player *playa)
{   
    // Variables
    display = false;
      frame = 0;    

    // Object Variables
    player  = playa;

    int enemPerLev[5] = {2, 3, 4, 5, 6};
    enemies_num = 0;
    for (int i =0; i < 5; i++) enemies_num += enemPerLev[i];
    enemies = new Enemy[enemies_num];
    int index = 0;

    for (int i = 0; i < enemies_num; i++) {
        int randX = rand() % (2*gl.xres/3) + (gl.xres/3);
        int randY = rand() % (gl.yres / 2);
        if (!enemPerLev[index]) index++; 
        enemies[i].setLoc(randX, randY, index);
        enemPerLev[index]--;
    }
    
    //Generate textures and bind images for team photos
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
}

void Level_2::Display()
{
	if (display) {
        player->display = true;

        //Clear the screen
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
        //---------------------------------------------------------------------
        // Checks to see if player is far left or far right
        
        if (player->xpos > gl.xres) {
            if (frame == 4) {
                player->xpos = gl.xres;
                display = false;
                screen.credits->display = true;
            } else {
                frame++;
                player->frame++;
                player->xpos = 0;
                refreshBottles();
            }
        };
        
        if (player->xpos < 0 && frame) {
            player->xpos = 0;
            // Go Back To previous Frame
            // frame--;
            // player->frame--;
            // player->xpos = gl.xres;
            // refreshBottles();
        }
        //---------------------------------------------------------------------
        // Draw
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(0, 0, 1);
        glBindTexture(GL_TEXTURE_2D, background_tex);
        glBegin(GL_QUADS);
            glTexCoord2f(      0.2 * frame, 1);  glVertex2i (      0,       0);
            glTexCoord2f(0.2 * frame + 0.2, 1);  glVertex2i (gl.xres,       0);
            glTexCoord2f(0.2 * frame + 0.2, 0);  glVertex2i (gl.xres, gl.yres);
            glTexCoord2f(      0.2 * frame, 0);  glVertex2i (      0, gl.yres);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
        //---------------------------------------------------------------------
        // Draw Dead Enemies
        for (int i = 0; i < enemies_num; i++) {
            if (!enemies[i].alive && enemies[i].frame == frame) enemies[i].Display();
        }
        //---------------------------------------------------------------------
        // Display back enemies
        for (int i = 0; i < enemies_num; i++) {
            if (enemies[i].frame == frame && enemies[i].ypos >= player->ypos) {
                enemies[i].Display();
            }
            enemies[i].Move(player->xpos, player->ypos, player->frame);
        }
        //---------------------------------------------------------------------
        // Draws Player
        player->Display(); 

        //---------------------------------------------------------------------
        // Draws front enemies
        for (int i = 0; i < enemies_num; i++) {
            if (enemies[i].frame == frame && enemies[i].ypos < player->ypos && enemies[i].alive) {
                enemies[i].Display();
            }
        }
        //---------------------------------------------------------------------
        // Draw Bottles
        player->showAttack();

        //---------------------------------------------------------------------
        // Calls key bindings
        checkMove();

        //---------------------------------------------------------------------
        // Draw foreground (aka the bushes in front)
        glColor3f(1.0f, 1.0f, 1.0f);
        glPushMatrix();
        glTranslatef(0, 0, 0);
        glBindTexture(GL_TEXTURE_2D, foreground_tex);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
        glBegin(GL_QUADS);
            glTexCoord2f(      0.2 * frame, 1);  glVertex2i (      0,       0);
            glTexCoord2f(0.2 * frame + 0.2, 1);  glVertex2i (gl.xres,       0);
            glTexCoord2f(0.2 * frame + 0.2, 0);  glVertex2i (gl.xres, gl.yres);
            glTexCoord2f(      0.2 * frame, 0);  glVertex2i (      0, gl.yres);
        glEnd();
        glPopMatrix();
        glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_ALPHA_TEST);
        
    }
}

void Level_2::checkMove()
{
    int step = player->step;
    int mainMenu[] = {0, 1, 0};

    int    up = XK_w;
    int  down = XK_s;
    int  left = XK_a;
    int right = XK_d;
    
    int  escape = XK_Escape;
    int key_two = XK_2;

    unsigned char *keys = gl.keys;
    
    // Change Levels
    if (keys[key_two]) {
		display = false;
		screen.level_2->display = true;
	}
	//--------------------------------------------------------------------------
	// Player Motion
    if (keys[up]) {
		if ( (player->ypos + player->size/3) < gl.yres / 2) {
	        player->ypos += step;
		}
        player->mov = true;
    }
    if (keys[down]) {
		if (player->ypos > 0) {
	        player->ypos -= step;
		}
        player->mov = true;
    }
    if (keys[left]) {
		if ( !(!frame && player->xpos < 0)  ) {
	        player->xpos -= step;
		}
        player->mov = true;
        player->bwd = true;
        player->fwd = false;
    }
    if (keys[right]) {
        player->xpos += step;
        player->mov = true;
        player->fwd = true;
        player->bwd = false;
    }
    //--------------------------------------------------------------------------
	// Escape to Main Menu
    if (keys[escape]) menuSelect(screen, mainMenu, 3);

	// Check for any kind of movement
    if ( !keys[up] && !keys[down] && !keys[left] && !keys[right] ) {
        player->mov = false;
    }

    // Check for attack
    int    throw_up = XK_Up;
    int  throw_down = XK_Down;
    int  throw_left = XK_Left;
    int throw_right = XK_Right;

    if (keys[throw_right]) {
        player->Attack(1);
    } else if (keys[throw_left]) {
        player->Attack(2);
    } else if (keys[throw_up]) {
        player->Attack(3);
    } else if (keys[throw_down]) {
        player->Attack(4);
    }

    int right_bracket = XK_bracketright;
    int  left_bracket = XK_bracketleft;

    if (keys[right_bracket]) {
        if (player->fireRate > 10){
            player->fireRate -= 1;
            if (player->time >= player->fireRate) {
                player->time  = player->fireRate;
            }
        }
    }
    if (keys[left_bracket]) {
        if (player->fireRate < 30) {
            player->fireRate += 1;
        }
    }

    checkHit();

}

void Level_2::checkHit()
{
    for (int i = 0; i < NUM_BOTTLES; i++) {
        int xpos_b = player->bottles[i].xpos;
        int ypos_b = player->bottles[i].ypos;
        int active = player->bottles[i].active;

        if (active) {
            for (int j = 0; j < enemies_num; j++) {
                int xpos_e = enemies[j].xpos;
                int ypos_e = enemies[j].ypos;
                int size_e = enemies[j].size;
                bool alive = enemies[j].alive;

                bool sameFrame = (frame == enemies[j].frame);

                bool inX = (xpos_b > xpos_e-size_e) && (xpos_b < xpos_e+(size_e/2));
                bool inY = (ypos_b > ypos_e) && (ypos_b < ypos_e+2*size_e);

                if (inX && inY && active && alive && sameFrame) {
					player->bottles[i].active = false;
                    enemies[j].health -= 1;
                }
            }
        }
    }
}

int Level_2::checkKey(int key)
{
    switch (key) {
        case XK_l:
        {
            printf("Level_1 Debug\n");
            printf("Frame: %d\n", frame);
            for (int i = 0; i < enemies_num; i++) {
                printf("\tEnemy: %d\n", i);
                printf("\t xpos: %d\n", enemies[i].xpos);
                printf("\t ypos: %d\n", enemies[i].ypos);
                printf("\tframe: %d\n", enemies[i].frame);
            }

        }
        break;
    }
    return 0;
}

void Level_2::refreshBottles()
{
    for (int i = 0; i < NUM_BOTTLES; i++) {
        player->bottles[i].active = false;
    }
}

Enemy::Enemy()
{   
    // Texture
    enemy_num = sizeof(enemy_img) / sizeof(*enemy_img);
    enemy_tex = new GLuint[enemy_num];

    // Position Variable
    size = 100;
    step = 5;
    rand_diff = rand() % 4 + 11;

    // Display Variables
    health = 5;
      near = false;
     stood = false;
     alive =  true;
       fwd = rand() % 2;
       bwd = !fwd;
       mov = false;
     slice = 0;

    for (int i = 0; i < enemy_num; i++) {
        glGenTextures(1, &enemy_tex[i]);
        glBindTexture(GL_TEXTURE_2D, enemy_tex[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        unsigned char *new_img = buildAlphaData(&enemy_img[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, enemy_img[i].width, enemy_img[i].height, 0,
            GL_RGBA, GL_UNSIGNED_BYTE, new_img);
        free(new_img);
    }
}

void Enemy::Display()
{
    if (alive) drawHitBox();
    step = size / rand_diff;
    int depth = gl.yres - ypos;
    float depthRatio = ((float)depth + 2*gl.yres) / (3 * gl.yres); 
    size = gl.yres / 7 * depthRatio;
    if (health < 1) alive = false;

    if (mov && alive) moveDraw();
    else if (alive) {
        if (near) {
            if (stood) standDraw();
            else standupDraw();
        } else {
            if (stood) sitdownDraw();
            else sitDraw();
        }
    } else {
        Die();
    }
}

void Enemy::drawHitBox()
{
    if (debug) {
        float color[3] =  {40.0 / 255, 255.0 / 255, 0.0};

        for (int i = 0; i < 4; i++) {
            hitbox[i][0] = (i == 0 || i == 3) ? -size / 2 : size / 2;
            hitbox[i][1] = (i < 2) ? 0 : 2*size;
        }

        glPushMatrix();
        glTranslatef(xpos, ypos, 0);
        glDisable(GL_TEXTURE_2D);
        glColor3fv(color);
        glBegin(GL_LINES);
            glVertex2iv(hitbox[0]); glVertex2iv(hitbox[1]);
            glVertex2iv(hitbox[1]); glVertex2iv(hitbox[2]);
            glVertex2iv(hitbox[2]); glVertex2iv(hitbox[3]);
            glVertex2iv(hitbox[3]); glVertex2iv(hitbox[0]);
        glEnd();
        glPopMatrix();
        glEnable(GL_TEXTURE_2D);
    }
}

void Enemy::sitDraw()
{
    GLuint texture;
    if (fwd) texture = enemy_tex[4];
    else     texture = enemy_tex[5];

    slice = 0;

    float inc = 0.25;
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(xpos, ypos, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(  0, 1); glVertex2i(-size,      0);
        glTexCoord2f(inc, 1); glVertex2i( size,      0);
        glTexCoord2f(inc, 0); glVertex2i( size, 2*size);
        glTexCoord2f(  0, 0); glVertex2i(-size, 2*size);
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Enemy::standDraw()
{
    slice = 0;

    GLuint texture;
    if (fwd) texture = enemy_tex[0];
    else     texture = enemy_tex[1];

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(xpos, ypos, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 1); glVertex2i(-size,      0);
        glTexCoord2f(1, 1); glVertex2i( size,      0);
        glTexCoord2f(1, 0); glVertex2i( size, 2*size);
        glTexCoord2f(0, 0); glVertex2i(-size, 2*size);
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Enemy::moveDraw()
{
    GLuint texture;
    if (fwd) texture = enemy_tex[2];
    else     texture = enemy_tex[3];

    float inc = 0.125;
    float start = inc * slice;
    float end = start + inc;

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(xpos, ypos, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);            
    glBegin(GL_QUADS);
        glTexCoord2f(start, 1); glVertex2i(-size,      0);
        glTexCoord2f(  end, 1); glVertex2i( size,      0);
        glTexCoord2f(  end, 0); glVertex2i( size, 2*size);
        glTexCoord2f(start, 0); glVertex2i(-size, 2*size);
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);

    slice = (slice + 1) % 8;
}

void Enemy::standupDraw()
{   
    GLuint texture;
    if (fwd) texture = enemy_tex[4];
    else     texture = enemy_tex[5];

    float inc = 0.25;
    float start = slice * inc;
    float   end = start + inc;
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(xpos, ypos, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(start, 1); glVertex2i(-size,      0);
        glTexCoord2f(  end, 1); glVertex2i( size,      0);
        glTexCoord2f(  end, 0); glVertex2i( size, 2*size);
        glTexCoord2f(start, 0); glVertex2i(-size, 2*size);
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (slice == 3) {
        stood = true;
        slice = 0;
    } else {
        slice++;  
    }
}

void Enemy::sitdownDraw()
{
    GLuint texture;
    if (fwd) texture = enemy_tex[4];
    else     texture = enemy_tex[5];
    float inc = 0.25;
    slice = (slice + 3) % 4;
    float start = inc * slice;
    float   end = start + inc;
    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(xpos, ypos, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(start, 1); glVertex2i(-size,      0);
        glTexCoord2f(  end, 1); glVertex2i( size,      0);
        glTexCoord2f(  end, 0); glVertex2i( size, 2*size);
        glTexCoord2f(start, 0); glVertex2i(-size, 2*size);
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (slice % 4 == 0) {
        stood = false;
        slice = 0;
    }
}

void Enemy::setLoc(int x, int y, int _frame)
{
     xpos = x;
     ypos = y;
    frame = _frame;
}

void Enemy::Move(int x, int y, int pframe)
{   
    int frame_diff = (pframe - frame) * gl.xres;

    double dx = -((double)xpos - x) + frame_diff;
    double dy = -((double)ypos - y);

    double dist = sqrt( pow(dx, 2.0) + pow(dy, 2.0) );

    if (dist < 0.6*gl.xres && alive) {
        near = true;
        if (dx > 0) {
            fwd = true;
            bwd = !fwd;
        } else {
            fwd = false;
            bwd =  !fwd;
        }
    } else {
        near = false;
    }

    if (near && dist < 0.5*gl.xres) {
        mov = true;
    } else {
        mov = false;
    }

    if (dist < 50 && alive) {
        Attack();
    }

    if (mov && alive) {

        double dx_enemy = dx * step / dist;
        double dy_enemy = dy * step / dist;
        if (xpos > gl.xres + size) {
            frame++;
            xpos = -size;
        } else if (xpos < -size) {
            frame--;
            xpos = gl.xres + size;
        }
        xpos += dx_enemy;
        ypos += dy_enemy;
    }
}

void Enemy::Attack()
{
	/*
    GLuint texture;
    if (fwd) texture = enemy_tex[8];
    else texture = enemy_tex[9];

    float inc = 0.125;
    float start = inc * slice;
    float   end = start + inc;

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(xpos, ypos, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(start, 1); glVertex2i(-size,      0);
        glTexCoord2f(  end, 1); glVertex2i( size,      0);
        glTexCoord2f(  end, 0); glVertex2i( size, 2*size);
        glTexCoord2f(start, 0); glVertex2i(-size, 2*size);
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);
    * */
      mov = false;
    stood = true;
}

void Enemy::Die()
{
    alive = false;
      mov = false;
    GLuint texture;
    if (fwd) texture = enemy_tex[6];
    else texture = enemy_tex[7];

    float inc = 0.125;
    float start = inc * slice;
    float   end = start + inc;

    glColor3f(1.0f, 1.0f, 1.0f);
    glPushMatrix();
    glTranslatef(xpos, ypos, 0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0.0f);
    glBegin(GL_QUADS);
        glTexCoord2f(start, 1); glVertex2i(-size,      0);
        glTexCoord2f(  end, 1); glVertex2i( size,      0);
        glTexCoord2f(  end, 0); glVertex2i( size, 2*size);
        glTexCoord2f(start, 0); glVertex2i(-size, 2*size);
    glEnd();
    glPopMatrix();
    glDisable(GL_ALPHA_TEST);
    glBindTexture(GL_TEXTURE_2D, 0);

    if (slice < 7) slice++;
}
