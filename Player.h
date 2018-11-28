#ifndef PLAYER_H_
#define PLAYER_H_

#include "Entity.h"
#include <string>

using namespace std;

class Player : public Entity {
    public:
        using Entity::Entity;
        
        void render (void);
        
        void move (float nx, float ny)
        {
            x += nx;
            y += ny;
        }
};

#endif
