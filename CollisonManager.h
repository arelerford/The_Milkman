// Author: Justin Selsor

#include <list>

using namespace std;

class CollisonManager {
    public:
        void add (Entity* e) 
        {
            entities.push_back(e);            
        }
        void remove (Entity* e)
        {
            entities.remove(e);
        }
        list<Entity*> entities;

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
