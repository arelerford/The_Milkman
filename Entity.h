// Author: Justin Selsor (Nov. 14 2018)
// For the Milkman Projects. Software Enginering.
#ifndef ENTITIY_H_
#define ENTITIY_H_

#include <string>

using namespace std;

class Entity {
    public:
        explicit Entity (string, float, float, float, float, bool);
        string name;
        float x, y;
        float width, height;
        bool isStatic;
        void render(void);
        void update(void);
        bool checkCollision(Entity* e);
        float distance (Entity* e);
        void onCollision(void);
    protected:
        void init (string, float, float, float, float, bool);
    private:
        Entity();
};

#endif
