#include "background.h"
#include "entids.h"
Pool<Building, 100> buildings;
Pool<Star, 1000> stars;
Building::Building(Vector2 location, float height){
    m_collision.x = location.x;
    m_collision.y = location.y;
    m_collision.height= location.y+10000;
    m_collision.width = 0;
    m_height = height;
    m_depth = 1;
    m_velocity = {50, 0};
    passed = false;
}
void Building::on_tick(){
    m_collision.x += m_velocity.x;
    if(m_collision.x>5000){
        destroy_entity(m_this_ref);
    } else if(!passed){
        if(m_collision.x <-300){
            ResourceRef ref = load_sound_by_name("buildingPass.wav");
            Sound * s = get_sound(ref);
            if(m_collision.y>-300){
                if(s){
                    passed = true;
                    PlaySound(*s);
                }
            }
        }
    }
}
void Building::on_render(){
    DrawRectangleV(convert_world_to_screen(Vector2{m_collision.x, m_collision.y+m_height}), (Vector2){200,m_collision.height+m_height*2},WHITE);
}
size_t Building::get_id(){
    return (size_t)ent_id::background;
}
void Building::free_memory(){
    buildings.mfree(this);
}
ResourceRef new_building(Vector2 location, float height){
    Building * b = POOL_ALLOC(Building, buildings, location, height);
    return register_entity(b);
}
Star::Star(Vector2 location, float radius){
    m_collision.x = location.x;
    m_collision.y = location.y;
    m_collision.height= 0;
    m_collision.width = 0;
    m_radius = radius;
    m_depth = 0;
    m_velocity = {25, 0};
}
void Star::on_tick(){
    m_collision.x += m_velocity.x;
    if(m_collision.x>5000){
        destroy_entity(m_this_ref);
    }
}
void Star::on_render(){
    DrawCircleV(convert_world_to_screen(Vector2{m_collision.x, m_collision.y+400}),m_radius,WHITE);
}
size_t Star::get_id(){
    return (size_t)ent_id::background;
}
void Star::free_memory(){
    stars.mfree(this);
}
ResourceRef new_star(Vector2 location, float radius){
    Star * b = POOL_ALLOC(Star,stars, location, radius);
    return register_entity(b);
}