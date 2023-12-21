// steps.h
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#pragma once
#ifndef STEPS_H
#define STEPS_H

#include <stdbool.h>
#include <stdint.h>

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

void wms_init(void);
void run_wash(Step_t prog[]);


#endif
