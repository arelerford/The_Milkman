#ifndef ENEMY_H_
#define ENEMY_H_

using namespace std;

class Enemy : public Entity {
    public:
        using Entity::Entity;

        void render(void);
        void onCollision(Entity* e);

        void patrolSettings (float)
        {

        }

        void move (float nx, float ny)
        {
            x += nx;
            y += ny;
        }
};

#endif
