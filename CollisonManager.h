// Author: Justin Selsor

#include <list>

using namespace std;

class CollisonManager {
    public:
        void add (Entity* e) 
        {
            entities.push_back(e);

            if (!e->isStatic) {
                nonStaticEntities.push_back (e);
            }
        }
        
        void remove (Entity* e)
        {
            entities.remove(e);

            if (!e->isStatic) {
                nonStaticEntities.remove(e);
            }
        }

        list<Entity*> entities;
        list<Entity*> nonStaticEntities;

        // Implmentation of a singleton pattern.
        static CollisonManager *instance() {
            if (!s_instance)
                s_instance = new CollisonManager();
            return s_instance;
        }
    private:
        static CollisonManager *s_instance;
        CollisonManager () {
            
        }
};

// This may not be the best way to do this, but it works for now.
CollisonManager *CollisonManager::s_instance = 0;
