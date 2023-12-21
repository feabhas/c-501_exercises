// sevensegment.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include "sevensegment.h"

#include "gpio.h"

// -----------------------------------------------------------------------------
// Seven-segment functions
//
enum { SS_A = 8, SS_B = 9, SS_C = 10, SS_D = 11 };


void sevensegment_init(void)
{
  gpio_init();

  gpio_set_as_output(SS_A);
  gpio_set_as_output(SS_B);
  gpio_set_as_output(SS_C);
  gpio_set_as_output(SS_D);

  sevensegment_blank();
}


void sevensegment_display(unsigned value)
{
  (value & 0x01) ? gpio_set_pin(SS_A) : gpio_clear_pin(SS_A);
  (value & 0x02) ? gpio_set_pin(SS_B) : gpio_clear_pin(SS_B);
  (value & 0x04) ? gpio_set_pin(SS_C) : gpio_clear_pin(SS_C);
  (value & 0x08) ? gpio_set_pin(SS_D) : gpio_clear_pin(SS_D);
}


void sevensegment_blank(void)
{
  sevensegment_display(15);
}
