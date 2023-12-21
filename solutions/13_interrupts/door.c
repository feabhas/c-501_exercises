// door.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include "door.h"

#include "gpio.h"

// -----------------------------------------------------------------------------
// Door functions
//

enum { DOOR = 0 };


void door_init(void)
{
  gpio_init();

  gpio_set_as_input(DOOR);
}


bool door_is_open(void)
{
  return gpio_is_set(DOOR);
}
