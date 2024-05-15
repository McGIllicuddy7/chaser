 #include "player.h"
Player::Player(ResourceRef manager){
    m_manager = manager;
}
 void Player::on_tick(){
      float dt = GetFrameTime();
      Vector2 input = {-0.0,0};
      if(IsKeyDown(KEY_W)){
          input.y +=-1;
      }
      if(IsKeyDown(KEY_S)){
          input.y +=1;
      }
      if(IsKeyDown(KEY_A)){
          input.x -= 0.125;
      }
      if(IsKeyDown(KEY_D)){
          input.x +=0.125;
      }
      if(input.y>0 && disp_y>500){
          input.y = 0;
      }
      if(input.y<0 && disp_y<-500){
        input.y = 0;
      }
      float dist = 500;
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
    disp_y += get_location().y;
    const int sz = 100;
 }
 void Player::on_init(ResourceRef this_ref){
    m_this_ref = this_ref;
    set_location({0,0});
    m_collision.height = 27;
    m_collision.width = 32;
    m_texture = load_texture_by_name("trollface.png");
    set_entity_as_origin(this_ref, get_location());
 }
 void Player::on_render(){
  /*
   Texture * tmp =get_texture(m_texture);
   if(tmp){
    DrawTextureV(*tmp, convert_world_to_screen(Vector2{m_collision.x, m_collision.y}), WHITE);
   }*/
   DrawRectangleV(convert_world_to_screen(Vector2{m_collision.x, m_collision.y}),{32,27}, WHITE);
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