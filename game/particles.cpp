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
    m_collision = {0,0,80, 0};
    set_location(location);
    m_collision.y -= 20;
    remaining_life = 10;
    m_velocity = velocity;
    m_velocity.x +=(rand()%100)-50;
    m_velocity.y +=(rand()%20);
    m_depth = 4;
}
void Chaff::on_tick(){
    float dt = GetFrameTime();
    remaining_life -= dt;
    if(remaining_life<0){
        destroy_entity(m_this_ref);
    }
    set_location( get_location()+ m_velocity*dt);
    m_collision.x -= 8*dt;
    m_collision.y -= 8*dt;
    m_collision.width += 16*dt;
    float vel_scale = Vector2Length(m_velocity);
    if(vel_scale>0.1){
        Vector2 vel_norm = Vector2Normalize(m_velocity);
        m_velocity =m_velocity- vel_scale*0.01*dt*vel_norm;
    } else{
        m_velocity = {0};
    }
    m_velocity.y += 10*dt;
}
void Chaff::on_render(){
    //DrawRectangleV(convert_world_to_screen(Vector2{m_collision.x, m_collision.y}), {m_collision.width, m_collision.height}, {255, 255, 255,(unsigned char)Lerp(0,255, (remaining_life/10)*(remaining_life/10))});
    ResourceRef r = load_texture_by_name("fog.png");
    Texture * tex = get_texture(r);
    float scale = m_collision.width/100;
    DrawTextureEx(*tex,convert_world_to_screen(Vector2{m_collision.x, m_collision.y}),0,scale, {255, 255, 255,(unsigned char)Lerp(0,255, (remaining_life/10)*(remaining_life/10))});

}
void Chaff::on_damage(float damage, ResourceRef damager){

}
size_t Chaff::get_id(){
    return (size_t)(ent_id::particle);
}
ShipExplosion::ShipExplosion(Vector2 location, Vector2 velocity){
    m_depth = 3;
    m_collision.height = 0;
    m_collision.width = 0;
    m_collision.x = location.x;
    m_collision.y = location.y;
    m_velocity = velocity;
    time = 0;
    frames[0] = load_texture_by_name("ship_explosion/ship_explosion1.png");
    frames[1] = load_texture_by_name("ship_explosion/ship_explosion2.png");
    frames[2] = load_texture_by_name("ship_explosion/ship_explosion3.png");
    frames[3] = load_texture_by_name("ship_explosion/ship_explosion4.png");
    frames[4] = load_texture_by_name("ship_explosion/ship_explosion5.png");
    frames[5] = load_texture_by_name("ship_explosion/ship_explosion6.png");
    frames[6] = load_texture_by_name("ship_explosion/ship_explosion7.png");
    frames[7] = load_texture_by_name("ship_explosion/ship_explosion8.png");
    frames[8] = load_texture_by_name("ship_explosion/ship_explosion9.png");
    frames[9] = load_texture_by_name("ship_explosion/ship_explosion10.png");
    frames[10] = load_texture_by_name("ship_explosion/ship_explosion11.png");
    frames[11] = load_texture_by_name("ship_explosion/ship_explosion12.png");
    frames[12] = load_texture_by_name("ship_explosion/ship_explosion13.png");
    frames[13] = load_texture_by_name("ship_explosion/ship_explosion14.png"); 
}
void ShipExplosion::on_tick(){
    time += GetFrameTime();
    if(time >0.5){
        destroy_entity(m_this_ref);
    }
    set_location(get_location()+m_velocity*GetFrameTime());
}
void ShipExplosion::on_render(){
    int idx = round((time)*14*2);
    if(idx<0) idx = 0;
    if(idx>13) idx = 13;
    Texture * texture = get_texture(frames[idx]);
    DrawTextureV(*texture, convert_world_to_screen(Vector2{m_collision.x-50, m_collision.y-50}), {255, 255, 255, 128});
}
size_t ShipExplosion::get_id(){
    return (size_t)(ent_id::particle); 
}
static Pool<LaserBeam,100> lasers;
static Pool<Chaff, 100> chaffs;
static Pool<ShipExplosion, 100>  ship_explosions;
void LaserBeam::free_memory(){
    lasers.mfree(this);
}
void Chaff::free_memory(){
    chaffs.mfree(this);
}
void ShipExplosion::free_memory(){
    ship_explosions.mfree(this);
}
ResourceRef new_laser(Vector2 start, Vector2 end, ResourceRef Parent){
    LaserBeam * b = POOL_ALLOC(LaserBeam, lasers, start, end, Parent);
    return register_entity(b);
}
ResourceRef new_chaff(Vector2 start, Vector2 velocity){
    Chaff * b = POOL_ALLOC(Chaff, chaffs, start, velocity);
    return register_entity(b);
}
ResourceRef new_ship_explosion(Vector2 location, Vector2 velocity){
    ShipExplosion * b = POOL_ALLOC(ShipExplosion, ship_explosions, location, velocity);
    return register_entity(b);
}