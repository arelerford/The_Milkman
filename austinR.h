
#ifndef AUSTINR_H_
#define AUSTINR_H_

#include "default.h"

const int MAX_BULLETS = 1;

class Bullet {
public:
    Vec pos;
    Vec vel;
    float color[3];
    struct timespec time;
public:
    Bullet() { }
};

class Game {
public:
    Bullet *barr;
    int nbullets;
    struct timespec bulletTimer;
public:
   Game() {
        barr = new Bullet[MAX_BULLETS];
	nbullets = 0;
	clock_gettime(CLOCK_REALTIME, &bulletTimer);
   } 
   ~Game() {
       delete [] barr;
   }
	
} g;



#endif
