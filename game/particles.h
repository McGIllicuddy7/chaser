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
};