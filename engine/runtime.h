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
void cleanup(Entity * entity);
void cleanup(Texture * texture);
void cleanup(Sound * sound);
struct ColTree{
    static const int stride =64;
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
    static const int num_layers = 5;
    void * m_current_level = 0;
    size_t m_screen_height;
    size_t m_screen_width;
    bool m_reset;
    std::string m_name;
    ResourceCache<Entity> m_entities;
    ResourceRef m_origin_entity;
    bool m_use_entity_as_origin;
    Vector2 m_camera_location;
    std::vector<EntityBB> m_collisions;
    ColTree m_col_tree;
    std::map<std::string, ResourceRef> m_texture_table;
    ResourceCache<Texture> m_textures;
    ResourceCache<Sound> m_sounds;
    std::map<std::string, ResourceRef> m_sound_table;
    std::vector<ResourceRef> m_to_draw[num_layers];
    std::vector<ResourceRef> m_entity_destroy_queue;
    Color m_background_color;
    protected:
    void Tick();
    void set_relative_locations();
    void Render();
    void frame_collision_set_up();
    void collect_dead();
    public: 
    void reset();
    void call_reset();
    void clear_all_but(std::vector<ResourceRef> r);
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
    void destroy_entity(ResourceRef ref);
    Collision line_trace(Vector2 start, Vector2 end,ResourceRef to_ignore = ResourceRef());
    Collision box_trace(Vector2 start, Vector2 end, Rectangle rec,ResourceRef to_ignore = ResourceRef());
    int screen_height();
    int screen_width();
    ResourceRef load_texture_by_name(std::string texture);
    Texture* get_texture(ResourceRef ref);
    void unload_texture(std::string texture);
    ResourceRef load_sound_by_name(std::string sound);
    Sound * get_sound(ResourceRef ref);
    void unload_sound(std::string sound);
    void set_background_color(Color color);
};
