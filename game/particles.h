#pragma once
#include "raylib/raylib.h"
#include "engine.h"
class LaserBeam:public Entity{
    Vector2 m_start;
    Vector2 m_end;
    ResourceRef m_parent;
    float timer;
    public:
    LaserBeam(Vector2 start, Vector2 end, ResourceRef Parent);
    void on_tick();
    void on_render();
    size_t get_id();
};
class Chaff:public Entity{
    float remaining_life;
    float temperature;
public:
    Chaff(Vector2 location, Vector2 velocity);
    void on_tick();
    void on_render();
    void on_damage(float damage, ResourceRef damager);
    size_t get_id();
};
class ShipExplosion:public Entity{
    float time;
    ResourceRef frames[14];
    public:
    ShipExplosion(Vector2 location, Vector2 velocity);
    void on_tick();
    void on_render();
    size_t get_id(); 
};