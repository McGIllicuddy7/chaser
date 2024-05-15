#include "engine.h"
#include "manager.h"
class Enemy:public Entity{
    ResourceRef m_texture;
    ResourceRef m_manager;
    public:
    Enemy(ResourceRef manager);
    void on_tick();
    void on_init(ResourceRef this_ref);
    void on_render();
    void on_destroy();
};