#pragma once
#include <raylib.h>
#include "../engine/ops.h"
#include "../engine/entity.h"
#include "../engine/resource.h"
Vector2 convert_world_to_screen(Vector2 v);
ResourceRef register_entity(Entity *e);
void set_entity_as_origin(ResourceRef ref, Vector2 Offset);
void unset_entity_as_origin();
bool is_origin_set();
Entity * get_entity(ResourceRef ref);
void destroy_entity(ResourceRef ref);
Collision line_trace(Vector2 start, Vector2 end,ResourceRef to_ignore = ResourceRef());
Collision box_trace(Vector2 start, Vector2 end, Rectangle rec,ResourceRef to_ignore = ResourceRef());
int screen_height();
int screen_width();
ResourceRef load_texture_by_name(std::string texture);
Texture* get_texture(ResourceRef ref);
void unload_texture(std::string texture);
#define SCREEN(V) convert_world_to_screen(v)