// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>
#include "steps.h"
#include "UI.h"


int main(void)
{
  Step_t white_wash[]  =
  {
    { .action = FILL,     .duration = 500 },
    { .action = WASH,     .duration = 2000, .config = { 500, 500, true } },
    { .action = EMPTY,    .duration = 500 },
    { .action = FILL,     .duration = 500 },
    { .action = HEAT,     .duration = 500 },
    { .action = WASH,     .duration = 2000, .config = { 500, 500, true } },
    { .action = EMPTY,    .duration = 500 },
    { .action = FILL,     .duration = 500 },
    { .action = RINSE,    .duration = 2000, .config = { 100, 900, true } },
    { .action = EMPTY,    .duration = 500 },
    { .action = SPIN,     .duration = 2000, .config = { 2000, 00, false } },
    { .action = DRY,      .duration = 500 },
    { .action = COMPLETE, .duration = 500 }
  };

  Step_t colour_wash[]  =
  {
    { .action = FILL,     .duration = 500 },
    { .action = HEAT,     .duration = 500 },
    { .action = WASH,     .duration = 2000, .config = { 500, 500, true } },
    { .action = EMPTY,    .duration = 500 },
    { .action = FILL,     .duration = 500 },
    { .action = RINSE,    .duration = 2000, .config = { 100, 900, true } },
    { .action = EMPTY,    .duration = 500 },
    { .action = SPIN,     .duration = 2000, .config = { 2000, 00, false } },
    { .action = DRY,      .duration = 500 },
    { .action = COMPLETE, .duration = 500 }
  };


  Step_t mixed_wash[]  =
  {
    { .action = FILL,     .duration = 500 },
    { .action = HEAT,     .duration = 500 },
    { .action = WASH,     .duration = 2000, .config = { 500, 500, true } },
    { .action = EMPTY,    .duration = 500 },
    { .action = FILL,     .duration = 500 },
    { .action = RINSE,    .duration = 2000, .config = { 100, 900, true } },
    { .action = EMPTY,    .duration = 500 },
    { .action = FILL,     .duration = 500 },
    { .action = RINSE,    .duration = 2000, .config = { 100, 900, true } },
    { .action = EMPTY,    .duration = 500 },
    { .action = SPIN,     .duration = 2000, .config = { 2000, 00, false } },
    { .action = DRY,      .duration = 500 },
    { .action = COMPLETE, .duration = 500 }
  };

  // to be implemented
  //
  Step_t economy_wash[] = { { .action = COMPLETE } };
  Step_t program_1[]    = { { .action = COMPLETE } };
  Step_t program_2[]    = { { .action = COMPLETE } };

   wms_init();
  _Bool done = 0;
  do {
    Wash_programme_t selection = get_programme_choice();

    switch (selection)
    {
    case WHITE:
      run_wash(white_wash);
      break;

    case COLOUR:
      run_wash(colour_wash);
      break;

    case MIXED:
      run_wash(mixed_wash);
      break;

    case ECONOMY:
      run_wash(economy_wash);
      break;

    case USER1:
      run_wash(program_1);
      break;

    case USER2:
      run_wash(program_2);
      break;

    case EXIT:
      done = 1;
      break;

    default:
      break;
    }
  }while(!done);

  puts("Done");
}

