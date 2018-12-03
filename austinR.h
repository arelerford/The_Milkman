
#ifndef AUSTINR_H_
#define AUSTINR_H_

#include "default.h"

const int MAX_BULLETS = 1;

class Bullet {
public:
    Vec pos;
    Vec vel;
    GLuint texid;
    Image bottle_img = "./images/player/bottle.png";
    float color[3];
    struct timespec time;
public:
    Bullet() {
    glGenTextures(1, &texid);
        glBindTexture(GL_TEXTURE_2D, texid);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        unsigned char *new_img = buildAlphaData(&bottle_img);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bottle_img.width, bottle_img.height, 0,GL_RGBA, GL_UNSIGNED_BYTE, new_img);
        free(new_img);
    printf("%u\n",texid );
    
    }   
    
};

class Game {
public:
    Bullet *barr;
    int nbullets;
    struct timespec bulletTimer;
/*public:
   Game() {
        barr = new Bullet[MAX_BULLETS];
	nbullets = 0;
	clock_gettime(CLOCK_REALTIME, &bulletTimer);
   } */
    Game();
   ~Game() {
       delete [] barr;
   }
	
};



#endif
