#include "runtime.h"
#include "raylib/raylib.h"
#include <stdlib.h>
#include <time.h>
#include "raylib/raymath.h"
#include <dlfcn.h>
extern void initscript(Runtime * runtime);
Runtime * rt =0;
Runtime::~Runtime(){
    reset();
    CloseWindow();
}
Runtime::Runtime( int width,int height, const char * name){
    this->m_name = name;
    this->m_screen_height = height;
    this->m_screen_width = width;
    srand(time(0));
}
Runtime* Runtime:: New(){
    Runtime * out =  new Runtime(640*1.5, 480*1.5, "hello sailor");
    rt = out;
    return out;
}
void Runtime::set_relative_locations(){
    Entity * origin = get_entity(m_origin_entity);
    if(!origin){
        return;
    }
    Vector2 loc = origin->get_location()+m_camera_location;
    for(int i =0; i<m_entities.cache_size(); i++){
        Entity * e = m_entities.get_unchecked(i);
        if (e){
            e->set_location(Vector2Subtract(e->get_location(), loc));
        }
    }
}
void Runtime::collect_dead(){
    for(int i=0; i<m_entity_destroy_queue.size(); i++){
        m_entities.remove(m_entity_destroy_queue[i]);
    }
    m_entity_destroy_queue.clear();
}
void Runtime::Tick(){
    for(int j =0; j<num_layers; j++){
        m_to_draw[j].clear();
    }
    for(int i =0; i<m_entities.cache_size(); i++){
        Entity * e = m_entities.get_unchecked(i);
        if (e){
            e->on_tick();
            if(e->get_render_depth()<0 || e->get_render_depth()>=num_layers){
                continue;
            } 
            m_to_draw[e->get_render_depth()].push_back(ResourceRef{(size_t)i, (size_t)m_entities.get_generation(i)});
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
    InitAudioDevice();
    restart:
    initscript(this);
    while(!WindowShouldClose()){
        frame_collision_set_up();
        Tick();
        if(m_reset){
            m_reset = false;
            reset();
            goto restart;
        }
        if(m_use_entity_as_origin){
            set_relative_locations();
        }
        Render();
        collect_dead();
    }
}
Vector2 Runtime::convert_world_to_screen(Vector2 v) const{
    return Vector2Add(Vector2Subtract(v, m_camera_location), Vector2{float(m_screen_width)/2, float(m_screen_height)/2});
}
ResourceRef Runtime::register_entity(Entity *e){
    auto handle = m_entities.emplace(e);
    e->on_init(handle);
    int depth = e->get_render_depth();
    return handle;
}
Entity * Runtime::get_entity(ResourceRef ref){
    return this->m_entities.get(ref);
}
void Runtime::set_entity_as_origin(ResourceRef ref, Vector2 Offset){
    m_origin_entity = ref;
    m_use_entity_as_origin = true;
    m_camera_location = Offset;
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
            if(g.box.height == 0 || g.box.width == 0){
                continue;
            }
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
    m_entity_destroy_queue.push_back(ref);
}
Vector2 convert_world_to_screen(Vector2 v){
    return rt->convert_world_to_screen(v);
}
ResourceRef register_entity(Entity *e){
    return rt->register_entity(e);
}
void set_entity_as_origin(ResourceRef ref, Vector2 Offset){
    rt->set_entity_as_origin(ref, Offset);
}
void unset_entity_as_origin(){
    rt->unset_entity_as_origin();
}
bool is_origin_set(){
    return rt->origin_set();
}
Entity * get_entity(ResourceRef ref){
    return rt->get_entity(ref);
}
void destroy_entity(ResourceRef ref){
    rt->destroy_entity(ref);
}
Collision line_trace(Vector2 start, Vector2 end,ResourceRef to_ignore){
    return rt->line_trace(start, end, to_ignore);
}
Collision box_trace(Vector2 start, Vector2 end, Rectangle rec,ResourceRef to_ignore){
    return rt->box_trace(start, end,rec, to_ignore);
}
int screen_height(){
    return rt->screen_height();
}
int screen_width(){
    return rt->screen_width();
}
ResourceRef load_texture_by_name(std::string texture){
    return rt->load_texture_by_name(texture);
}
Texture* get_texture(ResourceRef ref){
    return rt->get_texture(ref);
}
void unload_texture(std::string texture){
    rt->unload_texture(texture);
}
ResourceRef load_sound_by_name(std::string sound){
    return rt->load_sound_by_name(sound);
}
Sound* get_sound(ResourceRef ref){
    return rt->get_sound(ref);
}
void unload_sound(std::string sound){
    rt->unload_sound(sound);
}
void Runtime::reset(){
    m_reset = false;
    if(m_current_level){
        dlclose(m_current_level);
    }
    m_use_entity_as_origin = false;
    m_entities.clear();
    m_collisions.clear();
    m_texture_table.clear();
    m_textures.clear();
    m_sound_table.clear();
    m_sounds.clear();
    for(int i =0; i<num_layers; i++){
        m_to_draw[i].clear();
    }
    for(int i =0; i<m_col_tree.stride*m_col_tree.stride; i++){
        m_col_tree.m_area[i].clear();
    }
    collect_dead();
}
void Runtime::call_reset(){
    m_reset = true;
}
void Runtime::clear_all_but(std::vector<ResourceRef> r){
    for(int i =0; i<m_entities.cache_size(); i++){
        bool contained = false;
        for(int j =0; j<r.size(); j++){
            if(r[j].idx == i){
                if(m_entities.get_generation(i) == r[j].generation){
                    contained = true;
                    break;
                }
            }
        }
        if(!contained){
            this->destroy_entity(ResourceRef{.idx = (size_t)i, .generation = (size_t)(m_entities.get_generation(i))});
        }
    }
}
