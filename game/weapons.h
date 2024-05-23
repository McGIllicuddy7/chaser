#pragma once
#include "engine.h"
#include "raylib/raylib.h"
Collision fire_laser(Vector2 start, Vector2 direction, ResourceRef ref);
void fire_railgun(Vector2 start, Vector2 direction, ResourceRef ref);
void spawn_chaff(Vector2 location,Vector2 velocity, ResourceRef ref);
void fire_missile(Vector2 location, Vector2 velocity, ResourceRef ref, size_t target_id);
void fire_blaster(Vector2 start, Vector2 direction, ResourceRef ref);