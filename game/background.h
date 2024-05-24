#pragma once
#include "raylib/raylib.h"
#include "engine.h"
class Building:public Entity{
    bool passed;
    float m_height;
public:
    Building(Vector2 location,float height);
    void on_tick();
    void on_render();
    size_t get_id(); 
    void free_memory();
};
ResourceRef new_building(Vector2 location, float height);
class Star:public Entity{
    float m_radius;
public:
    Star(Vector2 location, float radius);
    void on_tick();
    void on_render();
    size_t get_id(); 
    void free_memory();
};
ResourceRef new_star(Vector2 location, float radius);