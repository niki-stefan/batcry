#pragma once

#include <stdint.h>

struct config {
  char battery_level_filepath[70];
  char is_charging_filepath[70];

  int32_t poll_time;

  uint8_t levels_count;
  struct level *levels;
};

struct level {
  int8_t level;
  char command[50];

  // 000000AB, A = is_charging, B = has_executed
  uint8_t status;
};

uint8_t parse_config(char *path, struct config *config, char *error);
