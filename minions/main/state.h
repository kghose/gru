/*
 * Defines a state struct and manages thread safe access to it.
 * Part of the Antifreeze program. https://github.com/kghose/antifreeze
 *
 * (c) 2024 Kaushik Ghose
 *
 * Released under the MIT License
 */

#ifndef _STATE_H_
#define _STATE_H_

#include <time.h>

#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"

typedef struct {
  float required_temp_c;
  float temp_trim_c;
  float room_temp_c;
} State;

esp_err_t initialize_state();

void set_required_temp_c(float);
float get_required_temp_c();

void set_temp_trim_c(float);
float get_temp_trim_c();

void set_room_temp_c(float);
float get_room_temp_c();

State get_state();

#endif  //_STATE_H_
