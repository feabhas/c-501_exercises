// gpio.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include "gpio.h"

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
static volatile uint32_t* const AHB1_enable = (uint32_t*)AHB1ENR_ADDR;

static volatile uint32_t* const mode   = (uint32_t*)GPIO_D_MODER_ADDR;
static volatile uint32_t* const input  = (uint32_t*)GPIO_D_IDR_ADDR;
static volatile uint32_t* const output = (uint32_t*)GPIO_D_ODR_ADDR;


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
