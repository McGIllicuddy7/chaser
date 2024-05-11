#pragma once
#include <stdlib.h>
#include <string>
#include "raylib/raylib.h"
#include "resource.h"
#include "entity.h"
#include <map>
#include <string>
class Entity;
struct EntityBB;
struct Collision;
struct ColTree{
    static const int stride =32;
    float xdiv;
    float ydiv;
    std::vector<EntityBB>m_area[stride*stride];
    Vector2 m_min;
    Vector2 m_max;
    void initialize(std::vector<EntityBB> &collisions,Vector2 min, Vector2 max);
    Collision line_trace(Vector2 start, Vector2 end,std::vector<EntityBB> &boxes,ResourceRef to_ignore = ResourceRef());
    Collision box_trace(Vector2 start, Vector2 end, Rectangle rec,std::vector<EntityBB> &boxes,ResourceRef to_ignore = ResourceRef());
};

class Runtime {
    static const int num_layers = 4;
    void * m_current_level = 0;
    size_t m_screen_height;
    size_t m_screen_width;
    std::string m_name;
    ResourceCache<Entity> m_entities;
    ResourceRef m_origin_entity;
    bool m_use_entity_as_origin;
    Vector2 m_camera_location;
    std::vector<EntityBB> m_collisions;
    ColTree m_col_tree;
    std::map<std::string, ResourceRef> m_texture_table;
    ResourceCache<Texture> m_textures;
    std::vector<ResourceRef> m_to_draw[num_layers];
    protected:
    void Tick();
    void set_relative_locations();
    void Render();
    void frame_collision_set_up();
    void reset();
    public: 
    ~Runtime();
    Runtime(int height, int width, const char * name);
    static Runtime* New();
    void load_level(std::string level_name);
    void load_level_func(void (*load)(Runtime *));
    void Run();
    Vector2 convert_world_to_screen(Vector2 v) const;
    ResourceRef register_entity(Entity *e);
    void set_entity_as_origin(ResourceRef ref, Vector2 Offset);
    void unset_entity_as_origin();
    bool origin_set();
    Entity * get_entity(ResourceRef ref);
    void destroy_entity(ResourceRef ref);
    Collision line_trace(Vector2 start, Vector2 end,ResourceRef to_ignore = ResourceRef());
    Collision box_trace(Vector2 start, Vector2 end, Rectangle rec,ResourceRef to_ignore = ResourceRef());
    int screen_height();
    int screen_width();
    ResourceRef load_texture_by_name(std::string texture);
    Texture* get_texture(ResourceRef ref);
    void unload_texture(std::string texture);
};
