#include <list>

using namespace std;

class CollisonManager {
    public:
        void add (Entity* e);
        void remove (Entity* e);
        list<Entity*> entities;
    private:
}
