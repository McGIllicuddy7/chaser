#include "engine.h"
#include "manager.h"
class Bullet:public Entity{
    ResourceRef m_texture;
    Vector2 m_velocity;
    ResourceRef m_firer;
    float m_lf;
    public:
    Bullet(Vector2 location, Vector2 Velocity, ResourceRef firer);
    void on_tick();
    void on_init(ResourceRef this_ref);
    void on_render();
    void on_destroy();
    void free_memory();
};
ResourceRef new_bullet(Vector2 location, Vector2 velocity, ResourceRef firer);