#pragma once
#include "engine.h"
#include "manager.h"
class Player:public Entity{
    ResourceRef m_texture;
    ResourceRef m_manager;
    Vector2 m_momentum;
    float disp_y;
    int m_health;
    int missile_ammo;
    int railgun_ammo;
    int power;
    float ammo_timer;
    int chaff_ammo;
    public:
    Player(ResourceRef manager);
    void on_tick();
    void on_init(ResourceRef this_ref);
    void on_render();
    void on_destroy();
    void on_collision(Collision col);
    void on_damage(float damage, ResourceRef Other);
    size_t get_id();
    float y_disp();
};