#include "bullet.h" 
void Bullet::on_tick(){
    float dt = GetFrameTime();
    float dist = Vector2Length(m_velocity);
    Collision c = box_trace(this->get_location(),this->get_location()+m_velocity*dt, m_collision, m_this_ref);
    if(c.hit){
        Entity * e = get_entity(c.collided_with);
        if(e){
            c.collided_with = m_this_ref;
            e->on_damage(1,m_this_ref);
            destroy_entity(m_this_ref);
        }
        dist = Vector2Distance(Vector2{m_collision.x, m_collision.y}, c.location);
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
void Bullet::on_init(ResourceRef this_ref){
    m_this_ref = this_ref;
}
void Bullet::on_render(){
    DrawRectangleV(convert_world_to_screen(Vector2{m_collision.x, m_collision.y}), Vector2{m_collision.height, m_collision.width}, PINK);
}
void Bullet::on_destroy(){

}
Bullet::Bullet(Vector2 location, Vector2 velocity){
    m_collision.x = location.x-2;
    m_collision.y = location.y-2;
    m_collision.height = 4;
    m_collision.width = 4;
    m_velocity = velocity;
    m_lf = 2;
}