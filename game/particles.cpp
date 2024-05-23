#include "particles.h"
#include "engine.h"
#include "entids.h"
LaserBeam::LaserBeam(Vector2 start, Vector2 end, ResourceRef Parent){
    m_start = start;
    Entity * p = get_entity(Parent);
    if(p){
        m_start = start-p->get_location();
    }
    m_end = end;
    timer = 0.1;
    m_parent = Parent;
    m_collision.width = 0;
    m_collision.height = 0;
}
void LaserBeam::on_tick(){
    timer -= GetFrameTime();
    if(timer<0){
        destroy_entity(m_this_ref);
    }
}
void LaserBeam::on_render(){
    Entity * p = get_entity(m_parent);
    if(p){
        DrawLineEx(convert_world_to_screen(p->get_location()+m_start),convert_world_to_screen(m_end), 1, {255, 150,150,255});
    }
}
size_t LaserBeam::get_id(){
    return (size_t)(ent_id::particle); 
}
Chaff::Chaff(Vector2 location, Vector2 velocity){
    m_collision = {0,0, 40, 40};
    set_location(location);
    remaining_life = 10;
    m_velocity = velocity;
    m_depth = 4;
}
void Chaff::on_tick(){
    float dt = GetFrameTime();
    remaining_life -= dt;
    if(remaining_life<0){
        destroy_entity(m_this_ref);
    }
    set_location( get_location()+ m_velocity*dt);
    m_collision.x -= 5*dt;
    m_collision.y -= 5*dt;
    m_collision.height += 10*dt;
    m_collision.width += 10*dt;
}
void Chaff::on_render(){
    DrawRectangleV(convert_world_to_screen(Vector2{m_collision.x, m_collision.y}), {m_collision.width, m_collision.height}, {255, 255, 255,(unsigned char)Lerp(0,255, (remaining_life/10)*(remaining_life/10))});
    

}
void Chaff::on_damage(float damage, ResourceRef damager){

}
size_t Chaff::get_id(){
    return (size_t)(ent_id::particle);
}
ShipExplosion::ShipExplosion(Vector2 location){
    m_collision.height = 0;
    m_collision.width = 0;
    m_collision.x = location.x;
    m_collision.y = location.y;
    time = 0;
    frames[0] = load_texture_by_name("explosion/explosion1.png");
    frames[1] = load_texture_by_name("explosion/explosion2.png");
    frames[2] = load_texture_by_name("explosion/explosion3.png");
    frames[3] = load_texture_by_name("explosion/explosion4.png");
    frames[4] = load_texture_by_name("explosion/explosion5.png");
    frames[5] = load_texture_by_name("explosion/explosion6.png");
    frames[6] = load_texture_by_name("explosion/explosion7.png");
    frames[7] = load_texture_by_name("explosion/explosion8.png");
    frames[8] = load_texture_by_name("explosion/explosion9.png");
    frames[9] = load_texture_by_name("explosion/explosion10.png");
    frames[10] = load_texture_by_name("explosion/explosion11.png");
    frames[11] = load_texture_by_name("explosion/explosion12.png");
    frames[12] = load_texture_by_name("explosion/explosion13.png");
}
void ShipExplosion::on_tick(){
    time += GetFrameTime();
    if(time >0.5){
        destroy_entity(m_this_ref);
    }
}
void ShipExplosion::on_render(){
    int idx = round((time)*6.5);
    if(idx<0) idx = 0;
    if(idx>12) idx = 12;
    Texture * texture = get_texture(frames[idx]);
    DrawTextureV(*texture, convert_world_to_screen(Vector2{m_collision.x-25, m_collision.y-25}), WHITE);
}
size_t get_id(){
    return (size_t)(ent_id::particle); 
}