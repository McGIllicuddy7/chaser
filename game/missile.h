#pragma once
#include "engine.h"
class Missile:public Entity{
    ResourceRef m_texture;
    Vector2 m_velocity;
    ResourceRef m_firer;
    ResourceRef m_target;
    float m_lf;
    float m_look_timer;
    size_t m_target_id;
    bool m_found_target;
    public:
    Missile(Vector2 location, Vector2 Velocity, ResourceRef firer, size_t target_id);
    void on_tick();
    void on_init(ResourceRef this_ref);
    void on_render();
    void on_destroy();
    void free_memory();
};
ResourceRef new_missile(Vector2 location, Vector2 Velocity, ResourceRef firer, size_t target_id);