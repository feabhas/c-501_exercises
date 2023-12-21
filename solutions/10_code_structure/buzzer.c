// buzzer.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include "buzzer.h"

#include "gpio.h"
#include "timer.h"

// -----------------------------------------------------------------------------
// Buzzer functions
//
enum { BUZZER = 15 };


void buzzer_init(void)
{
  gpio_init();

  gpio_set_as_output(BUZZER);

  gpio_clear_pin(BUZZER);
}


void buzzer_sound(void)
{
  gpio_set_pin(BUZZER);
  sleep(100);
  gpio_clear_pin(BUZZER);
}
