#include <raylib.h>
#include "../engine/runtime.h"
#include "player.h"
#include "box.h"


inline void init_script(Runtime *runtime){
    Player *bert = new Player;    
    runtime->register_entity(bert);
    for(int i =0; i<5000; i++){
        Box * b = new Box;
        runtime->register_entity(b);
    }

}