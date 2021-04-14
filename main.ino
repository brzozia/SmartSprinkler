#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>

#include "src/credentials.h"
#include "src/GeneralLogger/GeneralLogger.h"
#include "src/SettingsManager/SettingsManager.h"
#include "src/globals.h"

Thread GNDHumiditySensorTH;
Thread DHTSensorReadTH;
ThreadController controller(10);

//globals
GeneralLogger *logger;

SettingsManager *settings;

void gnd_humidity_sensor_read_handler(void){

  
  //logger
  logger->notice("GND HUMIDITY READ::");
}
void dht_sensor_read_handler(void){

  //logger
  logger->notice("DHT SENSOR READ::");
}

void setup() {
  //Serial and logger
  Serial.begin(115200);
  while(!Serial){};
  logger = new GeneralLogger();
  logger->begin(LOG_LEVEL_VERBOSE, &Serial);

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