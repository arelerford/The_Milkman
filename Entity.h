// Author: Justin Selsor (Nov. 14 2018)
// For the Milkman Projects. Software Enginering.
#ifndef ENTITIY_H_
#define ENTITIY_H_

#include <string.h>

class Entity {
    public:
        Entity();
        Entity (float, float, float, float, bool);
        float x, y;
        float width, height;
        bool isStatic;
        void render(void);
        void update(void);
        bool checkCollision(Entity* e);
        float distance (Entity* e);
        void onCollision(void);
};

#endif
