#pragma once
#include <stdint.h>
#include <stdlib.h>
enum class ent_id:size_t{
    object = 0,
    player = 1,
    enemy = 2,
    particle = 3,
    background = 4,
};