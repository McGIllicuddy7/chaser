#include "weapons.h"
Collision fire_laser(Vector2 start, Vector2 direction, ResourceRef ref){
    Collision col = line_trace(start, start+direction*1000,ref);
    Vector2 hit_loc = start+direction*1000;
    col.location = hit_loc;
    if(col.hit){
        hit_loc = col.location;
        Entity * e= get_entity(col.collided_with);
        if(e){
            e->on_damage(1, ref);
        }
    }
    return col;
}
void fire_railgun(Vector2 start, Vector2 direction, ResourceRef ref){
    return;
}