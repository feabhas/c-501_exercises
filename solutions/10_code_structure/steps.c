// steps.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdint.h>
#include "steps.h"
#include "timer.h"
#include "buzzer.h"
#include "door.h"
#include "motor.h"
#include "programkeys.h"
#include "sevensegment.h"

// -----------------------------------------------------------------------------
// Wash functions
//

// 'Private' functions
//
static void drive_motor(const Motor_config_t * const cfg, uint32_t duration);
static void run_step(const Step_t * const step);


void wms_init(void)
{
  motor_init();
  sevensegment_init();
  programkeys_init();
  buzzer_init();
  door_init();
}


void drive_motor(const Motor_config_t * const cfg, uint32_t duration)
{
  uint32_t num_cycles = (duration / (cfg->on_time + cfg->off_time));

  for (uint32_t i = 0; i < num_cycles; ++i)
  {
    motor_on();
    sleep(cfg->on_time);
    motor_off();
    sleep(cfg->off_time);
    if (cfg->reverse)
    {
      motor_change_direction();
    }
  }
}


void run_step(const Step_t * const step)
{
  // Display the step number
    //
    sevensegment_display(step->action);

    // If the configuration on-time is zero
    // this step must not control the motor
    //
    if (step->config.on_time != 0)
    {
      drive_motor(&step->config, step->duration);
    }
    else
    {
      sleep(step->duration);
    }
}


void run_wash(Step_t prog[])
{
  int i = 0;

  // Since a wash programme only has a single
  // COMPLETE step, we can use this as a loop
  // terminator.
  //
  while (true)
  {
    run_step(&prog[i]);
    if (prog[i].action == COMPLETE) break;
    ++i;
  }
}
