 #include "box.h"
 #include <stdlib.h>
 void Box::on_tick(){
  if( m_hit>0){
    m_hit -= GetFrameTime();
    if(m_hit<0){
      m_runtime->destroy_entity(m_this_ref);
    }
  }
 }
 void Box::on_init(Runtime *runtime, ResourceRef this_ref){
   m_runtime = runtime;
   m_this_ref = this_ref;
   set_location({float(rand()%2000-1000),float(rand()%2000-1000)});
   m_collision.width = 32;
   m_collision.height = 32;
 }
 void Box::on_render(){
  Vector2 loc = m_runtime->convert_world_to_screen({m_collision.x, m_collision.y});
  if( m_hit<0.1){
    DrawRectangleV(m_runtime->convert_world_to_screen({m_collision.x, m_collision.y}), Vector2{32, 32,}, BLUE);
  }
  else{
    DrawRectangleV(m_runtime->convert_world_to_screen({m_collision.x, m_collision.y}), Vector2{32, 32,}, RED);
  }
 }
void Box::on_collision(Collision col){
    m_hit = 0.5;
}