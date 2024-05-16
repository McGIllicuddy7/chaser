#include "../raylib/raylib.h"
#define CTILS_IMPLEMENTATION
#include "utils.h"
const int height = 32;
const int width = 48;
void DrawOval(Vector2 location, float rv, float rh, Color col){
    for(int y =0; y<height; y++){
        for(int x = 0; x<width; x++){
            Vector2 loc = {x,y};
            float dx = loc.x-location.x;
            float dy = loc.y-location.y;
            dx *= dx;
            dy *= dy;
            dx /= rh*rh;
            dy /= rv*rv;
            if((dx+dy)<1){
                DrawPixel(x,y, col);
            }
        }   
    }
}
void generate_ship(){
    DrawRectangle(10, 4, 48-34, 32-8, BLUE);
    DrawRectangle(0, 0, 20, 8, BLUE);
    DrawRectangle(0, 32/2-4, 20, 8, BLUE);    
    DrawRectangle(0, 32-8, 20, 8, BLUE);
    DrawTriangle((Vector2){48, 16}, (Vector2){20, 4},(Vector2){20, 32-4}, BLUE);
    DrawOval((Vector2){2,3.5},4,3, BLACK);
    DrawOval((Vector2){2,16-0.5},4,3, BLACK);
    DrawOval((Vector2){2,32.0-8.0+3.5},4,3, BLACK);
    DrawTriangle((Vector2){44, 16}, (Vector2){32, 11},(Vector2){32, 32-11}, BLACK);
}
void draw(){
    generate_ship();

}
int main(void){
    const char * name = "friendly_ship.png";
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(width, height, "generation");
    {
        RenderTexture r = LoadRenderTexture(width, height);
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