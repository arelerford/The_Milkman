// Author: Justin Selsor (Nov. 14 2018)
// For the Milkman Projects. Software Enginering.
#ifndef ENTITIY_H_
#define ENTITIY_H_

#include <string>

using namespace std;

class Entity {
    public:
        explicit Entity (string, int, float, float, float, float, bool);
        int health;
        string name;
        float x, y;
        float width, height;
        bool isStatic;
        void render(void);
        void update(void);
        bool checkCollision(Entity* e);
        float distance (Entity* e);
        void onCollision(Entity* e);
        void takeDamage(int amount);
        void move(float nx, float ny)
        {
            x = nx;
            y = ny;
        }
    protected:
        void init (string, int, float, float, float, float, bool);
    private:
        Entity();
};

#endif
