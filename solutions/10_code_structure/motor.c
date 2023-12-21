// motor.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include "motor.h"
#include "gpio.h"

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
  if (motor_get_direction() == CW) motor_set_direction(ACW);
  else                             motor_set_direction(CW);
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
