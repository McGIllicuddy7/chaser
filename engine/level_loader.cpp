#include "runtime.h"
#include <dlfcn.h>
/*   ResourceCache<Entity> m_entities;
    ResourceRef m_origin_entity;
    bool m_use_entity_as_origin;
    Vector2 m_camera_location;
    std::vector<EntityBB> m_collisions;
    ColTree m_col_tree;
    std::map<std::string, ResourceRef> m_texture_table;
    ResourceCache<Texture> m_textures;
    std::vector<ResourceRef> m_to_draw[num_layers];*/
void Runtime::load_level(std::string level_name){
    void * lib = dlopen(level_name.c_str(), RTLD_NOW);
    if(!lib){
        return;
    }
    m_current_level = lib;
    void(*load)(Runtime * ) = (void(*)(Runtime *))dlsym(lib, "load_level");
    if(!load){
        return;
    }
    load_level_func(load);
}
void Runtime::load_level_func(void (*load)(Runtime *)){
    reset();
    load(this);
}