#include "background.h"
#include "entids.h"
Building::Building(Vector2 location, float height){
    m_collision.x = location.x;
    m_collision.y = location.y;
    m_collision.height= location.y+10000;
    m_collision.width = 0;
    m_height = height;
    m_depth = 1;
    m_velocity = {50, 0};
}
void Building::on_tick(){
    m_collision.x += m_velocity.x;
    if(m_collision.x>5000){
        destroy_entity(m_this_ref);
    }
}
void Building::on_render(){
    DrawRectangleV(convert_world_to_screen(Vector2{m_collision.x, m_collision.y+m_height}), (Vector2){200,m_collision.height+m_height*2},WHITE);
}
size_t Building::get_id(){
    return (size_t)ent_id::background;
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
