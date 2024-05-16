#include "engine.h"
#include "manager.h"
class Enemy:public Entity{
    ResourceRef m_texture;
    ResourceRef m_manager;
    int m_health;
    Vector2 m_momentum;
    Vector2 calculate_input();
    void handle_movement();
    void handle_firing();
    public:
    Enemy(ResourceRef manager, Vector2 location);
    void on_tick();
    void on_init(ResourceRef this_ref);
    void on_render();
    void on_destroy();
    void on_damage(float damage, ResourceRef Other);
};