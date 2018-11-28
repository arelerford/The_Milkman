// Author: Justin Selsor (Sep. 25, 2018)
// For the Milkman Project. Software Enginering.

#include <GL/glx.h>
#include <list>
#include "fonts.h"
#include <list>
#include <algorithm>
#include "Entity.h"
#include "Player.h"
#include "CollisonManager.h"
#include <string>
#include <string.h>
#include <math.h>
#include <iostream>


using namespace std;

Entity::Entity ()
{
    init ("Blank Entity Constuctor Called", 0, 0, 0, 0, true);
}

Entity::Entity (string _name, float _x, float _y, float _width, float _height, bool _isStatic) 
{   
    init (_name, _x, _y, _width, _height, _isStatic);
}

void Entity::init (string _name, float _x, float _y, float _width, float _height, bool _isStatic)
{
    name = _name;
    x = _x;
    y = _y;
    width = _width;
    height = _height;
    isStatic = _isStatic;
    
    CollisonManager::getInstance().add(this);
}

void Entity::render (void) 
{

}

void Entity::update (void) 
{

}

float Entity::distance (Entity* e) 
{
    float dist = 0.0;

    float dx = (e->x - x);
    float dy = (e->y - y);

    dist = sqrt ((dx * dx) + (dy * dy));

    return dist;
}

bool Entity::checkCollision (Entity* e) 
{
    if (x < (e->x + e->width) && (x + width) > e->x && y < (e->y + e->height) &&
        (y + height) > e->y) {
            return true;
    }
    
    return false;
}

void Entity::onCollision()
{
    
}

void CollisonManager::add (Entity* e)
{
    //cout << e->name << ": added to list\n";

    entities.push_back(e);

    if (!e->isStatic) {
        nonStaticEntities.push_back (e);
    }
}

void CollisonManager::remove (Entity* e)
{
    entities.remove(e);

    if (!e->isStatic) {
        nonStaticEntities.remove(e);
    }
}

Entity* CollisonManager::getNonStaticEntity (int i)
{
    int count = 0;
    for (list<Entity*>::const_iterator ci = nonStaticEntities.begin(); ci != nonStaticEntities.end(); ++ci) {
        if (i == count) {
            return *ci;
        }

        count++;
    }

    return NULL;
}

Entity* CollisonManager::getEntity (int i)
{
    int count = 0;
    for (list<Entity*>::const_iterator ci = entities.begin(); ci != entities.end(); ++ci) {
        if (i == count) {
            return *ci;
        }

        count++;
    }

    return NULL;
}

void Player::render () 
{
    glColor3f (0.0f, 1.0f, 0.0f);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(x, y);
    	glTexCoord2f(0, 1); glVertex2f(x, y + height);
		glTexCoord2f(1, 1); glVertex2f(x + width, y + height);
    	glTexCoord2f(1, 0); glVertex2f(x + width, y);
	glEnd();
}

// Manageges Physics interactions
void managerCollison ()
{
    cout << CollisonManager::getInstance().nonStaticEntities.size();

    for (unsigned int i = 0; 
            i < CollisonManager::getInstance().nonStaticEntities.size(); 
            i++) {
        Entity* e1 = CollisonManager::getInstance().getNonStaticEntity(i);
        for (unsigned int j = 0; 
            j < CollisonManager::getInstance().entities.size(); 
            j++) {
            Entity* e2 = CollisonManager::getInstance().getEntity(j);

            if (e1->distance (e2) < 10.0) {
                bool collison = e1->checkCollision(e2);
                if (collison) {
                    cout << "COLLISON!\n";
                    e1->onCollision();
                    e2->onCollision();           
                }
            }
        }
    }
}

void show_credits_justin(Rect *r)
{
    ggprint16(r, 150, 0, "Justin S.");
}

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
