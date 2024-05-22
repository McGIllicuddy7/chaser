#pragma once
#include <stdint.h>
double noise2d(uint64_t seed, double x, double y, int layers);
double noise1d(uint64_t seed, double x, int layers);