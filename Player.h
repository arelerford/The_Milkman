#ifndef PLAYER_H_
#define PLAYER_H_

#include "Entity.h"
#include <string>

using namespace std;

class Player : public Entity {
    public:
        using Entity::Entity;
        
        void render (void);
        void update (void);
        void onCollision(Entity* e);
        
        void move (float nx, float ny)
        {
            x += nx;
            y += ny;
        }

        void jump (float force)
        {
            //TODO: add arc effect to the jump.
            y += force;
        }
};

#endif
