#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>
#include "src/globals.h"

#include "src/credentials.h"
#include "src/GeneralLogger/GeneralLogger.h"
#include "src/SettingsManager/SettingsManager.h"
#include "src/CommandParser/CommandParser.h"

Thread GNDHumiditySensorTH;
Thread DHTSensorReadTH;
ThreadController controller(10);

CommandParser cmdParser;

//globals
GeneralLogger *logger;
SettingsManager *settings;

void gnd_humidity_sensor_read_handler(void){

  
  //logger
  logger->trace("GND HUMIDITY READ::\r\n");
}
void dht_sensor_read_handler(void){

  //logger
  logger->trace("DHT SENSOR READ::\r\n");
}

void setup() {
  //Serial and logger
  Serial.begin(115200);
  while(!Serial){};
  logger = new GeneralLogger();
  settings = new SettingsManager();
  logger->begin(LOG_LEVEL_VERBOSE, &Serial);
  //threading configuration
  GNDHumiditySensorTH.onRun(gnd_humidity_sensor_read_handler);
  GNDHumiditySensorTH.setInterval(1000);
  controller.add(&GNDHumiditySensorTH);

  DHTSensorReadTH.onRun(dht_sensor_read_handler);
  DHTSensorReadTH.setInterval(1000);
  controller.add(&DHTSensorReadTH);


  pinMode(LED_BUILTIN, OUTPUT);
  logger->notice("\r\nsetup done\r\n");
}

void loop() {
  // controller.run();

  cmdParser.tick();
  // digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  // delay(1000);                       // wait for a second
  // digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  // delay(1000);                       // wait for a second

}
//file with libraries
// ArduinoThread
// ArduinoJSON
// AduinoLog
// ESP_EEPROM
// CmdParser
// https://www.hackster.io/brian-lough/3-simple-ways-of-programming-an-esp8266-12x-module-c514ee