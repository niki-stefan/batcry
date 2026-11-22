#include "battery.h"

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// If an errors occurs, the function will set errno to a number, strerror
// returns the symbolic name of that error number
// \033[38;5;196m sets the foreground to red, and \033[0m resets it
// https://stackoverflow.com/questions/4842424/list-of-ansi-color-escape-sequences
#define THROW_ERROR(ERROR)                                                     \
  snprintf(error, 50, "Error: \033[38;5;124m%s\033[0m", ERROR)

int8_t get_battery_level(char *path, char *error) {
  FILE *file = fopen(path, "r");
  int8_t level;
  int fscanf_return;

  if (file == NULL) {
    THROW_ERROR(strerror(errno));

    return -1;
  }

  if ((fscanf_return = fscanf(file, "%hhd", &level)) == EOF) {
    THROW_ERROR("File is empty");

    return -1;
  }

  if (fscanf_return == 0) {
    THROW_ERROR("File must contain a number");

    return -1;
  }

  fclose(file);
  return level;
}
