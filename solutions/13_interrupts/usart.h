// usart.h
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#pragma once
#ifndef USART_H
#define USART_H

#include <stdbool.h>
#include <stdint.h>

void usart_init(void);
void usart_send(uint8_t c);
void usart_send_string(const char* str);
uint8_t  usart_get(void);
bool usart_try_get(uint8_t * const chr);

#endif
