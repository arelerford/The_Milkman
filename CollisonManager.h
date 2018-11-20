// Author: Justin Selsor

#ifndef COLLISONMANAGER_H_
#define COLLISIONMANGER_H_

#include <list>
#include <algorithm>
#include "Entity.h"

using namespace std;

class CollisonManager {
    public:

        static CollisonManager& getInstance()
        {
            static CollisonManager instance;
            return instance;
        }

        list<Entity*> entities;
        list<Entity*> nonStaticEntities;

        void add (Entity*);
        void remove (Entity*);
        Entity* getNonStaticEntity (int);
        Entity* getEntity (int);

    private:
        CollisonManager () {}
        CollisonManager (CollisonManager const&); // Don't implement
        void operator = (CollisonManager const&); // Don't implement
};

#endif
