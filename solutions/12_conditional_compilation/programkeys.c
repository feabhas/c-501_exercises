// programkeys.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include "programkeys.h"
#include "gpio.h"


// -----------------------------------------------------------------------------
// Program selector functions
//

// 'Private' functions
//
static void set_latch(void);
static void unset_latch(void);



enum { PS_1 = 1, PS_2 = 2, PS_3 = 3, CANCEL = 4, ACCEPT = 5, LATCH = 14 };


void programkeys_init(void)
{
  // Enable the GPIO port
  //
  gpio_init();

  // Set  buttons as inputs
  gpio_set_as_input(PS_1);
  gpio_set_as_input(PS_2);
  gpio_set_as_input(PS_3);
  gpio_set_as_input(CANCEL);
  gpio_set_as_input(ACCEPT);

  // set latch as output
  gpio_set_as_output(LATCH); // clear msb
}


unsigned programkeys_get_selection(void)
{
  unsigned selection = 0;

  set_latch();

  do
  {
    if (gpio_is_set(PS_1)) selection |= 0x01;
    if (gpio_is_set(PS_2)) selection |= 0x02;
    if (gpio_is_set(PS_3)) selection |= 0x04;

    if (gpio_is_set(CANCEL))
    {
      unset_latch();
      set_latch();
      selection = 0;
    }

  } while (!gpio_is_set(ACCEPT));

  unset_latch();
  while (gpio_is_set(ACCEPT))
  {
    ; // wait...
  }

  return selection;
}


void set_latch(void)
{
  gpio_set_pin(LATCH);
}


void unset_latch(void)
{
  gpio_clear_pin(LATCH);
}
