// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include "timer.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define GPIO_D_BASE_ADDR  (0x40020C00)
#define GPIO_D_MODER_ADDR (GPIO_D_BASE_ADDR + 0x00)
#define GPIO_D_IDR_ADDR   (GPIO_D_BASE_ADDR + 0x10)
#define GPIO_D_ODR_ADDR   (GPIO_D_BASE_ADDR + 0x14)

#define AHB1ENR_ADDR (0x40023830)

enum GPIO_port // We only need the first 4 GPIO ports
{
  GPIO_A,      // 0
  GPIO_B,      // 1
  GPIO_C,      // 2
  GPIO_D       // 3
};

enum LED { D3 = 11, D4 = 10, D5 = 9, D6 = 8 };

enum WMS_pin
{
  DOOR, // 0
  PS_1,
  PS_2,
  PS_3,
  CANCEL,
  ACCEPT,
  MOTOR_FB,
  UNUSED,
  SS_A, // 8
  SS_B,
  SS_C,
  SS_D,
  MOTOR_CTRL,
  MOTOR_DIR,
  LATCH,
  BUZZER
};


// Set up the register pointers
//
volatile uint32_t* const AHB1_enable = (uint32_t*)AHB1ENR_ADDR;
volatile uint32_t* const mode   = (uint32_t*)GPIO_D_MODER_ADDR;
volatile uint32_t* const input  = (uint32_t*)GPIO_D_IDR_ADDR;
volatile uint32_t* const output = (uint32_t*)GPIO_D_ODR_ADDR;


int main(void)
{
  // Set up the outputs and inputs
  // for the WMS devices.
  // Even though input is the default
  // mode, we are explicitly setting it
  // in this code.

  // Enable the GPIO port
  //
  *AHB1_enable |= (1u << GPIO_D);


  // Set up all pins as either input or output
  //  default to 0 to set all to inputs
  //
  uint32_t initial_mode_config = 0;

  // Inputs
  // Clear down all bit of interest to 0b00 value
  // Not required as initial_mode_config is already zero
  // If we needed to change in o/p to an i/p then would need to do this
  //
  // initial_mode_config &= ~(3 << (DOOR * 2));
  // initial_mode_config &= ~(3 << (PS_1 * 2));
  // initial_mode_config &= ~(3 << (PS_2 * 2));
  // initial_mode_config &= ~(3 << (PS_3 * 2));
  // initial_mode_config &= ~(3 << (CANCEL * 2));
  // initial_mode_config &= ~(3 << (ACCEPT * 2));
  // initial_mode_config &= ~(3 << (MOTOR_FB * 2));

  // Outputs
  // Set all bits to 0b01 for Output mode
  //
  initial_mode_config |= (1u << (SS_A * 2));
  initial_mode_config |= (1u << (SS_B * 2));
  initial_mode_config |= (1u << (SS_C * 2));
  initial_mode_config |= (1u << (SS_D * 2));
  initial_mode_config |= (1u << (MOTOR_CTRL * 2));
  initial_mode_config |= (1u << (MOTOR_DIR * 2));
  initial_mode_config |= (1u << (LATCH * 2));
  initial_mode_config |= (1u << (BUZZER * 2));

  // single write updated bits back to the mode register
  //
  *mode = initial_mode_config;

  // Flash an LED.  The LEDs use
  // the same pins as the seven-
  // segment display
  //
  puts("Flashing an LED...");
  for (int i = 0; i < 10; ++i)
  {
    *output |= (1u << D3);
    sleep(500);
    *output &= ~(1u << D3);
    sleep(500);
  }

  // Pulse the motor and change its
  // direction
  //
  puts("Driving the motor...");
  *output &= ~(1u << MOTOR_CTRL);    // ensure motor is off
  for (unsigned i = 0; i < 5; ++i)
  {
    *output ^= (1u << MOTOR_DIR);    // xor to toggle direction
    *output |= (1u << MOTOR_CTRL);   // motor on
    sleep(1000);
    *output &= ~(1u << MOTOR_CTRL);  // motor off
    sleep(1000);
  }

  // Count up on the seven-
  // segment display
  //
  puts("Display values on the seven-segment display...");
  for (unsigned i = 0; i < 10; ++i)
  {
    // You must clear any old value from the output
    // register before displaying a new one
    //
    uint32_t current_value = *input;       // read input register
    current_value &= ~(0x0Fu << SS_A);      // clear bits 8..11
    current_value |= ((i & 0x0F) << SS_A); // move int value to 8..11
    *output = current_value;               // write to output register
    sleep(500);
  }

  // Get an input from the program switches
  // and display it on the seven-segment
  //
  puts("Select a program from the program switches...");
  unsigned selection = 0;

  // Raise the latch
  //
  *output |= (1u << LATCH);

  uint32_t input_value; // outside loop for scoping reasons
  do
  {
    input_value = *input; // snapshot input register
    // Reset the latch (and hence program
    // switches) on CANCEL
    //
    if ((input_value & (1u << CANCEL)) != 0)
    {
      *output &= ~(1u << LATCH);
      *output |= (1u << LATCH);
      selection = 0;
      continue; // finish this iteration and re-read inputs
    }

    // Method 1 - read and mask individual bits
    // if ((input_value & (1u << PS_1)) != 0) selection |= 0x01;
    // if ((input_value & (1u << PS_2)) != 0) selection |= 0x02;
    // if ((input_value & (1u << PS_3)) != 0) selection |= 0x04;

    // Method 2 - mask bits 1-3 and right shift to lsb
    // selection = (input_value & 0x0E) >> 1;
    //
    selection = (input_value & ((1u << PS_3) | (1u << PS_2) | (1u << PS_1))) >> PS_1;

  } while ((input_value & (1u << ACCEPT)) == 0);

  // Drop the latch and wait for
  // ACCEPT to be released
  //
  *output &= ~(1u << LATCH);
  while ((*input & (1u << ACCEPT)) != 0)
  {
    ; // wait until Accept button released
  }

  // Display the value
  //
  uint32_t current_value = *input;
  current_value &= ~(0x0Fu << SS_A);
  current_value |= ((selection & 0x0F) << SS_A);
  *output = current_value;

  puts("Done");
}
