#include "weapons.h"
#include "particles.h"
Collision fire_laser(Vector2 start, Vector2 direction, ResourceRef ref){
    Collision col = line_trace(start, start+direction*1000,ref);
    Vector2 hit_loc = start+direction*1000;
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
    return;
}