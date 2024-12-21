/*
 * Minion: A ESP32 program to control Mitsubishi mini-splits.
 * See https://github.com/kghose/gru
 *
 * (c) 2024 Kaushik Ghose
 *
 * Released under the terms of the MIT License
 */

#include <esp_log.h>

#include "constants.h"
#include "driver/gpio.h"
#include "ds18b20.h"
#include "esp_netif_sntp.h"
#include "esp_sntp.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "httpserver.h"
#include "mdns.h"
#include "nvs_flash.h"
#include "owb.h"
#include "owb_rmt.h"
#include "state.h"
#include "wifi.h"

static const char* TAG = "minion";

void heartbeat_task() {
  gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
  while (true) {
    gpio_set_level(LED_PIN, 1);
    vTaskDelay(LED_ON_TICKS);
    gpio_set_level(LED_PIN, 0);
    vTaskDelay(LED_OFF_TICKS);
  }
}

void temperature_sample_task(void* pvParameter) {
  // There is something sensitive here, possibly task related:
  // If the initialization lines are moved to a separate function the ds18b20
  // stops responding.

  OneWireBus* owb = NULL;
  // Stable readings require a brief period before communication
  vTaskDelay(2000.0 / portTICK_PERIOD_MS);

  owb_rmt_driver_info rmt_driver_info;
  owb = owb_rmt_initialize(&rmt_driver_info, TEMP_SENSOR_PIN, RMT_CHANNEL_1,
                           RMT_CHANNEL_0);
  owb_use_crc(owb, true);  // enable CRC check for ROM code

  // We don't use an external pullup, so we need this.
  gpio_set_pull_mode(TEMP_SENSOR_PIN, GPIO_PULLUP_ONLY);

  OneWireBus_SearchState search_state = {0};
  bool found = false;
  while (!found) {
    ESP_LOGI(TAG, "Looking for DS18B20 outdoor temp probe.");
    owb_search_first(owb, &search_state, &found);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }

  char rom_code_s[OWB_ROM_CODE_STRING_LENGTH];
  owb_string_from_rom_code(search_state.rom_code, rom_code_s,
                           sizeof(rom_code_s));
  ESP_LOGI(TAG, "Probe found. ROM Code:  %s\n", rom_code_s);

  // Create DS18B20 device on the 1-Wire bus
  DS18B20_Info* ds18b20_info = ds18b20_malloc();  // heap allocation
  ds18b20_init_solo(ds18b20_info, owb);           // only one device on bus
  ds18b20_use_crc(ds18b20_info, true);  // enable CRC check on all reads
  ds18b20_set_resolution(ds18b20_info,
                         DS18B20_RESOLUTION_12_BIT);  // The counterfeit ones
                                                      // make trouble with 9bit

  float t_c = 0;
  while (true) {
    DS18B20_ERROR err = ds18b20_convert_and_read_temp(ds18b20_info, &t_c);
    set_room_temp_c(t_c);
    vTaskDelay(TEMP_SAMPLE_PERIOD_TICKS);
  }
}

void init_flash() {
  // NVS required for WiFi
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
      ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
}

void init_time() {
  esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG(NTP_SERVER);
  esp_netif_sntp_init(&config);
  if (esp_netif_sntp_sync_wait(pdMS_TO_TICKS(10000)) != ESP_OK) {
    ESP_LOGE(TAG, "Failed to update system time within 10s timeout");
  } else {
    ESP_LOGI(TAG, "Obtained time from " NTP_SERVER);
  }
  setenv("TZ", TIME_ZONE, 1);
  tzset();

  time_t now;
  char strftime_buf[64];
  struct tm timeinfo;

  time(&now);
  localtime_r(&now, &timeinfo);
  strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
  ESP_LOGI(TAG, "Antifreeze thinks the time is: %s\n", strftime_buf);
}

void start_mdns_service() {
  // initialize mDNS service
  esp_err_t err = mdns_init();
  if (err) {
    printf("MDNS Init failed: %d\n", err);
    return;
  }

  mdns_hostname_set(MDNS_HOSTNAME);
  mdns_instance_name_set(MDNS_INSTANCE_NAME);

  mdns_service_add(NULL, "_http", "_tcp", 80, NULL, 0);
  mdns_service_add(NULL, MDNS_SERVICENAME, "_tcp", 80, NULL, 0);
}

void app_main() {
  init_flash();
  wifi_init_sta();
  init_time();
  start_mdns_service();

  // TODO: error check inside the function
  ESP_ERROR_CHECK(initialize_state());

  TaskHandle_t heartbeat_task_h;
  TaskHandle_t temperature_sample_task_h;
  TaskHandle_t http_server_task_h;

  xTaskCreate(heartbeat_task, "Heartbeat", 1024, NULL, tskIDLE_PRIORITY,
              &heartbeat_task_h);
  xTaskCreate(temperature_sample_task, "Temp Sample", 4096, NULL,
              tskIDLE_PRIORITY, &temperature_sample_task_h);
  xTaskCreate(http_server_task, "HTTP server", 4096, NULL, tskIDLE_PRIORITY,
              &http_server_task_h);

  while (true) {
    State state = get_state();
    ESP_LOGI(TAG, "Temperature: %0.1f", state.room_temp_c);
    vTaskDelay(5 * configTICK_RATE_HZ);
  }
}
