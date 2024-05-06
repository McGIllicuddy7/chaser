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
    char t[100] = {};
    snprintf(t, 100, "x_dist:%f y_dist:%f", m_col_tree.xdiv, m_col_tree.ydiv); 
    DrawFPS(600,80);
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
static Collision single_line_trace(Vector2 start, Vector2 end, ResourceRef to_ignore, std::vector<EntityBB> &collisions){
    float min_dist = Vector2LengthSqr(end-start);
    float dist = min_dist;
    Collision out = {false};
    for(int i = 0; i<collisions.size(); i++){
        ResourceRef rf = collisions[i].Parent;
        if(rf == to_ignore){
            continue;
        }
        Rectangle bx = collisions[i].box;
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
Collision Runtime::line_trace(Vector2 start, Vector2 end,ResourceRef to_ignore){
    return m_col_tree.line_trace(start, end, m_collisions, to_ignore);
}
void ColTree::initialize(std::vector<EntityBB> &collisions,Vector2 min, Vector2 max){
    for(int i = 0; i<stride*stride; i++){
        m_area[i].clear();
        m_area[i].reserve(32);
    }
    m_min = min;
    m_max = max;
    xdiv =(m_max.x-m_min.x)/stride;
    ydiv =(m_max.y-m_min.y)/stride;
    for(int i = 0; i<collisions.size(); i++){
        EntityBB a = collisions[i];
        {
            int x = (a.box.x-m_min.x)/xdiv;
            int y =(a.box.y-m_min.y)/ydiv;
            if (x<0) x = 0;
            if (y<0) y = 0;
            if (x>stride-1) x= stride-1;
            if (y>stride -1) y= stride-1;
            m_area[x+y*stride].push_back(a);
        }
        {
            int x = (a.box.x+a.box.width-m_min.x)/xdiv;
            int y =(a.box.y-m_min.y)/ydiv;
            if (x<0) x = 0;
            if (y<0) y = 0;
            if (x>stride-1) x= stride-1;
            if (y>stride -1) y= stride-1;
            m_area[x+y*stride].push_back(a);
        }
        {
            int x = (a.box.x-m_min.x)/xdiv;
            int y =(a.box.y+a.box.height-m_min.y)/ydiv;
            if (x<0) x = 0;
            if (y<0) y = 0;
            if (x>stride-1) x= stride-1;
            if (y>stride -1) y= stride-1;
            m_area[x+y*stride].push_back(a);
        }{
            int x = (a.box.x+a.box.width-m_min.x)/xdiv;
            int y =(a.box.y+a.box.height-m_min.y)/ydiv;
            if (x<0) x = 0;
            if (y<0) y = 0;
            if (x>stride-1) x= stride-1;
            if (y>stride -1) y= stride-1;
            m_area[x+y*stride].push_back(a);
        }
    }
}
Collision ColTree::line_trace(Vector2 start, Vector2 end, std::vector<EntityBB> &boxes,ResourceRef to_ignore){
    float min_dist = Vector2DistanceSqr(start, end);
    float d = min_dist;
    Collision out = {false};
    for(int y = 0; y<stride; y++){
        for(int x = 0; x<stride; x++){
            Rectangle bx = {x*xdiv+m_min.x, y*ydiv+m_min.y,xdiv, ydiv};
            Vector2 c1 = {bx.x, bx.y};
            Vector2 c2 = {bx.x+bx.width, bx.y};
            Vector2 c3 = {bx.x,bx.y+bx.height};
            Vector2 c4 = {bx.x+bx.width, bx.y+bx.height};
            bool inside = false;
            if(CheckCollisionPointRec(start, bx)){
                inside = true;
            }
            if(CheckCollisionPointRec(end, bx)){
                inside = true;
            }
            if ((Vector2DistanceSqr(start, c1)>d && Vector2DistanceSqr(start, c2)>d
            && Vector2DistanceSqr(start, c3)>d && Vector2DistanceSqr(start, c4)>d)&& !inside){
                continue;
            }
            Vector2 col;
            if(CheckCollisionLines(start, end, c1, c2, &col)|| CheckCollisionLines(start, end, c1, c3, &col)||
            CheckCollisionLines(start, end, c2, c4, &col)||CheckCollisionLines(start, end, c3, c4, &col) || inside){
                Collision tmp = single_line_trace(start, end,to_ignore, boxes);
                float d0 = Vector2Distance(tmp.location, start);
                if( d0<min_dist){
                    min_dist = d0;
                    out = tmp;
                }
            }
        }
    }
    return out;
}
Collision ColTree::box_trace(Vector2 start, Vector2 end, Rectangle rec, std::vector<EntityBB> &boxes,ResourceRef to_ignore){
    start = {rec.x, rec.y};
    float min_dist = Vector2DistanceSqr(start, end);
    Collision out = {false};
    Collision c1 = line_trace(start, end, boxes, to_ignore);
    if(c1.hit){
        float d = Vector2DistanceSqr(start, c1.location);
        if( d<min_dist){
            min_dist = d;
            out = c1;
        }
    }
    Collision c2 = line_trace(start+Vector2{rec.width,0}, end+Vector2{rec.width,0}, boxes, to_ignore);
    if(c2.hit){
        float d = Vector2DistanceSqr(start+Vector2{rec.width,0}, c2.location);
        if( d<min_dist){
            min_dist = d;
            out = c2;
        }
    }
    Collision c3 = line_trace(start+Vector2{0, rec.height}, end+Vector2{0, rec.height}, boxes, to_ignore);
    if(c3.hit){
        float d = Vector2DistanceSqr(start+Vector2{0,rec.height}, c3.location);
        if( d<min_dist){
            min_dist = d;
            out = c3;
        }
    }
    Collision c4 = line_trace(start+Vector2{rec.width, rec.height}, end+Vector2{rec.width, rec.height}, boxes, to_ignore);
    if(c4.hit){
        float d = Vector2DistanceSqr(start+Vector2{rec.width,rec.height}, c4.location);
        if( d<min_dist){
            min_dist = d;
            out = c4;
        }
    }
    Collision c5 = line_trace(start+Vector2{rec.width/2, rec.height/2}, end+Vector2{rec.width/2, rec.height/2}, boxes, to_ignore);
    if(c5.hit){
        float d = Vector2DistanceSqr(start+Vector2{rec.width/2,rec.height/2}, c5.location);
        if( d<min_dist){
            min_dist = d;
            out = c5;
        }
    }
    return out;
}
void Runtime::destroy_entity(ResourceRef ref){
    m_entities.remove(ref);
}