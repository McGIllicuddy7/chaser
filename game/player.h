#pragma once
#include "../engine/entity.h"
class Player:public Entity{
    ResourceRef m_texture;
    public:
    void on_tick();
    void on_init(Runtime * runtime, ResourceRef this_ref);
    void on_render();
    void on_destroy();
};