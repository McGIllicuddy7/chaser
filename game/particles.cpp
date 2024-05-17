#include "particles.h"
#include "engine.h"
LaserBeam::LaserBeam(Vector2 start, Vector2 end, ResourceRef Parent){
    m_start = start;
    Entity * p = get_entity(Parent);
    if(p){
        m_start = start-p->get_location();
    }
    m_end = end;
    timer = 0.1;
    m_parent = Parent;
    m_collision.width = 0;
}
void LaserBeam::on_tick(){
    timer -= GetFrameTime();
    if(timer<0){
        destroy_entity(m_this_ref);
    }
}
void LaserBeam::on_render(){
    Entity * p = get_entity(m_parent);
    if(p){
        DrawLineEx(convert_world_to_screen(p->get_location()+m_start),convert_world_to_screen(m_end), 1, {255, 150,150,255});
    }
}

Chaff::Chaff(Vector2 location, Vector2 velocity){
    m_collision = {0,0, 40, 40};
    set_location(location);
    remaining_life = 10;
    m_velocity = velocity;
    m_depth = 4;
}
void Chaff::on_tick(){
    float dt = GetFrameTime();
    remaining_life -= dt;
    if(remaining_life<0){
        destroy_entity(m_this_ref);
    }
    set_location( get_location()+ m_velocity*dt);
    m_collision.x -= 5*dt;
    m_collision.y -= 5*dt;
    m_collision.height += 10*dt;
    m_collision.width += 10*dt;
}
void Chaff::on_render(){
    DrawRectangleV(convert_world_to_screen(Vector2{m_collision.x, m_collision.y}), {m_collision.width, m_collision.height}, {255, 255, 255,(unsigned char)Lerp(0,255, (remaining_life/10)*(remaining_life/10))});
    

}
void Chaff::on_damage(float damage, ResourceRef damager){

}
