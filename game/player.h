#pragma once
#include "engine.h"
#include "manager.h"
class Player:public Entity{
    ResourceRef m_texture;
    ResourceRef m_manager;
    public:
    Player(ResourceRef manager);
    void on_tick();
    void on_init(ResourceRef this_ref);
    void on_render();
    void on_destroy();
};