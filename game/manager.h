#pragma once 
#include "engine.h"
#include <vector>
class Manager:public Entity{
    void * m_runtime;
    ResourceRef player;
    std::vector<ResourceRef> ships;
    public:
    Manager(void *runtime);
    void on_tick();
    void on_init( ResourceRef this_ref);
    void player_destroyed();
    void ship_destroyed(ResourceRef ship_ref);
};