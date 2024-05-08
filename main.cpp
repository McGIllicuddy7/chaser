#include <raylib.h>
#include "runtime.h"
#include "player.h"
#include "box.h"
void init_script(Runtime *runtime){
    Player *bert = new Player;    
    runtime->register_entity(bert);
    for(int i =0; i<1000; i++){
        Box * b = new Box;
        runtime->register_entity(b);
    }

}
int main(void){
    Runtime * runtime = Runtime::New();
    runtime->Run();
}