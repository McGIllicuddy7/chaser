 #include "player.h"

 void Player::on_tick(){
    float dt = GetFrameTime();
    Vector2 input = {0,0};
    if(IsKeyDown(KEY_W)){
        input.y +=-1;
    }
    if(IsKeyDown(KEY_S)){
        input.y +=1;
    }
    if(IsKeyDown(KEY_A)){
        input.x -= 1;
    }
    if(IsKeyDown(KEY_D)){
        input.x +=1;
     }
   set_location(get_location()+input*200*dt);

   Collision c = m_runtime->line_trace(get_location(), get_location()+Vector2{100, 0}, m_this_ref);
   if(c.hit){
      Entity *e = m_runtime->get_entity(c.collided_with);
      c.collided_with = m_this_ref;
      e->on_collision(c);
   } 
   c = m_runtime->line_trace(get_location(), get_location()+Vector2{-100, 0}, m_this_ref);
   if(c.hit){
      Entity *e = m_runtime->get_entity(c.collided_with);
      c.collided_with = m_this_ref;
      e->on_collision(c);
   }
     c = m_runtime->line_trace(get_location(), get_location()+Vector2{0,-100}, m_this_ref);
   if(c.hit){
      Entity *e = m_runtime->get_entity(c.collided_with);
      c.collided_with = m_this_ref;
      e->on_collision(c);
   }
    c = m_runtime->line_trace(get_location(), get_location()+Vector2{0,100}, m_this_ref);
   if(c.hit){
      Entity *e = m_runtime->get_entity(c.collided_with);
      c.collided_with = m_this_ref;
      e->on_collision(c);
   }
 }
 void Player::on_init(Runtime *runtime, ResourceRef this_ref){
   m_runtime = runtime;
   m_this_ref = this_ref;
   set_location({0,0});
   runtime->set_entity_as_origin(this_ref, get_location());
 }
 void Player::on_render(){
    DrawRectangleV(m_runtime->convert_world_to_screen(Vector2{m_collision.x, m_collision.y}-Vector2{16,16}), Vector2{32, 32,}, RAYWHITE);
    DrawLineV(m_runtime->convert_world_to_screen(get_location()), m_runtime->convert_world_to_screen(get_location()+Vector2{100, 0}), GREEN);
    DrawLineV(m_runtime->convert_world_to_screen(get_location()), m_runtime->convert_world_to_screen(get_location()+Vector2{-100, 0}), GREEN);
    DrawLineV(m_runtime->convert_world_to_screen(get_location()), m_runtime->convert_world_to_screen(get_location()+Vector2{0, 100}), GREEN);
    DrawLineV(m_runtime->convert_world_to_screen(get_location()), m_runtime->convert_world_to_screen(get_location()+Vector2{0, -100}), GREEN);
 }

void Player::on_destroy(){

}