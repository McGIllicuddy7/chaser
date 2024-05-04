 #include "box.h"
 #include <stdlib.h>
 void Box::on_tick(){
  if( m_hit>0){
    m_hit -= GetFrameTime();
  }

 }
 void Box::on_init(Runtime *runtime, ResourceRef this_ref){
   m_runtime = runtime;
   m_this_ref = this_ref;
   set_location({float(rand()%10000-5000),float(rand()%10000-5000)});
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