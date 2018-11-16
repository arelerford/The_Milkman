// Author: Justin Selsor (Sep. 25, 2018)
// For the Milkman Project. Software Enginering.

#include <GL/glx.h>
#include <list>
#include "fonts.h"
#include "Entity.h"
#include <string.h>

using namespace std;

void Entity::render (void) {

}

void Entity::update (void) {

}

bool Entity::checkCollision (Entity& e) {
    if (x < (e.x + e.width) && (x + width) > e.x && y < (e.y + e.height) &&
        (y + height) > e.y) {
            return true;
    }
    
    return false;
}

// Manageges Physics interactions
class CollisonManager {
    public:
        list<Entity*> enities;
        
        CollisonManager () 
        {
            enities.clear();
        }

        void addEntity (Entity* e) 
        {
            enities.insert(enities.begin (), e);
        }

        void checkCollisons () 
        {
            // FOR ALL NON-STATIC ENTITIES CHECK COLLISONS BY BOUNDING BOX.
            // AND ONLY DO THIS IF THE ENITIES ARE CLOSE TOGETHER.

            // Foreach elementing in the enities list:
            //      If a entity is non-static:
            //          Compare it to each entity if there is a collison and
            //              the entities are not far apart:
            //              Move the enitites so that it is not collideing with the other.
        }
} physicsManager;

// TODO: Modifie to allow for offsets also look at paramters for
//       ggprint8b(Rect, int, int, string) and what they do.
//       ggprint8b(Rect, ,color, string to draw) 
void show_credits_justin(Rect *r)
{
    ggprint16(r, 150, 0, "Justin S.");
}

// TODO: Animate image.
void show_justin_image (int x, int y, GLuint textid)
{
    glColor3f (1.0f, 1.0f, 1.0f);
    
    int wid = 40;

    glPushMatrix();

    glTranslatef(x, y, 0);
    
    glBindTexture(GL_TEXTURE_2D, textid);

    glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 1.0f); glVertex2i (-wid, -wid);
        glTexCoord2f(0.0f, 0.0f); glVertex2i (-wid, wid);
        glTexCoord2f(1.0f, 0.0f); glVertex2i (wid, wid);
        glTexCoord2f(1.0f, 1.0f); glVertex2i (wid, -wid);
    glEnd();

    glPopMatrix();
}

// Converts a pixel positon to a opengl mapping.
float convert_pixel_position (float positon, float res)
{
    return (2 / res) * (positon) - 1;
}

// Converts a gl mapping to a pixel position.
float convert_gl_postion (float positon, float res)
{
    return positon * res;
}
