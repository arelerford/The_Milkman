#ifndef ENEMY_H_
#define ENEMY_H_

using namespace std;

class Enemy : public Entity {
    public:
        using Entity::Entity;

        float patrolSpeed;
        float patrolLength;

        void render(void);
        void onCollision(Entity* e);

        void patrolSettings (float _patrolSpeed, float _patrolLength)
        {
            patrolSpeed = _patrolSpeed;
            patrolLength = _patrolLength;

            bound1 = patrolLength + x;
            bound2 = patrolLength - x;
        }

        void move (float nx, float ny)
        {
            x += nx;
            y += ny;
        }
    private:
        float bound1, bound2;
};

#endif
