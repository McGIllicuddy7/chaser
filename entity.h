#pragma once
#include <raylib.h>
#include <raymath.h>
#include "resource.h"
#include "runtime.h"
#include "ops.h"
class Runtime;

struct Collision{
    bool hit; 
    Vector2 location;
    Vector2 normal;
    ResourceRef collided_with;
};
struct EntityBB{
    ResourceRef Parent;
    Rectangle box;
};
class Entity{
    protected:
    Runtime * m_runtime;
    ResourceRef m_this_ref;
    Rectangle m_collision;
    Vector2 m_velocity;
public:
    virtual void on_tick();
    virtual void on_init(Runtime * runtime, ResourceRef this_ref);
    virtual void on_render();
    void set_location(Vector2 location);
    void set_velocity(Vector2 velocity);
    virtual void on_destroy();
    virtual void on_collision(Collision col);
    Vector2 get_location();
    Vector2 get_velocity();
    EntityBB get_bb();
    virtual void on_damage(float damage,ResourceRef other);
};
void cleanup(Entity * entity);
