# Modbus Server
In this project the ESP32 devkit is a Modbus-Server which measures Temperature and AC Current via two sensors. The program is written using FreeRTOS tasks.

## Wifi Configuration
In the `config\config.h` you can set the Wifi configuration for ESP32   
```
/**
 * WiFi credentials   
 */
#define WIFI_NETWORK "------"
#define WIFI_PASSWORD "******"
```

## Modus Registers
Three Modbus Input Registers are defined with addresses 100, 101 and 102 as follows:   
```
const int Temp_IREG = 100;
const int Current_IREG = 101;
const int Watts_IREG = 102;
```
Every 100ms these registers are updated with `updateModbusRegs` task.   


## Schematic
This is the schematic of the hardware:   
![Schematic of ESP32 with SCT013 and DS18B20](https://github.com/h4med/ESP32-ModubsServer-FREERTOS/blob/main/Docs/ESP32_SCT013_DS18B20.jpg)    

Regarding interfacing of SCT AC current sensors you can refer to [Open Energy Monitor Docs](https://docs.openenergymonitor.org/electricity-monitoring/ct-sensors/interface-with-arduino.html)   
