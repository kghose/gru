# Minion

# Hardware

TODO

# Build and flash

```
idf.py add-dependency espressif/mdns
idf.py reconfigure
idf.py menuconfig
idf.py flash monitor
```

# Techniques demonstrated

1. Digital output
1. RTOS multi-tasking
1. WiFi connection
1. mDNS
1. Http server
1. Getting time via SNTP
1. Dallas one wire protocol
    - DS18b20 using ESP32 internal pull-up resistor
