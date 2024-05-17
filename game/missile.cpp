#include "missile.h"
void Missile::on_tick(){
    float dt = GetFrameTime();
    if(!m_found_target){
        if(m_look_timer>0){
            m_look_timer-=dt;
        }
        else{
            m_look_timer -= dt;
            if(m_look_timer<-1){
                destroy_entity(m_this_ref);
                return;
            }
            float look_dist = 10000;
            for(float theta = 0; theta<2*PI; theta += 2*PI/16.0){
                Vector2 delta = Vector2{cos(theta-m_look_timer)*look_dist, sin(theta-m_look_timer)*look_dist};
                Collision c= line_trace(get_location(), get_location()+delta, m_this_ref);
                if(c.hit){
                    Entity * e= get_entity(c.collided_with);
                    if(e){
                        if(e->get_id() == m_target_id){
                            m_found_target = true;
                            m_target = c.collided_with;
                        }
                    }
                }
            }
        }
    }
    else{
        Entity *target = get_entity(m_target);
        if(!target){
            destroy_entity(m_this_ref);
            return;
        }
        Collision c = line_trace(get_location(),target->get_location(), m_this_ref);
        if(c.hit){
            if(!(c.collided_with == m_target)){
                destroy_entity(m_this_ref);
                return;
            }
            float d = Vector2Distance(get_location(),target->get_location());
            if(d<60){
                target->on_damage(4, m_this_ref);
                destroy_entity(m_this_ref);
                return;
            }
            Vector2 delta = target->get_location()-get_location();
            delta = Vector2Normalize(delta);
            m_velocity = delta*800;
        }
    }   
    float dist = Vector2Length(m_velocity);
    Collision c = box_trace(this->get_location(),this->get_location()+m_velocity*dt, m_collision, m_this_ref);
    if(c.hit){
        Entity * e = get_entity(c.collided_with);
        if(e && !(c.collided_with == m_firer)){
            c.collided_with = m_this_ref;
            e->on_damage(3,m_this_ref);
            destroy_entity(m_this_ref);
            dist = Vector2Distance(Vector2{m_collision.x, m_collision.y}, c.location);
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
void Missile::on_init(ResourceRef this_ref){
    m_this_ref = this_ref;
}
void Missile::on_render(){
    Color c = PINK;
    c.a = 200;
    int w = 8;
    DrawCircleV(convert_world_to_screen(Vector2{m_collision.x+m_collision.width/2, m_collision.y+m_collision.height/2}),w, c);
    c = WHITE;
    DrawRectangleV(convert_world_to_screen(Vector2{m_collision.x, m_collision.y}), Vector2{m_collision.height, m_collision.width}, c);
}
void Missile::on_destroy(){

}
Missile::Missile(Vector2 location, Vector2 velocity, ResourceRef firer, size_t target_id){
    m_collision.x = location.x-2;
    m_collision.y = location.y-2;
    m_collision.height = 4;
    m_collision.width = 4;
    m_velocity = velocity;
    m_firer = firer;
    m_lf = 10;
    m_depth = 3;
    m_look_timer = 0.5;
    m_target_id = target_id;
    m_found_target = false;
}