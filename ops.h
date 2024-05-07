#pragma once
#include <raylib.h>
#include <raymath.h>
inline Vector2 operator +(Vector2 a, Vector2 b){
    return Vector2Add(a,b);
}
inline Vector2 operator-(Vector2 a, Vector2 b){
    return Vector2Subtract(a,b);
}
inline Vector2 operator *(Vector2 a, float b){
    return Vector2Scale(a,b);
}
inline Vector2 operator*(float b, Vector2 a){
    return Vector2Scale(a,b);
}
inline Vector2 operator/ (Vector2 a, float b){
    return Vector2Scale(a,1/b);
}