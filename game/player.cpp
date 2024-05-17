 #include "player.h"
 #include "weapons.h"
Player::Player(ResourceRef manager){
    m_manager = manager;
}
 void Player::on_tick(){
      Vector2 old_loc = get_location();
      float dt = GetFrameTime();
      Vector2 input = {-0.0,0};
      float y_speed = 1;
      float x_speed = 0.25;
      if(IsKeyDown(KEY_W)){
          input.y +=-y_speed;
      }
      if(IsKeyDown(KEY_S)){
          input.y +=y_speed;
      }
      if(IsKeyDown(KEY_A)){
          input.x -= x_speed;
      }
      if(IsKeyDown(KEY_D)){
          input.x +=x_speed;
      }
      if(input.y>0 && disp_y>=1000){
          input.y = -1;
      }
      if(input.y<0 && disp_y<=-1000){
        input.y = 1;
      }
      if(input.x == 0){
        input.x = -m_momentum.x;
      }
      if(input.y == 0){
        input.y = -m_momentum.y;
      }
      m_momentum = m_momentum+input*dt*8;
      if(m_momentum.y>y_speed){
        m_momentum.y = y_speed;
      }
      if(m_momentum.y<-y_speed){
        m_momentum.y = -y_speed;
      }
      if(m_momentum.x>x_speed){
        m_momentum.x = x_speed;
      }
      if(m_momentum.x<-x_speed){
        m_momentum.x = -x_speed;
      }
      float dist = 200;
    Collision c = box_trace(this->get_location(),this->get_location()+m_momentum*dist*dt, m_collision, m_this_ref);
    if(c.hit){ 
        Entity * e = get_entity(c.collided_with);
        if(e){
          if(!(c.collided_with == m_this_ref)){
            c.collided_with = m_this_ref;
            e->on_collision(c);
            dist = Vector2Distance(Vector2{m_collision.x, m_collision.y}, c.location);
          }
        }
    }
    set_location(get_location()+m_momentum*dist*dt);
    disp_y += get_location().y-old_loc.y;
    Vector2 new_loc = get_location();
    m_velocity = (new_loc-old_loc)/GetFrameTime();
    const int sz = 100;
    if(IsKeyPressed(KEY_J)){
        fire_laser(get_location()+Vector2{5,-7}, Vector2{1,0}, m_this_ref);
        fire_laser(get_location()+Vector2{5,7}, Vector2{1,0}, m_this_ref);
    }
    if(IsKeyPressed(KEY_K)){
        fire_railgun(get_location()+Vector2{5,-0}, Vector2{1,0}, m_this_ref);
    }
    if(IsKeyPressed(KEY_L)){
        spawn_chaff(get_location()+Vector2{64,0},get_velocity()+Vector2{100,0}, m_this_ref);
    }
    if(IsKeyPressed(KEY_M)){
        fire_missile(get_location()+Vector2{64,0},get_velocity()+Vector2{100,(float)((rand()%2)*2-1)*300}, m_this_ref,2);
    }
 }
 void Player::on_init(ResourceRef this_ref){
    m_this_ref = this_ref;
    set_location({0,0});
    m_collision.height = 32;
    m_collision.width = 48;
    m_health = 4;
    disp_y = 0;
    m_texture = load_texture_by_name("friendly_ship_engines.png");
    set_entity_as_origin(this_ref,{200,0});
 }
 void Player::on_render(){
   Texture * tmp =get_texture(m_texture);
   if(tmp){
    DrawTextureV(*tmp, convert_world_to_screen(Vector2{m_collision.x, m_collision.y}), WHITE);
   }
  }

void Player::on_destroy(){
  Manager * mn = (Manager*)get_entity(m_manager);
  if(mn){
    mn->player_destroyed();
  }
}
void Player::on_collision(Collision col){

}
void Player::on_damage(float damage, ResourceRef other){
    m_health -= damage;
    if(m_health<=0){
      destroy_entity(m_this_ref);
      on_destroy();
    }
}
size_t Player::get_id(){
  return 1;
}
float Player::y_disp(){
  return disp_y;
}
