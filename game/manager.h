#pragma once 
#include "engine.h"
#include <vector>
class Manager:public Entity{
    bool started;
    bool end_screen;
    void * m_runtime;
    int ship_count;
    int desired_ship_count;
    ResourceRef player;
    std::vector<ResourceRef> ships;
    public:
    Manager(void *runtime);
    void on_tick();
    void on_init( ResourceRef this_ref);
    void on_render();
    void player_destroyed();
    void ship_destroyed(ResourceRef ship_ref);
    Entity * get_player();
};