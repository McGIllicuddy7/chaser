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
    new_laser(start, hit_loc, ref);
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
    new_bullet(start, dir*6000, ref);
    ResourceRef r= load_sound_by_name("railgun.wav");
    Sound * s = get_sound(r);
    if(s){
        PlaySound(*s);
    }
}
void spawn_chaff(Vector2 location, Vector2 velocity,ResourceRef ref){
    new_chaff(location, velocity);
}
void fire_missile(Vector2 location, Vector2 velocity, ResourceRef ref, size_t target_id){
    new_missile(location, velocity, ref, target_id);
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
    Entity * e = get_entity(ref);
    Vector2 vel = {};
    if(e){
        vel = e->get_velocity();
    }
    new_bolt(start, dir*850+vel, ref);
    ResourceRef r= load_sound_by_name("laserShoot.wav");
    Sound * s = get_sound(r);
    if(s){
        PlaySound(*s);
    }
}