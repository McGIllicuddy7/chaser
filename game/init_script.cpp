#include <raylib.h>
#include "../engine/runtime.h"
#include "player.h"
#include "box.h"
extern void initscript(Runtime * runtime);
void initscript(Runtime *runtime){
    Player *bert = new Player;    
    runtime->register_entity(bert);
    for(int i =0; i<5000; i++){
        Box * b = new Box;
        runtime->register_entity(b);
    }
}