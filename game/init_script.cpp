#include "raylib/raylib.h"
#include "../engine/runtime.h"
#include "manager.h"
void initscript(Runtime *runtime){
    Manager * manager = new Manager(runtime);
    auto r = runtime->register_entity(manager);
}