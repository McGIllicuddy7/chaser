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
    m_new_high_score = false;
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
    m_score = 0;
    ((Runtime * )(m_runtime))->set_background_color(Color{ 100/4, 122/8, 127/4, 255 });
    FILE * f = fopen("../resources/high_score", "rb");
    if(f){
        fread(&m_high_score,sizeof(size_t),1 ,f);
        fclose(f);
    } else{
        f = fopen("../resources/high_score", "wb");
        if(f){
            size_t s = 0;
            fwrite(&s, sizeof(s), 1, f);
            fclose(f);
        }
        m_high_score = 0;
    }
}
void Manager::player_destroyed(){
    if(m_runtime){
        ((Runtime * )(m_runtime))->clear_all_but({m_this_ref});
        started = false;
        end_screen = true;
        if(m_score>m_high_score){
            FILE*f = fopen("../resources/high_score", "wb");
            if(f){
                fwrite(&m_score, sizeof(m_score), 1, f);
                fclose(f);
            } 
            m_high_score = m_score;
            m_new_high_score = true;
        }
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
    m_score ++;
}
void Manager::on_render(){
    if(!started){
        if(!end_screen){
            DrawText("Chaser", 400, 80, 128, RED);
            DrawText("Press Enter to Start", 520,200, 32, WHITE);
            char buff[100] = {};
            snprintf(buff, 99, "high score %zu00", m_high_score);
            DrawText(buff, 1000, 125, 24, WHITE);
        } else{
            DrawText("Game Over",400,100, 64, WHITE); 
            DrawText("Press Enter to Restart", 520, 200, 32,WHITE);
            char buff[100] = {};
            snprintf(buff, 99, "your score %zu00", m_score);
            DrawText(buff,450, 300, 25, WHITE);
            snprintf(buff, 99, "high score %zu00", m_high_score);
            DrawText(buff, 450, 350, 25, WHITE);
            if(m_new_high_score){
                DrawText("NEW HIGH SCORE!", 500, 450,50, WHITE);
            }
        }
    }
    else{
        char buff[100] = {};
        snprintf(buff, 99, "score %zu00", m_score);
        DrawText(buff, 1050, 100,25, WHITE);
    }
}
Entity * Manager::get_player(){
    return get_entity(player);

}
