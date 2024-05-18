 #include "enemy.h"
 #include "bullet.h"
 #include "weapons.h"
 #include "player.h"
 #include "entids.h"
Enemy::Enemy(ResourceRef manager, Vector2 location){
    m_manager = manager;
    m_collision.x = location.x;
    m_collision.y = location.y;
    m_health = 1;
}
Entity * Enemy::get_player(){
    Manager * m = (Manager *)get_entity(m_manager);
    if(!m){
        return 0;
    }
    return m->get_player();
}
Vector2 Enemy::calculate_input(){
    Vector2 input = Vector2{.x =(float)(((double)(rand()%2-1))/3.0),.y =float((rand()%3)-1)};
    Entity * e =get_player();
    if(!e){
        return input;
    }
    Player * p = (Player *)e;
    float delta = (get_location().y-e->get_location().y);
    if(delta>=50 || (p->y_disp()>500 && fabs(delta)<200)){
        input.y = -1;
    } else if (delta<=-50|| (p->y_disp()<500 && fabs(delta)<200)){
        input.y = 1;
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
    float dist = 50;
    Collision c0 = line_trace(this->get_location(),this->get_location()+m_momentum*dist, m_this_ref);
    if(c0.hit){
        if((m_momentum.y>0 && input.y> 0 )|| (m_momentum.y<0 && input.y<0)){
            input.y *= -1;
        }
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
            if((ent_id)e->get_id()!= ent_id::particle){
                c.collided_with = m_this_ref;
                e->on_collision(c);
                dist = Vector2Distance(Vector2{m_collision.x, m_collision.y}, c.location);
            }
        }
    }
    set_location(get_location()+m_momentum*dist*dt);
    Vector2 new_loc = get_location();
    m_velocity = (new_loc-old_loc)/GetFrameTime();
}
void Enemy::handle_firing(){
    const float shot_min = 0.8;
    if(shot_timer<=0){
        Collision c = line_trace(get_location()+Vector2{-32,0}, get_location()+Vector2{-10000,0},m_this_ref);
        if(!c.hit){
            Player * p= (Player*)get_player();
            if(!p){
                return;
            }
            Vector2 to_player = p->get_location()-get_location();
            to_player = Vector2Normalize(to_player);
            float delta = Vector2DotProduct(to_player, Vector2{-1,0});
            if(delta>0.9){
                if(reflex<=0){
                    reflex = 0.2;
                }
                reflex -=GetFrameTime();
                if(reflex<0){
                    shot_timer = shot_min;
                    fire_laser(get_location()+Vector2{-32,-5}, Vector2{-1,0}, m_this_ref);
                    fire_laser(get_location()+Vector2{-32,5}, Vector2{-1,0}, m_this_ref);
                }
            }
        }
        Entity * e = get_entity(c.collided_with);
        if(e->get_id()  != 1){
            return;
        }
        if(reflex<=0){
            reflex = 0.2;
        }
        reflex -= GetFrameTime();
        if(reflex<=0){
            shot_timer = shot_min;
            fire_laser(get_location()+Vector2{-32,-5}, Vector2{-1,0}, m_this_ref);
            fire_laser(get_location()+Vector2{-32,5}, Vector2{-1,0}, m_this_ref);
        }
    }
}
 void Enemy::on_tick(){ 
    if(shot_timer>0){
        shot_timer-= GetFrameTime();
    }
    handle_movement();
    handle_firing();
 }
 void Enemy::on_init(ResourceRef this_ref){
    m_this_ref = this_ref;
    m_collision.height = 32;
    m_collision.width = 48;
    m_health = 2;
    shot_timer = 0;
    m_texture = load_texture_by_name("enemy_ship_engines.png");
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
    if((void *)mn>(void *)4095){
        mn->ship_destroyed(m_this_ref);
    }
    ResourceRef r= load_sound_by_name("ship_explosion.wav");
    Sound * s = get_sound(r);
    if(s){
        PlaySound(*s);
    }
}
size_t Enemy::get_id(){
    return (size_t)(ent_id::enemy);
}
