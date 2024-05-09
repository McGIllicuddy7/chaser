#pragma once
#include "engine.h"
class Player:public Entity{
    ResourceRef m_texture;
    public:
    void on_tick();
    void on_init(ResourceRef this_ref);
    void on_render();
    void on_destroy();
};