// UI.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include "UI.h"
#include "programkeys.h"


Wash_programme_t get_programme_choice(void)
{
  return (Wash_programme_t)programkeys_get_selection();
}
