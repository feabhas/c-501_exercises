// motor.h
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#pragma once
#ifndef MOTOR_H
#define MOTOR_H

typedef enum { CW, ACW } Motor_dir_t;

void motor_init(void);
void motor_on(void);
void motor_off(void);
void motor_set_direction(Motor_dir_t dir);
Motor_dir_t motor_get_direction(void);
void motor_change_direction(void);

#endif
