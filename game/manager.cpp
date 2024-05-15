#include "manager.h"
#include "../engine/runtime.h" 
#include "player.h"
Manager::Manager(void *runtime){
    m_runtime = runtime;
}
void Manager::on_tick(){

}
void Manager::on_init( ResourceRef this_ref){
    m_this_ref = this_ref;
    Player * bert = new Player(this_ref);
    player = ((Runtime * )(m_runtime))->register_entity(bert);
}
void Manager::player_destroyed(){
    ((Runtime * )(m_runtime))->reset();
}
void Manager::ship_destroyed(ResourceRef ship_ref){
    int idx = -1;
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