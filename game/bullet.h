#include "engine.h"
#include "manager.h"
class Bullet:public Entity{
    ResourceRef m_texture;
    Vector2 m_velocity;
    float m_lf;
    public:
    Bullet(Vector2 location, Vector2 Velocity);
    void on_tick();
    void on_init(ResourceRef this_ref);
    void on_render();
    void on_destroy();
};