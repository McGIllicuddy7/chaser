#include "../raylib/raylib.h"
#include "../raylib/raymath.h"
#define CTILS_IMPLEMENTATION
#include "utils.h"
#include "noise.h"
#include <math.h>
const int height = 100;
const int width = 100;
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
void DrawNoisedOval(Vector2 location, float rvmin, float rvmax, float rhmin, float rhmax, Color col, int r){
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
RenderTexture render_texture;
void begin_image(){
    render_texture = LoadRenderTexture(width, height);
    BeginTextureMode(render_texture);
    ClearBackground((Color){0,0,0,0});
}
void end_image(const char * name, bool flipped){
    EndTextureMode();
    Image I = LoadImageFromTexture(render_texture.texture);
    if(flipped){
        ImageFlipHorizontal(&I);
    }
    char * name_actual = string_format("output/%s",name);
    ExportImage(I, name_actual);
    destroy(name_actual);
    UnloadImage(I);
    UnloadRenderTexture(render_texture);
}
int main(void){
    Color col = DARKBLUE;
    SetTraceLogLevel(LOG_ERROR); 
    InitWindow(width, height, "generation");
    int r = 100;
    for(int i =1; i<14; i++){
        String name = string_format("ship_explosion%d.png", i);
        begin_image();
        double theta = (((double)(i*2))*PI)/30;
        DrawNoisedOval((Vector2){50, 50}, 10, 200*(float)i/14, 10, 200*(float)i/14,(Color){50,50,50, 255-i*8}, r);
        DrawNoisedOval((Vector2){50, 50},0, 150*sin(theta), 0, 150*sin(theta), (Color){255,32,0, 255-i*8}, r);
        end_image(name, 1);
        destroy(name);
    }

    CloseWindow();
}