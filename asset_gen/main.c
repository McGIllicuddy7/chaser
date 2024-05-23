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
void generate_ship_explosions(){
    int r = 100;
    int amnt = 15;
    for(int i =1; i<amnt; i++){
        String name = string_format("ship_explosion%d.png", i);
        begin_image();
        double theta = (((double)(i*2))*PI)/35;
        DrawNoisedOval((Vector2){50, 50}, 10, 50*(float)i/amnt+50, 10, 50*(float)i/amnt+50,(Color){50,50,50, 255-i*4}, r);
        DrawNoisedOval((Vector2){50, 50},0, 40*sin(theta)+40, 0, 40*sin(theta)+40, (Color){255,32,0, 255-i*4}, r);
        end_image(name, 1);
        destroy(name);
    }
}
void generate_fog(){ 
    const char * name = "fog.png";
    begin_image();
    for(int x = 0; x<100; x++){
        for(int y = 0; y<100; y++){
            Vector2 loc = {x,y};
            float dx = loc.x-50;
            float dy = loc.y-50;
            Vector2 v = {dx, dy};
            v = Vector2Normalize(v);
            double theta = Vector2Angle(v, (Vector2){1,0});
            double noise = noise1d(100, theta*1000, 4);
            dx *= dx;
            dy *= dy;
            float rx =Lerp(20,70, noise);
            float ry =Lerp(20, 70, noise);
            dx /= rx*rx;
            dy /= ry*ry;
            if((dx+dy)<1){
                double n = noise2d(100, (double)x*10,(double) y*10, 5);
                DrawPixel(x,y, (Color){n*255, n*255, n*255,n*125+125});
            } 
        }
    }
    end_image(name, 1);
}
int main(void){
    Color col = DARKBLUE;
    SetTraceLogLevel(LOG_ERROR); 
    InitWindow(width, height, "generation");
    generate_fog();
    CloseWindow();
}