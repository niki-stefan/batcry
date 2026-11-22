#include "config.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED "\033[38;5;124m"
#define RESET "\033[0m"

#define THROW_ERROR(ERROR) snprintf(error, 50, RED "Error: %s" RESET, ERROR)

uint8_t parse_config(char *path, struct config *config, char *error) {
  FILE *file = fopen(path, "r");

  // @100$ (5) + 49 max command length + \0
  char buffer[56];

  uint8_t i = 0, offset = 0;
  struct level level;

  if (file == NULL) {
    THROW_ERROR(strerror(errno));

    return 0;
  }

  // First line: battery_level_filepath
  fgets(config->battery_level_filepath, 70, file);
  config
      ->battery_level_filepath[strcspn(config->battery_level_filepath, "\n")] =
      '\0';

  // Second line: is_charging_filepath
  fgets(config->is_charging_filepath, 70, file);
  config->is_charging_filepath[strcspn(config->is_charging_filepath, "\n")] =
      '\0';

  // Third line: number
  fscanf(file, "%d", &config->poll_time);

  // Fourth line: number of levels
  fscanf(file, "%hhu", &config->levels_count);
  config->levels = malloc(sizeof(struct level) * config->levels_count);

  // After reading the number, it will seek forward that many bytes, so C5\n, it
  // will become 5C\n, and fgets reads an empty line, so we skip it
  fseek(file, 1, SEEK_CUR);

  while (fgets(buffer, 56, file)) {
    offset = 0;

    level.when_charging = (buffer[0] == '#');

    // Skip the first character
    sscanf(buffer + ++offset, "%d", &level.level);

    if (level.level < 10)
      offset += 2; // We also account for the $
    else if (level.level != 100)
      offset += 3;
    else
      offset += 4;

    // Copy maximum 50 characters from buffer (after the $)
    strncpy(level.command, buffer + offset, 50);

    // strncpy doesn't automatically add \0 🙄
    level.command[49] = '\0';

    // Remove the newline
    level.command[strcspn(level.command, "\n")] = '\0';

    config->levels[i++] = level;
  }

  fclose(file);
  return 1;
}
