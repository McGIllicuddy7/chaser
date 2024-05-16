#include "../raylib/raylib.h"
#define CTILS_IMPLEMENTATION
#include "utils.h"
const int height = 32;
const int width = 32;
void draw(){
    DrawCircle(16, 16, 8, RED);
}
int main(void){
    const char * name = "test.png";
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(height, width, "generation");
    {
        RenderTexture r = LoadRenderTexture(height, width);
        BeginTextureMode(r);
        ClearBackground((Color){0,0,0,0});
        draw();
        EndTextureMode();
        Image I = LoadImageFromTexture(r.texture);
        ExportImage(I, name);
        UnloadImage(I);
        UnloadRenderTexture(r);
    }
}