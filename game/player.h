#pragma once
#include "engine.h"
#include "manager.h"
class Player:public Entity{
    ResourceRef m_texture;
    ResourceRef m_manager;
    Vector2 m_momentum;
    float disp_y;
    int m_health;
    float fired;
    Collision hit;
    public:
    Player(ResourceRef manager);
    void on_tick();
    void on_init(ResourceRef this_ref);
    void on_render();
    void on_destroy();
    void on_collision(Collision col);
    void on_damage(float damage, ResourceRef Other);
};