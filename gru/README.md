# Gru


# Hardware connections

```
[ Boiler relay box ]===========++==============[ Thermostat ]
                               ||
                               ||
                               ++==========[ Relay ]----- GPIO 33

[DS18B20 (outdoor temp)] -------------------------------- GPIO 15
```

# To build and flash

1. `idf.py add-dependency espressif/mdns` ([ESP mDNS](https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/protocols/mdns.html))
1. `idf.py flash`


# Techniques demonstrated

1. Digital output
1. RTOS multi-tasking
1. WiFi connection
1. mDNS
1. Http server
1. Getting time via SNTP
1. Dallas one wire protocol

# Appendix

## Halt on error configuration
(Found in: [Component config > ESP System Settings](panic))

[panic]: https://docs.espressif.com/projects/esp-idf/en/release-v5.2/esp32/api-reference/kconfig.html#config-esp-system-panic

**This should be set to reboot to avoid the possibility that the relay gets stuck in the on position. It also takes care of any transient faults that might have crashed the application.**

## Historical note

Gru started life as [Antifreeze](https://github.com/kghose/antifreeze).
