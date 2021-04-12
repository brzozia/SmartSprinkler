#define ARDUINO 243 // TODO
#include <Arduino.h>
#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>

#include "src/credentials.h"
#include "src/GeneralLogger/GeneralLogger.cpp"
#include "src/SettingsManager/SettingsManager.cpp"

Thread GNDHumiditySensorTH;
Thread DHTSensorReadTH;
ThreadController controller(10);

GeneralLogger logger;
SettingsManager settings;

void gnd_humidity_sensor_read_handler(void){

  
  //logger
  logger.notice("GND HUMIDITY READ::");
}
void dht_sensor_read_handler(void){

  //logger
  logger.notice("DHT SENSOR READ::");
}

void setup() {
  //Serial and logger
  Serial.begin(115200);
  while(!Serial){};
  logger.begin(LOG_LEVEL_VERBOSE, &Serial);

  //threading configuration
  GNDHumiditySensorTH.onRun(gnd_humidity_sensor_read_handler);
  GNDHumiditySensorTH.setInterval(1000);
  controller.add(&GNDHumiditySensorTH);

  DHTSensorReadTH.onRun(dht_sensor_read_handler);
  DHTSensorReadTH.setInterval(1000);
  controller.add(&DHTSensorReadTH);


  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  controller.run();

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
//file with libraries
// ArduinoThread
// ArduinoJSON
// AduinoLog
// ESP_EEPROM