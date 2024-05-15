 #include "enemy.h"
 #include "bullet.h"
Enemy::Enemy(ResourceRef manager, Vector2 location){
    m_manager = manager;
    m_collision.x = location.x;
    m_collision.y = location.y;
    m_health = 1;
}
 void Enemy::on_tick(){ 
    float dt = GetFrameTime();
    Vector2 input = {float(rand()%8-4)/8,float((rand()%2)-1)};
    if(get_location().y>0){
        if(rand()%4 == 0&&!(get_location().y>320)){
            input.y = 1;
        } else{
            input.y = -1;
        }
    } else if (get_location().y<0){
        if(rand()%2== 0 && !(get_location().y<-320)){
            input.y = -1;
        } else{
            input.y = 1;
        }
    }
    float dist = 400;
    Collision c = box_trace(this->get_location(),this->get_location()+input*dist*dt, m_collision, m_this_ref);
    if(c.hit){
        Entity * e = get_entity(c.collided_with);
        if(e){
            c.collided_with = m_this_ref;
            e->on_collision(c);
        }
        dist = Vector2Distance(Vector2{m_collision.x, m_collision.y}, c.location);
    }
    if(rand()%64 == 0){
        Bullet * a = new Bullet(get_location()+Vector2{-20,-10}, {-600,0});
        Bullet * b = new Bullet(get_location()+Vector2{-20,10}, {-600,0});
        register_entity(a);
        register_entity(b);
    }
    set_location(get_location()+input*dist*dt);
    const int sz = 100;
 }
 void Enemy::on_init(ResourceRef this_ref){
    m_this_ref = this_ref;
    m_collision.height = 27;
    m_collision.width = 32;
    m_texture = load_texture_by_name("trollface.png");
 }
 void Enemy::on_render(){
    Texture * tmp =get_texture(m_texture);
    DrawTextureV(*tmp, convert_world_to_screen(Vector2{m_collision.x, m_collision.y}), WHITE);
}
void Enemy::on_damage(float damage, ResourceRef Other){
    m_health -= damage;
    if(m_health<=0){
        destroy_entity(m_this_ref);
    }
}
void Enemy::on_destroy(){
    Manager * mn = (Manager*)get_entity(m_manager);
    mn->ship_destroyed(m_this_ref);
}