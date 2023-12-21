// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"

// -----------------------------------------------------------------------------
// GPIO functions
//
void gpio_init(void);
void gpio_set_as_input(uint32_t pin);
void gpio_set_as_output(uint32_t pin);
void gpio_set_pin(uint32_t pin);
void gpio_clear_pin(uint32_t pin);
bool gpio_is_set(uint32_t pin);


// -----------------------------------------------------------------------------
// Motor functions
//
typedef enum { CW, ACW } Motor_dir_t;

void        motor_init(void);
void        motor_on(void);
void        motor_off(void);
void        motor_change_direction(void);
void        motor_set_direction(Motor_dir_t dir);
Motor_dir_t motor_get_direction(void);


// -----------------------------------------------------------------------------
// Seven-segment functions
//
void sevensegment_init(void);
void sevensegment_display(unsigned value);
void sevensegment_blank(void);


// -----------------------------------------------------------------------------
// Program selector functions
//
void     programkeys_init(void);
unsigned programkeys_get_selection(void);


// -----------------------------------------------------------------------------
// Buzzer functions
//
void buzzer_init(void);
void buzzer_sound(void);

// -----------------------------------------------------------------------------
// Door functions
//
void door_init(void);
bool door_is_open(void);



// -----------------------------------------------------------------------------
// Wash behaviour
//

typedef enum
{
  EMPTY = 1,
  FILL,
  HEAT,
  WASH,
  RINSE,
  SPIN,
  DRY,
  COMPLETE
} Step_num_t;


typedef enum
{
  WHITE,
  COLOUR,
  MIXED,
  ECONOMY,
  USER1,
  USER2
} Wash_programme_t;



typedef struct
{
  uint32_t on_time;
  uint32_t off_time;
  bool     reverse;
} Motor_config_t;


typedef struct
{
  Step_num_t     action;
  uint32_t       duration;
  Motor_config_t config;
} Step_t;



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


  motor_init();
  sevensegment_init();
  programkeys_init();
  buzzer_init();
  door_init();

  puts("Select a program from the program switches...");
  Wash_programme_t selection = (Wash_programme_t)programkeys_get_selection();

  // Using the relationship between arrays and pointers
  // to select the wash programme to run.
  //
  Step_t *prog;

  switch (selection)
  {
  case WHITE:
    prog = white_wash;
    break;

  case COLOUR:
    prog = colour_wash;
    break;

  case MIXED:
    prog = mixed_wash;
    break;

  case ECONOMY:
    prog = economy_wash;
    break;

  case USER1:
    prog = program_1;
    break;

  case USER2:
    prog = program_2;
    break;

  default:
    prog = NULL;
    break;
  }

  if (prog != NULL)
  {
    while (true)
    {
      // Display the step number
      //
      sevensegment_display(prog->action);

      // If the step drives the motor then
      // use the config info to control it.
      // Otherwise, just sleep for the duration
      // of the step.
      //
      if (prog->config.on_time != 0)
      {
        uint32_t num_cycles = (prog->duration / (prog->config.on_time + prog->config.off_time));
        for (uint32_t i = 0; i < num_cycles; ++i)
        {
          motor_on();
          sleep(prog->config.on_time);
          motor_off();
          sleep(prog->config.off_time);
          if (prog->config.reverse)
          {
            motor_change_direction();
          }
        }
      }
      else
      {
        sleep(prog->duration);
      }

      if (prog->action == COMPLETE) break;
      ++prog;
    }

  }

  puts("Done");
}


// -----------------------------------------------------------------------------
// GPIO helper functions
//
#define GPIO_D_BASE_ADDR    (0x40020C00)
#define GPIO_D_MODER_ADDR   (GPIO_D_BASE_ADDR + 0x00)
#define GPIO_D_IDR_ADDR     (GPIO_D_BASE_ADDR + 0x10)
#define GPIO_D_ODR_ADDR     (GPIO_D_BASE_ADDR + 0x14)

#define AHB1ENR_ADDR        (0x40023830)

typedef enum // We only need the first 4 GPIO ports
{
  GPIO_A,
  GPIO_B,
  GPIO_C,
  GPIO_D
} GPIO_port_t;

// Set up the register pointers
//
volatile uint32_t* const AHB1_enable = (uint32_t*)AHB1ENR_ADDR;

volatile uint32_t* const mode   = (uint32_t*)GPIO_D_MODER_ADDR;
volatile uint32_t* const input  = (uint32_t*)GPIO_D_IDR_ADDR;
volatile uint32_t* const output = (uint32_t*)GPIO_D_ODR_ADDR;


void gpio_init(void)
{
  *AHB1_enable |= (1u << GPIO_D);
}


void gpio_set_as_input(uint32_t pin)
{
  uint32_t current_mode = *mode;
  current_mode &= ~(3u << (pin * 2));
  *mode = current_mode;
}


void gpio_set_as_output(uint32_t pin)
{
  uint32_t current_mode = *mode;
  current_mode |= (1u << (pin * 2));
  current_mode &= ~(2u << (pin * 2));
  *mode = current_mode;
}


void gpio_set_pin(uint32_t pin)
{
  *output |= (1u << pin);
}


void gpio_clear_pin(uint32_t pin)
{
  *output &= ~(1u << pin);
}


bool gpio_is_set(uint32_t pin)
{
  return ((*input & (1u << pin)) != 0);
}


// -----------------------------------------------------------------------------
// Motor functions
//

enum { MOTOR_CTRL = 12, MOTOR_DIR = 13 };


void motor_init(void)
{
  gpio_init();

  gpio_set_as_output(MOTOR_CTRL);
  gpio_set_as_output(MOTOR_DIR);

  motor_off();
}


void motor_on(void)
{
  gpio_set_pin(MOTOR_CTRL);
}


void motor_off(void)
{
  gpio_clear_pin(MOTOR_CTRL);
}


void motor_change_direction(void)
{
  *output ^= (1u << MOTOR_DIR);
}


void motor_set_direction(Motor_dir_t dir)
{
  if (dir == CW) {
    gpio_clear_pin(MOTOR_DIR);
  }
  else {
    gpio_set_pin(MOTOR_DIR);
  }
}


Motor_dir_t motor_get_direction(void)
{
  return (Motor_dir_t)(gpio_is_set(MOTOR_DIR));
}


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
  *output &= ~(0x0Fu << SS_A);
  *output |= ((value & 0x0F) << SS_A);
}


void sevensegment_blank(void)
{
  sevensegment_display(15);
}


// -----------------------------------------------------------------------------
// Program selector functions
//

// 'Private' functions
//
void set_latch(void);
void unset_latch(void);


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

  uint32_t input_value; // outside loop for scoping reasons
  do
  {
    input_value = *input; // snapshot input register

    if ((input_value & (1u << CANCEL)) != 0)
    {
      unset_latch();
      set_latch();
      selection = 0;
    }

    selection = (input_value & ((1u << PS_3) | (1u << PS_2) | (1u << PS_1))) >> PS_1;

  } while (gpio_is_set(ACCEPT) == 0);

  unset_latch();
  while (gpio_is_set(ACCEPT) != 0)
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
