 #include "enemy.h"
 #include "bullet.h"
 #include "weapons.h"
Enemy::Enemy(ResourceRef manager, Vector2 location){
    m_manager = manager;
    m_collision.x = location.x;
    m_collision.y = location.y;
    m_health = 1;
}
Vector2 Enemy::calculate_input(){
    Vector2 input = Vector2{.x =(float)(((double)(rand()%2-1))/3.0),.y =float((rand()%2)-1)};
    if(get_location().y>0){
        if(rand()%4 == 0&&!(get_location().y>320)){
            input.y = 1;
        } else{
            input.y = -1;
        }
    } else if (get_location().y<0){
        if(rand()%4== 0 && !(get_location().y<-320)){
            input.y = -1;
        } else{
            input.y = 1;
        }
    }
    if(get_location().x<400){
        input.x =0.33;
    }
    if(get_location().x>500){
        input.x = -0.33;
    }
    return input;
}
void Enemy::handle_movement(){
    Vector2 old_loc = get_location();
    float dt = GetFrameTime();
    Vector2 input = calculate_input();
    Vector2 old_momentum = m_momentum;
    m_momentum = m_momentum+input*4*dt;
    float dist = 300;
    Collision c0 = line_trace(this->get_location(),this->get_location()+m_momentum*dist, m_this_ref);
    if(c0.hit){
        input.y *= -1;
    }
    m_momentum = old_momentum+input*4*dt;
    if(m_momentum.x>0.33){
        m_momentum.x = 0.33;
    }
    if(m_momentum.x<-0.33){
        m_momentum.x = -0.33;
    } 
    if(m_momentum.y>1){
        m_momentum.y = 1;
    }
    if(m_momentum.x<-1){
        m_momentum.y = -1;
    }
    Collision c = box_trace(this->get_location(),this->get_location()+m_momentum*dist*dt, m_collision, m_this_ref);
    if(c.hit){
        Entity * e = get_entity(c.collided_with);
        if(e){
            c.collided_with = m_this_ref;
            e->on_collision(c);
        }
        dist = Vector2Distance(Vector2{m_collision.x, m_collision.y}, c.location);
    }
    set_location(get_location()+m_momentum*dist*dt);
    Vector2 new_loc = get_location();
    m_velocity = (new_loc-old_loc)/GetFrameTime();
}
void Enemy::handle_firing(){
    if(fired>0){
        fired -= GetFrameTime();
    }
    Collision c = line_trace(get_location()+Vector2{-32,0}, get_location()+Vector2{-64,0},m_this_ref);
    if(c.hit){
        return;
    }
    if(rand()%128 == 0){
        fired = 0.1;
        Collision a= fire_laser(get_location()+Vector2{-32,-10}, Vector2{-1,0}, m_this_ref);
        Collision b = fire_laser(get_location()+Vector2{-32,10}, Vector2{-1,0}, m_this_ref);
        hits[0] = a;
        hits[1] = b;

    }
}
 void Enemy::on_tick(){ 
    handle_movement();
    handle_firing();
 }
 void Enemy::on_init(ResourceRef this_ref){
    m_this_ref = this_ref;
    m_collision.height = 32;
    m_collision.width = 48;
    fired = 0;
    m_texture = load_texture_by_name("enemy_ship_engines.png");
 }
 void Enemy::on_render(){
    Texture * tmp =get_texture(m_texture);
    DrawTextureV(*tmp, convert_world_to_screen(Vector2{m_collision.x, m_collision.y}), WHITE);
    if(fired>0){
        DrawLineEx(convert_world_to_screen(get_location()+Vector2{-32,-10}), convert_world_to_screen(hits[0].location), 1, {255,0,0,64});
        DrawLineEx(convert_world_to_screen(get_location()+Vector2{-32,10}), convert_world_to_screen(hits[1].location), 1, {255,0,0,64});
    }
}
void Enemy::on_damage(float damage, ResourceRef Other){
    m_health -= damage;
    if(m_health<=0){
        destroy_entity(m_this_ref);
        on_destroy();
    }
}
void Enemy::on_destroy(){
    Manager * mn = (Manager*)get_entity(m_manager);
    mn->ship_destroyed(m_this_ref);
}