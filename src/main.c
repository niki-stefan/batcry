#include "battery.h"
#include "config.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CONFIG_PATH "/home/niki/.config/batcry/config"

int main(void) {
  char error[50];

  struct config config;
  int i;

  if (!parse_config(CONFIG_PATH, &config, error)) {
    printf("%s\n", error);

    return 1;
  }

  for (i = 0; i < config.levels_count; i++) {
    printf("executing %s at %d%% when is_charging=%d\n",
           config.levels[i].command, config.levels[i].level,
           config.levels[i].when_charging);
  }

  int8_t level = get_battery_level(config.battery_level_filepath, error);

  if (level < 0) {
    printf("%s\n", error);

    return 1;
  }

  int8_t is_charging_status = is_charging(config.is_charging_filepath, error);

  if (is_charging_status < 0) {
    printf("%s\n", error);

    return 1;
  }

  printf("%hhd%%, is_charging=%hhd\n", level, is_charging_status);

  free(config.levels);
  return 0;
}
