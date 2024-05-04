#include <raylib.h>
#include "runtime.h"
#include "player.h"
#include "box.h"
int main(void){
    printf("%zu\n", sizeof( Player));
    Runtime * runtime = Runtime::New();
    Player *bert = new Player;
    for(int i =0; i<10000; i++){
        Box * b = new Box;
        runtime->register_entity(b);
    }
    runtime->register_entity(bert);
    runtime->Run();
}