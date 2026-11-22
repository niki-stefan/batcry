#pragma once

#include <stdint.h>

// int8_t goes from -128 to 127, and -1 <= value <= 100, -1 gets returned upon error
int8_t get_battery_level(char *path, char *error);
