#include "battery.h"

#include <stdint.h>
#include <stdio.h>

#define BATTERY_PATH "/sys/class/power_supply/BAT0/capacity"

int main(void) {
  char error[50];

  int8_t level = get_battery_level(BATTERY_PATH, error);

  if (level < 0) {
    printf("%s\n", error);
  } else {
    printf("%hhd\n", level);
  }

  return 0;
}
