#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#define CTILS_IMPLEMENTATION
#include "utils.h"
#include "noise.h"
#include <math.h>
const int height = 50;
const int width = 50;
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
void DrawNoisedOval(Vector2 location, float rvmin, float rvmax, float rhmin, float rhmax, Color col){
    int r = rand();
       for(int y =0; y<height; y++){
        for(int x = 0; x<width; x++){
            Vector2 loc = {x,y};
            float dx = loc.x-location.x;
            float dy = loc.y-location.y;
            Vector2 v = {dx, dy};
            v = Vector2Normalize(v);
            double theta = Vector2Angle(v, (Vector2){1,0});
            double noise = noise1d(r, theta/(2*PI)*40000, 4);
            noise *= noise;
            dx *= dx;
            dy *= dy;
            float rx =Lerp(rvmin, rvmax, noise);
            float ry =Lerp(rhmin, rhmax, noise);
            dx /= rx*rx;
            dy /= ry*ry;
            if((dx+dy)<1){
                DrawPixel(x,y, col);
            }
        }   
    } 
};
void generate_ship(Color col){
    const Color ship_color = col;
    DrawRectangle(10, 4, 48-34, 32-8, ship_color);
    DrawRectangle(0, 0, 20, 8,ship_color);
    DrawRectangle(0, 32/2-4, 20, 8,ship_color);    
    DrawRectangle(0, 32-8, 20, 8, ship_color);
    DrawTriangle((Vector2){48, 16}, (Vector2){20, 4},(Vector2){20, 32-4}, ship_color);
    DrawOval((Vector2){2,3.5},4,3, BLACK);
    DrawOval((Vector2){2,16-0.5},4,3, BLACK);
    DrawOval((Vector2){2,32.0-8.0+3.5},4,3, BLACK);
    DrawTriangle((Vector2){44, 16}, (Vector2){32, 11},(Vector2){32, 32-11}, BLACK);
}
void generate_engines(Color col){
    DrawOval((Vector2){2,3.5},2,2, col);
    DrawOval((Vector2){2,16-0.5},2,2, col);
    DrawOval((Vector2){2,32.0-8.0+3.5},2,2, col); 
}
void draw(Color col){
    DrawNoisedOval((Vector2){25,25}, 1,25, 1,25, RED);
    //generate_engines(BLUE);
}
int main(void){
    const char * name = "test0.png";
    bool flipped = 1;
    Color col = DARKBLUE;
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(width, height, "generation");
    {
        RenderTexture r = LoadRenderTexture(width, height);
        BeginTextureMode(r);
        ClearBackground((Color){0,0,0,0});
        draw(col);
        EndTextureMode();
        Image I = LoadImageFromTexture(r.texture);
        if(flipped){
           ImageFlipHorizontal(&I);
        }
        ExportImage(I, name);
        UnloadImage(I);
        UnloadRenderTexture(r);
    }
    CloseWindow();
}