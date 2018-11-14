// Author: Justin Selsor (Nov. 14 2018)
// For the Milkman Projects. Software Enginering.

class Entity {
    public:
        float x, y;
        float width, height;
        bool isStatic;
        void render(void);
        void update(void);
        bool checkCollision(Entity e);
};
