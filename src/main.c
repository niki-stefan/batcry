#include "battery.h"
#include "config.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CONFIG_PATH "/home/niki/.config/batcry/config"

void poll(int8_t *level, int8_t *is_charging_status, char *error,
          struct config *config);

int main(void) {
  char error[50];

  struct config config;
  uint8_t i;

  int8_t battery_level, is_charging_status;
  struct level *level;

  if (!parse_config(CONFIG_PATH, &config, error)) {
    printf("%s\n", error);

    return 1;
  }

  while (1) {
    poll(&battery_level, &is_charging_status, error, &config);

    for (i = 0; i < config.levels_count; i++) {
      level = &config.levels[i];

      if (is_charging_status) {
        // Only when charging, has not executed and battery is above
        if (!(level->status & 1) && (level->status >> 1) & 1 &&
            battery_level >= level->level) {
          system(level->command);

          level->status |= 1;
        }

        // Has executed, only when NOT charging
        if ((level->status & 1) && !((level->status >> 1) & 1)) {
          level->status ^= 1; // Invert the last bit
        }
      } else {
        // Has not executed, only when NOT charging, and battery level is below
        if (!(level->status & 1) && !((level->status >> 1) & 1) &&
            battery_level <= level->level) {
          system(level->command);

          // Set the last bit to one
          level->status |= 1;
        }

        // Only when charging, has executed, battery level is strictly below
        if ((level->status & 1) && (level->status >> 1) & 1 &&
            battery_level < level->level) {
          level->status ^= 1; // Invert the last bit
        }
      }
    }

    sleep(config.poll_time);
  }

  free(config.levels);
  return 0;
}

void poll(int8_t *level, int8_t *is_charging_status, char *error,
          struct config *config) {
  *level = get_battery_level(config->battery_level_filepath, error);

  if (*level < 0) {
    printf("%s\n", error);

    return;
  }

  *is_charging_status = is_charging(config->is_charging_filepath, error);

  if (*is_charging_status < 0) {
    printf("%s\n", error);
  }
}
