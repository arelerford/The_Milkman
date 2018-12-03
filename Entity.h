// Author: Justin Selsor (Nov. 14 2018)
// For the Milkman Projects. Software Enginering.
// 
// This is a header file for entity objects of the game.

#ifndef ENTITIY_H_
#define ENTITIY_H_

#include <string>
#include <GL/glx.h>
#include "default.h"

using namespace std;

class Entity {
    public:
        explicit Entity (string, int, float, float, float, float, bool);
        int health;
        string name;
        float x, y;
        float width, height;
        bool isStatic;
        bool showImage;
        GLuint texture;
        Image image = "";

        void loadImage (const char* path)
        {
            // Assgins the opengl texture.
            image = path;
            glGenTextures (1, &texture);
            glBindTexture (GL_TEXTURE_2D, texture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, 3, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
        }

        void toggleImage ()
        {
            showImage = !showImage;
        }

        void render(void);
        void update(void);
        bool checkCollision(Entity* e);
        float distance (Entity* e);
        void onCollision(Entity* e);

        void takeDamage(int amount)
        {
            health -= amount;
        }

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
