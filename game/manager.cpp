#include "manager.h"
#include "../engine/runtime.h" 
Manager::Manager(void *runtime){
    m_runtime = runtime;
}
void Manager::on_tick(){

}
void Manager::on_init( ResourceRef this_ref){

}
void Manager::player_destroyed(){
    ((Runtime * )(m_runtime))->reset();
}
void Manager::ship_destroyed(ResourceRef ship_ref){

}