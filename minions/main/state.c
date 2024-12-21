#include "state.h"

#include "constants.h"

State state;
SemaphoreHandle_t state_mutex;

esp_err_t initialize_state() {
  state_mutex = xSemaphoreCreateMutex();
  if (state_mutex == NULL) {
    printf(
        "[ERROR] Could not initialize state "
        "mutex.");
    return ESP_ERR_INVALID_STATE;
  }
  state.required_temp_c = 21;
  state.temp_trim_c = 0;
  state.room_temp_c = 0;

  return ESP_OK;
}

void set_required_temp_c(float t) {
  xSemaphoreTake(state_mutex, portMAX_DELAY);  // block
  state.required_temp_c = t;
  xSemaphoreGive(state_mutex);
}

float get_required_temp_c() {
  float t;
  xSemaphoreTake(state_mutex, portMAX_DELAY);  // block
  t = state.required_temp_c;
  xSemaphoreGive(state_mutex);
  return t;
}

void set_room_temp_c(float t) {
  xSemaphoreTake(state_mutex, portMAX_DELAY);  // block
  state.room_temp_c = t;
  xSemaphoreGive(state_mutex);
}

float get_room_temp_c() {
  float t;
  xSemaphoreTake(state_mutex, portMAX_DELAY);  // block
  t = state.room_temp_c;
  xSemaphoreGive(state_mutex);
  return t;
}

void set_temp_trim_c(float t) {
  xSemaphoreTake(state_mutex, portMAX_DELAY);  // block
  state.temp_trim_c = t;
  xSemaphoreGive(state_mutex);
}

float get_temp_trim_c() {
  float t;
  xSemaphoreTake(state_mutex, portMAX_DELAY);  // block
  t = state.temp_trim_c;
  xSemaphoreGive(state_mutex);
  return t;
}

State get_state() {
  xSemaphoreTake(state_mutex, portMAX_DELAY);  // block
  State state_copy = state;
  xSemaphoreGive(state_mutex);
  return state_copy;
}
