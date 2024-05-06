#include "entity.h"
#include "ops.h"
void cleanup(Entity * entity){
    (entity)->on_destroy();
}
 void Entity::on_tick(){
 }
 void Entity::on_init(Runtime *runtime, ResourceRef this_ref){
   m_runtime = runtime;
   m_this_ref = this_ref;
 }
 void Entity::on_render(){

 }
 Vector2 Entity::get_location(){
    return {m_collision.x+m_collision.width/2, m_collision.y+m_collision.height/2};
 }
  Vector2 Entity::get_velocity(){
    return m_velocity;
 }
void Entity::set_velocity(Vector2 velocity){
   m_velocity = velocity;
}
void Entity::set_location(Vector2 location){
      m_collision.x = location.x-m_collision.width/2;
      m_collision.y = location.y-m_collision.height/2;
}
void Entity::on_destroy(){

}
void Entity::on_collision(Collision col){

}
void Entity::on_damage(float damage,ResourceRef other){
}
EntityBB Entity::get_bb(){
   return {this->m_this_ref, this->m_collision};
}
