#include <Arduino.h>
#include <WiFi.h>
#include <driver/adc.h>
#include <ModbusIP_ESP8266.h>

#include <OneWire.h> 
#include <DallasTemperature.h>

#include "config/config.h"
#include <EmonLib.h>

#define ONE_WIRE_BUS 14 

// Modbus Registers Offsets
const int Temp_IREG = 100;
const int Current_IREG = 101;
const int Watts_IREG = 102;

u16_t temperature_val = 0;
u16_t temperature_val_old;
u16_t current_val = 0;
u16_t watt_val = 0;
//Used Pins
const int ledPin = 25; //GPIO25

//ModbusIP object
ModbusIP mb;

OneWire oneWire(ONE_WIRE_BUS); 
DallasTemperature sensors(&oneWire);

EnergyMonitor emon1;

long ts;


void readDS18B20(void * parameter){

  for(;;){ // infinite loop
    sensors.requestTemperatures(); // Send the command to get temperature readings
    temperature_val = (u16_t) (sensors.getTempCByIndex(0) * 100);

    if (temperature_val > 8000){
      temperature_val = temperature_val_old;
    }

    #if DEBUG == true
      Serial.print("Temperature is: ");
      Serial.print(temperature_val);
      Serial.println("");
    #endif
    temperature_val_old = temperature_val;
    // Pause the task again for 2000ms
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void toggleLED(void * parameter){
  for(;;){

    if(WiFi.status() == WL_CONNECTED){
      digitalWrite(ledPin, HIGH);
      vTaskDelay(5000 / portTICK_PERIOD_MS);
      continue;
    }

    digitalWrite(ledPin, LOW);
    WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}


void updateModbusRegs(void * parameter){
  for(;;){ // infinite loop

    mb.task();
    mb.Ireg(Temp_IREG, temperature_val);
    mb.Ireg(Current_IREG, current_val);
    mb.Ireg(Watts_IREG, watt_val);

    // Pause the task again for 100ms
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}


void measureElectricity(void * parameter){
  for(;;){ // infinite loop
    long start = millis();

    double amps = emon1.calcIrms(1480);
    double watts = amps * HOME_VOLTAGE;

    current_val = (u16_t) (amps * 100);
    watt_val = (u16_t) watts;

    #if DEBUG == true
      Serial.print("Current is: ");
      Serial.print(amps);  
      Serial.println("");

      Serial.print("Watt is: ");
      Serial.print(watts);  
      Serial.println("");
      Serial.println("");
    #endif

    long end = millis();

    vTaskDelay((1000-(end-start)) / portTICK_PERIOD_MS);
  }
}

void setup() {
  #if DEBUG == true
    Serial.begin(115200);
  #endif  

  WiFi.begin(WIFI_NETWORK, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("");

  pinMode(ledPin, OUTPUT); // LED for Wifi Status


  xTaskCreate(
    readDS18B20,    // Function that should be called
    "Read DS18B20",   // Name of the task (for debugging)
    5000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    4,               // Task priority
    NULL             // Task handle
  ); 

  xTaskCreate(
    toggleLED,    // Function that should be called
    "TEst LED Blink",   // Name of the task (for debugging)
    10000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  ); 


  mb.server();
  
  // Add SENSOR_IREG register - Use addIreg() for analog Inputs
  mb.addIreg(Temp_IREG);  
  mb.addIreg(Current_IREG);
  mb.addIreg(Watts_IREG);

  xTaskCreate(
    updateModbusRegs,    // Function that should be called
    "Update Modbus Regs",   // Name of the task (for debugging)
    5000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    1,               // Task priority
    NULL             // Task handle
  );   


  // Setup the ADC
  adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11);
  analogReadResolution(ADC_BITS);
  pinMode(ADC_INPUT, INPUT);

  //for DS18B20
  sensors.begin();

  // Initialize emon library
  emon1.current(ADC_INPUT, 30);

  xTaskCreate(
    measureElectricity,    // Function that should be called
    "Measure Electricity",   // Name of the task (for debugging)
    5000,            // Stack size (bytes)
    NULL,            // Parameter to pass
    3,               // Task priority
    NULL             // Task handle
  );

}
 
void loop() {

  vTaskDelay(10000 / portTICK_PERIOD_MS);
}


