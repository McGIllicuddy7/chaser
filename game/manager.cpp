#include "manager.h"
#include "../engine/runtime.h" 
#include "player.h"
#include "bullet.h"
#include "enemy.h"
#include "background.h"
Manager::Manager(void *runtime){
    m_runtime = runtime;
    started = false;
    end_screen = false;
    ship_count = 0;
    desired_ship_count = 2;
    m_collision.width = 0;
}
void Manager::on_tick(){
    Entity * p = get_entity(player);
    if(!started){
        if(!end_screen){
            if(IsKeyPressed(KEY_ENTER)){
                started = true;
                Player * bert = new Player(m_this_ref);
                player = register_entity(bert);
                for(int i =0; i<desired_ship_count; i++){
                    Enemy * e = new Enemy(m_this_ref, Vector2{(float)(500+rand()%280),(float)(rand()%screen_height()-screen_height()/2)});
                    ResourceRef r = register_entity(e);
                    ships.push_back(r);
                    ship_count++;
                }
            }
        }
        else{
            if(IsKeyPressed(KEY_ENTER)){
                ((Runtime * )(m_runtime))->call_reset();
            }
        }
    } else{
        if(ship_count<desired_ship_count){
            if(rand()%64 == 0){
                Enemy * e = new Enemy(m_this_ref, Vector2{(float)(500+rand()%280),(float)(rand()%screen_height()-screen_height()/2)});
                ResourceRef r = register_entity(e);
                ships.push_back(r);
                ship_count++;
            }
        }
        if(rand()%60 == 0){
            Player * p = (Player *)get_entity(player);
            Building * e = new Building({-1000,-p->y_disp()}, (rand()%1600)-800);
            register_entity(e);
        }
        if(rand()%2 == 0){
            Star * s = new Star((Vector2){-1000, (float)(rand()%2000-1000)}, rand()%2+rand()%2);
            register_entity(s);
        }
    }

}
void Manager::on_init( ResourceRef this_ref){
    m_this_ref = this_ref;
    started = false;
    ships.clear();
}
void Manager::player_destroyed(){
    if(m_runtime){
        ((Runtime * )(m_runtime))->clear_all_but({m_this_ref});
        started = false;
        end_screen = true;
    } else{
        exit(0);
    }
}
void Manager::ship_destroyed(ResourceRef ship_ref){
    int idx = -1;
    ship_count--;
    for(int i =0; i<ships.size(); i++){
        if(ships[i] == ship_ref){
            idx = i;
            break;
        }
    }
    if(idx == -1){
        return;
    }
    ships.erase(ships.begin()+idx, ships.begin()+idx);
}
void Manager::on_render(){
    if(!started){
        if(!end_screen){
            DrawText("Chaser", 400, 80, 128, RED);
            DrawText("Press Enter to Start", 520,200, 32, WHITE);
        } else{
            DrawText("Game Over",500,100, 64, WHITE); 
            DrawText("Press Enter to Restart", 520, 200, 32,WHITE);
        }
    }
}
Entity * Manager::get_player(){
    return get_entity(player);
}
