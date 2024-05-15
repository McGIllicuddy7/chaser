#include "entity.h"
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
        m_area[i].reserve(16);
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
static bool v_contains(std::vector<Vector2> &v, Vector2 loc){
    for(int i = v.size()-1; i>-1; i--){
        if(v[i].x == loc.x && v[i].y == loc.y){
            return true;
        }
    }
    return false;
}
static bool point_in_rect(Vector2 v, Rectangle a){
    return (v.x>=a.x && v.x<a.width+a.x) && ( v.y>=a.y && v.y<a.height+a.y);
}
static bool check_rec_collision(Rectangle a, Rectangle b){
    Vector2 ac1 = {a.x, a.y};
    Vector2 ac2 = {a.x+a.width, a.y};
    Vector2 ac3 = {a.x, a.y+a.height};
    Vector2 ac4 = {a.x+a.width, a.y+a.height};
    Vector2 a_s[] = {ac1, ac2, ac3, ac4};

    Vector2 bc1 = {b.x, b.y};
    Vector2 bc2 = {b.x+b.width, b.y};
    Vector2 bc3 = {b.x, b.y+b.height};
    Vector2 bc4 = {b.x+b.width, b.y+b.height};
    Vector2 b_s[] = {bc1, bc2, bc3, bc4};
    for(int i =0 ; i<4; i++){
        if(point_in_rect(a_s[i], b)){
            return true;
        }
    }
    for(int i =0 ; i<4; i++){
        if(point_in_rect(b_s[i], a)){
            return true;
        }
    }
    return false;
}
Collision ColTree::box_trace(Vector2 start, Vector2 end, Rectangle rec, std::vector<EntityBB> &boxes,ResourceRef to_ignore){
    Rectangle current = rec;
    float dist = 0;
    float max_dist = Vector2Distance(start, end);
    if (max_dist<3){
        max_dist = 3;
    }
    Vector2 Direction = Vector2Normalize(end-start);
    Vector2 previous = {current.x, current.y};
    float delta = 0.5;
    std::vector<Vector2> hits = {};
    float dx = m_max.x-m_min.x;
    float dy = m_max.y-m_min.y;
    float mx = m_min.x;
    float my = m_min.y;
    while(dist<=max_dist){
        float lx = ((current.x-mx)/dx)*stride;
        float ly = ((current.y-my)/dy)*stride;
        float gx = (((current.x-mx+current.width)/dx)*stride);
        float gy = (((current.y-my+current.height)/dy)*stride);
        Vector2 loc1 = {lx,ly};
        Vector2 loc2 = {gx,ly};        
        Vector2 loc3 = {lx,gy};
        Vector2 loc4 = {gx,gy};
        Vector2 locs[] = {loc1,loc2, loc3, loc4};
        for(int i =0; i<4; i++){
            if( (int)locs[i].x<0 || (int)locs[i].x>=stride ||(int) locs[i].y<0 || (int)locs[i].y>=stride){
                continue;
            }
            if(v_contains(hits, locs[i])){
                continue;
            }
            std::vector<EntityBB> & ref = m_area[(int)(locs[i].x)+(int)(locs[i].y)*stride];
            for(int i =0; i<ref.size(); i++){
                if(ref[i].Parent == to_ignore){
                    continue;
                }
                if(CheckCollisionRecs(ref[i].box, current)){
                    return {true, previous,{1,0},ref[i].Parent};
                }
            }
        }
        previous = {current.x, current.y};
        current.x += Direction.x*delta;
        current.y += Direction.y*delta;
        dist += delta;
    }
    return {false};
}