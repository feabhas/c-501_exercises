// UI.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include "UI.h"


// -----------------------------------------------------------------------------
// USART implementation
//
#ifdef UI_USE_USART

#include "usart.h"

static void display_menu(void)
{
  usart_send_string("***************************\n\r");
  usart_send_string("Washing Machine Program   *\n\r");
  usart_send_string("***************************\n\r");
  usart_send_string("1 - White Wash             \n\r");
  usart_send_string("2 - Colour Wash            \n\r");
  usart_send_string("3 - Mixed Wash             \n\r");
  usart_send_string("4 - Economy Wash           \n\r");
  usart_send_string("5 - Program 1              \n\r");
  usart_send_string("6 - Program 2              \n\r");
  usart_send_string("7 - Exit to maintenance    \n\r");
  usart_send_string("                           \n\r");
  usart_send_string("Select a program: ");
}



Wash_programme_t get_programme_choice(void)
{
  static _Bool once = 0;
  if (once == 0) {
    usart_init();
    once = 1;
  }
  uint8_t selection;
  do {
    display_menu();
    selection = usart_get();
    usart_send(selection);
    usart_send_string("\n\r");
  } while((selection < '0')||(selection>'7'));

  // Selection is an ASCII character so convert
  // to a plain integer.  Also, the user menu
  // is offset by one compared to Wash_programme_t
  // enumeration; so compensate accordingly
  //
  return (Wash_programme_t)(selection - '1');
}


// -----------------------------------------------------------------------------
// Program keys implementation
//
#else

#include <stdio.h>
#include "programkeys.h"

Wash_programme_t get_programme_choice(void)
{
  puts("Select a program from the program switches...");
  return (Wash_programme_t)programkeys_get_selection();
}

#endif
