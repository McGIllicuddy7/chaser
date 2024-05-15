#include "engine.h"
#include "manager.h"
class Bullet:public Entity{
    ResourceRef m_texture;
    Vector2 m_velocity;
    public:
    void on_tick();
    void on_init(ResourceRef this_ref);
    void on_render();
    void on_destroy();
};