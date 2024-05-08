#include "runtime.h"
#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <raymath.h>
extern void init_script(Runtime * runtime);
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
    for(int j = 0; j<num_layers; j++){
        for(int i =0; i<m_to_draw[j].size(); i++){
            Entity * e = get_entity(m_to_draw[j][i]);
            if(e){
                e->on_render();
            } else{
                m_to_draw[j].erase(m_to_draw[j].begin()+i, m_to_draw[j].begin()+i);
                i--;
            }
        }
    }
    DrawFPS(600,80);
    EndDrawing();
}
void Runtime::Run(){
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(m_screen_width, m_screen_height, m_name.c_str());
    SetTargetFPS(60);
    init_script(this);
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
    int depth = e->get_render_depth();
    if(depth<0 || depth >=num_layers){
        goto done;
    }
    m_to_draw[depth].push_back(handle);
    done:
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
    float minx = 100000000000;
    float miny = 100000000000;
    float maxx = -100000000000;
    float maxy = -100000000000;
    for(int i =0; i<m_entities.cache_size(); i++){
        Entity * e = m_entities.get_unchecked(i);
        if (e){
            EntityBB g = e->get_bb();
            if(g.box.x<minx){
                minx = g.box.x;
            }
            if (g.box.x+g.box.width>maxx){
                maxx = g.box.x+g.box.width;
            }
            if(g.box.y<miny){
                miny = g.box.y;
            }
            if (g.box.y+g.box.height>maxy){
                maxy= g.box.y+g.box.height;
            }
            m_collisions.push_back(g);
        }
    }
    m_col_tree.initialize(m_collisions, {minx, miny}, {maxx, maxy});
}
Collision Runtime::box_trace(Vector2 start, Vector2 end, Rectangle rec,ResourceRef to_ignore){
    return m_col_tree.box_trace(start, end, rec, m_collisions, to_ignore);
}
int Runtime::screen_height(){
    return m_screen_height;
}
int Runtime::screen_width(){
    return m_screen_width;
}
void Runtime::destroy_entity(ResourceRef ref){
    m_entities.remove(ref);
}
