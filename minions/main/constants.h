/*
 * Program wide constants/defaults in one easily accessible place
 */
#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#define IR_PIN 13

#define TEMP_SENSOR_PIN 4
#define TEMP_SAMPLE_PERIOD_TICKS 10 * configTICK_RATE_HZ  // 10 sec

#define LED_PIN 2
#define LED_ON_TICKS 250 / portTICK_PERIOD_MS
#define LED_OFF_TICKS 1000 / portTICK_PERIOD_MS

#define NORMAL_HEARTBEAT_TICKS 5000 / portTICK_PERIOD_MS
#define FREEZE_DANGER_HEARTBEAT_TICKS 1000 / portTICK_PERIOD_MS

#define RELAY_ACTIVATED_LED_ON_TICKS 125 / portTICK_PERIOD_MS
#define RELAY_ACTIVATED_HEARTBEAT_TICKS 250 / portTICK_PERIOD_MS

#define MDNS_HOSTNAME CONFIG_MINION_NAME
#define MDNS_SERVICENAME "minion"
#define MDNS_INSTANCE_NAME "mini-split controller " CONFIG_MINION_NAME

#define TIME_ZONE "EST5EDT,M3.2.0,M11.1.0"
#define THE_END_OF_TIME LONG_MAX
#define NTP_SERVER "pool.ntp.org"

#endif