// gpio.h
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#pragma once
#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>
#include <stdbool.h>

// -----------------------------------------------------------------------------
// GPIO functions
//
void gpio_init(void);
void gpio_set_as_input(uint32_t pin);
void gpio_set_as_output(uint32_t pin);
void gpio_set_pin(uint32_t pin);
void gpio_clear_pin(uint32_t pin);
bool gpio_is_set(uint32_t pin);

#endif
