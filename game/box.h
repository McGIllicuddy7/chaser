#pragma once
#include "../engine/entity.h"
class Box:public Entity{
    float m_hit;
    public:
    void on_tick();
    void on_init(Runtime * runtime, ResourceRef this_ref);
    void on_render();
    void on_collision(Collision col);
};