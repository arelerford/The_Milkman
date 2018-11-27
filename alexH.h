#ifndef ALEXH_H_
#define ALEXH_H_

#include "default.h"

class Controls{
public:
        const char *name = "controls";

        //variables
        bool display = false;

        //Methods
        Controls();
        void Display();
        int checkKey(int);
};
Health Class // goes to player and enemey entity
int playerhealth = 5;
int enemyhealth = 3

//if projectile collides with player entity
//    decrement health
      //if(playerhealth = 0)
        //gameoverscreen  
#endif
