#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
double interpolate(double a0, double a1, double w) {
    return (a1 - a0) * ((w * (w * 6.0 - 15.0) + 10.0) * w * w * w) + a0;
}

typedef struct {
    double x, y;
} vector2;
vector2 randomGradient(size_t seed, int ix, int iy) {
    const unsigned w = 8 * sizeof(unsigned);
    const unsigned s = w / 2; // rotation width
    unsigned a = ix, b = iy;
    a *= 3284157443^seed; b ^= a << s | a >> (w-s);
    b *= 1911520717; a ^= b << s | b >> (w-s);
    a *= 2048419325;
    double random = a * (3.14159265 / ~(~0u >> 1));
    vector2 v;
    v.x = cos(random); v.y = sin(random);
    return v;
}

double dotGridGradient(size_t seed,int ix, int iy, double x, double y) {
    vector2 gradient = randomGradient(seed, ix, iy);
    double dx = x - (double)ix;
    double dy = y - (double)iy;
    double out = (dx*gradient.x + dy*gradient.y);
    return out;
}

double perlin(size_t seed, double x, double y) {
    int x0 = (int)floor(x);
    int x1 = x0 + 1;
    int y0 = (int)floor(y);
    int y1 = y0 + 1;
    double sx = x - (double)x0;
    double sy = y - (double)y0;
    double n0, n1, ix0, ix1, value;
    n0 = dotGridGradient(seed,x0, y0, x, y);
    n1 = dotGridGradient(seed,x1, y0, x, y);
    ix0 = interpolate(n0, n1, sx);
    n0 = dotGridGradient(seed, x0, y1, x, y);
    n1 = dotGridGradient(seed, x1, y1, x, y);
    ix1 = interpolate( n0, n1, sx);
    value = interpolate(ix0, ix1, sy);
    return value*0.5+0.5; 
}
double noise2d(uint64_t seed, double x, double y, int layers){
    double out = 0;
    double total = 0;
    for(int i =0; i<layers; i++){
        double mlt = pow(2, i-10);
        out += perlin(seed, x*mlt ,y*mlt)*(1-mlt);
        total +=(1- mlt);
    }
    return out/total;
}
double noise1d(uint64_t seed, double x, int layers){
    return noise2d(seed, x/2, 0, layers);
}
