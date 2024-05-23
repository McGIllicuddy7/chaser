#include "bolt.h" 
#include "entids.h"
void Bolt::on_tick(){
    float dt = GetFrameTime();
    float dist = Vector2Length(m_velocity);
    Collision c = box_trace(this->get_location(),this->get_location()+m_velocity*dt, m_collision, m_this_ref);
    if(c.hit){
        Entity * e = get_entity(c.collided_with);
        if(e && !(c.collided_with == m_firer)){
            if(true){
                c.collided_with = m_this_ref;
                e->on_damage(3,m_this_ref);
                destroy_entity(m_this_ref);
                return;
            }
        }
    } 
    c = line_trace(this->get_location(),this->get_location()+m_velocity*dt, m_this_ref);
    if(c.hit){
        Entity * e = get_entity(c.collided_with);
        if(e &&!(c.collided_with == m_firer)){
          if(true){
                c.collided_with = m_this_ref;
                e->on_damage(1,m_this_ref);
                destroy_entity(m_this_ref);
                return;
            }
        }
    }
    if(dist>0){
        Vector2 v = Vector2Normalize(m_velocity);
        m_collision.x += v.x*dist*dt;
        m_collision.y += v.y*dist*dt;
    }
    m_lf -= GetFrameTime();
    if(m_lf<0){
        destroy_entity(m_this_ref);
    }
}
void Bolt::on_init(ResourceRef this_ref){
    m_this_ref = this_ref;
}
void Bolt::on_render(){
    Color c = WHITE;
    c.a = 200;
    int w =4;
   // DrawCircleV(convert_world_to_screen(Vector2{m_collision.x+m_collision.width/2, m_collision.y+m_collision.height/2}),w, c);
    c = {255,0,0,125};
    DrawRectangleV(convert_world_to_screen(Vector2{m_collision.x-w/2, m_collision.y-w/2}), Vector2{m_collision.width+w, m_collision.height+w}, c);
    DrawRectangleV(convert_world_to_screen(Vector2{m_collision.x, m_collision.y}), Vector2{m_collision.width, m_collision.height}, WHITE);

}
void Bolt::on_destroy(){

}
Bolt::Bolt(Vector2 location, Vector2 velocity, ResourceRef firer){
    m_collision.x = location.x-2;
    m_collision.y = location.y-2;
    m_collision.height =1;
    m_collision.width = 25;
    m_velocity = velocity;
    m_firer = firer;
    m_lf = 1;
    m_depth = 3;
}