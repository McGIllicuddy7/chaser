#pragma once
#include <stdlib.h>
#include <string>
#include <raylib.h>
#include "resource.h"
#include "entity.h"

class Entity;
struct EntityBB;
struct Collision;
struct ColTree{
    const int stride =64;
    float xdiv;
    float ydiv;
    std::vector<EntityBB>m_area[64*64];
    Vector2 m_min;
    Vector2 m_max;
    void initialize(std::vector<EntityBB> &collisions,Vector2 min, Vector2 max);
    Collision line_trace(Vector2 start, Vector2 end,std::vector<EntityBB> &boxes,ResourceRef to_ignore = ResourceRef());
    Collision box_trace(Vector2 start, Vector2 end, Rectangle rec,std::vector<EntityBB> &boxes,ResourceRef to_ignore = ResourceRef());
};
class Runtime {
    size_t m_screen_height;
    size_t m_screen_width;
    std::string m_name;
    ResourceCache<Entity> m_entities;
    ResourceRef m_origin_entity;
    bool m_use_entity_as_origin;
    Vector2 m_camera_location;
    std::vector<EntityBB> m_collisions;
    ColTree m_col_tree;
    protected:
    void Tick();
    void set_relative_locations();
    void Render();
    void frame_collision_set_up();
    public: 
    ~Runtime();
    Runtime(int height, int width, const char * name);
    static Runtime* New();
    void Run();
    Vector2 convert_world_to_screen(Vector2 v) const;
    ResourceRef register_entity(Entity *e);
    void set_entity_as_origin(ResourceRef ref, Vector2 Offset);
    void unset_entity_as_origin();
    bool origin_set();
    Entity * get_entity(ResourceRef ref);
    Collision line_trace(Vector2 start, Vector2 end,ResourceRef to_ignore = ResourceRef());
    Collision box_trace(Vector2 start, Vector2 end, Rectangle rec,ResourceRef to_ignore = ResourceRef());
    int screen_height();
    int screen_width();
};
