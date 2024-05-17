#include "particles.h"
#include "engine.h"
int count = 0;
LaserBeam::LaserBeam(Vector2 start, Vector2 end, ResourceRef Parent){
    m_start = start;
    Entity * p = get_entity(Parent);
    if(p){
        m_start = start-p->get_location();
    }
    m_end = end;
    timer = 0.1;
    m_parent = Parent;
    printf("%d\n", count++);
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
