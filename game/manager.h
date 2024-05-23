#pragma once 
#include "engine.h"
#include <vector>
class Manager:public Entity{
    bool started;
    bool end_screen;
    void * m_runtime;
    int ship_count;
    int desired_ship_count;
    bool m_new_high_score;
    ResourceRef player;
    std::vector<ResourceRef> ships;
    size_t m_score;
    size_t m_high_score;
    public:
    Manager(void *runtime);
    void on_tick();
    void on_init( ResourceRef this_ref);
    void on_render();
    void player_destroyed();
    void ship_destroyed(ResourceRef ship_ref);
    Entity * get_player();
};