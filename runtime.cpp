#include "runtime.h"
#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <raymath.h>
Runtime::~Runtime(){
    CloseWindow();
}
Runtime::Runtime( int width,int height, const char * name){
    this->m_name = name;
    this->m_screen_height = height;
    this->m_screen_width = width;
    srand(time(0));
}
Runtime* Runtime:: New(){
    return new Runtime(640*1.5, 480*1.5, "hello sailor");
}
void Runtime::set_relative_locations(){
    Entity * origin = get_entity(m_origin_entity);
    if(!origin){
        return;
    }
    Vector2 loc = origin->get_location();
    for(int i =0; i<m_entities.cache_size(); i++){
        Entity * e = m_entities.get_unchecked(i);
        if (e){
            e->set_location(Vector2Subtract(e->get_location(), loc));
        }
    }
}
void Runtime::Tick(){
    for(int i =0; i<m_entities.cache_size(); i++){
        Entity * e = m_entities.get_unchecked(i);
        if (e){
            e->on_tick();
        }
    }
}
void Runtime::Render(){
    BeginDrawing();
    ClearBackground(BLACK);
    for(int i =0; i<m_entities.cache_size(); i++){
        Entity * e = m_entities.get_unchecked(i);
        if (e){
            (e)->on_render();
        }
    }
    DrawFPS(600,40);
    EndDrawing();
}
void Runtime::Run(){
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(m_screen_width, m_screen_height, m_name.c_str());
    while(!WindowShouldClose()){
        frame_collision_set_up();
        Tick();
        if(m_use_entity_as_origin){
            set_relative_locations();
        }
        Render();
    }
}
Vector2 Runtime::convert_world_to_screen(Vector2 v) const{
    return Vector2Add(Vector2Subtract(v, m_camera_location), Vector2{float(m_screen_width)/2, float(m_screen_height)/2});
}
ResourceRef Runtime::register_entity(Entity *e){
    auto handle = m_entities.emplace(e);
    e->on_init(this, handle);
    return handle;
}
Entity * Runtime::get_entity(ResourceRef ref){
    return this->m_entities.get(ref);
}
void Runtime::set_entity_as_origin(ResourceRef ref, Vector2 Offset){
    m_origin_entity = ref;
    m_use_entity_as_origin = true;
}
void Runtime::unset_entity_as_origin(){
    m_use_entity_as_origin = false;
}
bool Runtime::origin_set(){
    return m_use_entity_as_origin;
}

void Runtime::frame_collision_set_up(){
    m_collisions.clear();
    for(int i =0; i<m_entities.cache_size(); i++){
        Entity * e = m_entities.get_unchecked(i);
        if (e){
            m_collisions.push_back(e->get_bb());
        }
    }
}
Collision Runtime::line_trace(Vector2 start, Vector2 end,ResourceRef to_ignore){
    float min_dist = Vector2LengthSqr(end-start);
    float dist = min_dist;
    Collision out = {false};
    for(int i = 0; i<m_collisions.size(); i++){
        ResourceRef rf = m_collisions[i].Parent;
        if(rf == to_ignore){
            continue;
        }
        Rectangle bx = m_collisions[i].box;
        Vector2 c1 = {bx.x, bx.y};
        Vector2 c2 = {bx.x+bx.width, bx.y};
        Vector2 c3 = {bx.x, bx.y+bx.height};
        Vector2 c4 = {bx.x+bx.width, bx.y+bx.height};
        if (Vector2DistanceSqr(start, c1)>dist &&Vector2DistanceSqr(start, c2)>dist &&Vector2DistanceSqr(start, c3)>dist &&Vector2DistanceSqr(start, c4)>dist ){
            continue;
        }
        Vector2 col = {0,0};
        if (CheckCollisionLines(start, end, c1, c2, &col)){
            float d = Vector2LengthSqr(start-col);
            if(d<min_dist){
                out = {true, col, {-1,0}, rf};
                min_dist = d;
            }
        }
        if (CheckCollisionLines(start, end, c1, c3, &col)){
            float d = Vector2LengthSqr(start-col);
            if(d<min_dist){
                out = {true, col, {0,-1}, rf};
                min_dist = d;
            }
        }
        if (CheckCollisionLines(start, end, c2, c4, &col)){
            float d = Vector2LengthSqr(start-col);
            if(d<min_dist){
                out = {true, col, {0,1}, rf};
                min_dist = d;
            }
        }
        if (CheckCollisionLines(start, end, c3, c4, &col)){
            float d = Vector2LengthSqr(start-col);
            if(d<min_dist){
                out = {true, col, {1,0}, rf};
                min_dist = d;
            }
        }
    }
    return out;
}
Collision Runtime::box_trace(Vector2 start, Vector2 end, Rectangle rec,ResourceRef to_ignore){
    return Collision{false};
}
int Runtime::screen_height(){
    return m_screen_height;
}
int Runtime::screen_width(){
    return m_screen_width;
}