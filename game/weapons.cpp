#include "weapons.h"
#include "particles.h"
#include "missile.h"
#include "bullet.h"
#include "bolt.h"
Collision fire_laser(Vector2 start, Vector2 direction, ResourceRef ref){
    Collision col = line_trace(start, start+direction*1000,ref);
    Vector2 hit_loc = start+direction*10000;
    if(col.hit){
        hit_loc = col.location;
        Entity * e= get_entity(col.collided_with);
        if(e){
            e->on_damage(1, ref);
        }
    }
    else{
        col.location = hit_loc;
    }
    LaserBeam * e = new LaserBeam(start, hit_loc, ref);
    register_entity(e);
    ResourceRef r= load_sound_by_name("laserShoot.wav");
    Sound * s = get_sound(r);
    if(s){
        PlaySound(*s);
    }
    return col;
}
void fire_railgun(Vector2 start, Vector2 direction, ResourceRef ref){
    Vector2 dir;
    if(direction.x != 0 || direction.y !=0 ){
        dir= Vector2Normalize(direction);
    }
    else{
        dir = {0,0};
    }
    Bullet * b =new Bullet(start, dir*6000, ref);
    register_entity(b);
    ResourceRef r= load_sound_by_name("railgun.wav");
    Sound * s = get_sound(r);
    if(s){
        PlaySound(*s);
    }
}
void spawn_chaff(Vector2 location, Vector2 velocity,ResourceRef ref){
    Chaff * c = new Chaff(location, velocity);
    register_entity(c);
}
void fire_missile(Vector2 location, Vector2 velocity, ResourceRef ref, size_t target_id){
    Missile * m = new Missile(location, velocity, ref, target_id);
    register_entity(m);
    ResourceRef r= load_sound_by_name("missile.wav");
    Sound * s = get_sound(r);
    if(s){
        PlaySound(*s);
    }
}
void fire_blaster(Vector2 start, Vector2 direction, ResourceRef ref){
    Vector2 dir;
    if(direction.x != 0 || direction.y !=0 ){
        dir= Vector2Normalize(direction);
    }
    else{
        dir = {0,0};
    }
    Bolt * b =new Bolt(start, dir*3000, ref);
    register_entity(b);
    ResourceRef r= load_sound_by_name("laserShoot.wav");
    Sound * s = get_sound(r);
    if(s){
        PlaySound(*s);
    }
}