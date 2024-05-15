 #include "enemy.h"
Enemy::Enemy(ResourceRef manager){
    m_manager = manager;
}
 void Enemy::on_tick(){ 
    float dt = GetFrameTime();
    Vector2 input = {0,0};
    float dist = 0;
    Collision c = box_trace(this->get_location(),this->get_location()+input*dist*dt, m_collision, m_this_ref);
    if(c.hit){
        Entity * e = get_entity(c.collided_with);
        if(e){
            c.collided_with = m_this_ref;
            e->on_collision(c);
        }
        dist = Vector2Distance(Vector2{m_collision.x, m_collision.y}, c.location);
    }
    set_location(get_location()+input*dist*dt);
    const int sz = 100;
 }
 void Enemy::on_init(ResourceRef this_ref){
    m_this_ref = this_ref;
    set_location({0,0});
    m_collision.height = 27;
    m_collision.width = 32;
    m_texture = load_texture_by_name("trollface.png");
    set_entity_as_origin(this_ref, get_location());
 }
 void Enemy::on_render(){
    Texture * tmp =get_texture(m_texture);
    DrawTextureV(*tmp, convert_world_to_screen(Vector2{m_collision.x, m_collision.y}), WHITE);
}

void Enemy::on_destroy(){
    Manager * mn = (Manager*)get_entity(m_manager);
    mn->ship_destroyed(m_this_ref);
}