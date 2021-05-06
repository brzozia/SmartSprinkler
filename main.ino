#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>
#include "src/globals.h"

#include "src/credentials.h"
#include "src/GeneralLogger/GeneralLogger.h"
#include "src/SettingsManager/SettingsManager.h"
#include "src/OutputModule/OutputModule.h"
#include "src/CommandParser/CommandParser.h"
#include "src/WeatherAPI/WeatherAPI.h"
#include "src/WiFiConnector/WiFiConnector.h"

Thread GNDHumiditySensorTH;
Thread DHTSensorReadTH;
Thread WiFiConnectorTH;
Thread keepAliveThread;
ThreadController controller(10);
CommandParser cmdParser;

//globals
GeneralLogger *logger;
SettingsManager *settings;
OutputModule *outMod;
WeatherAPI *weatherAPI;
WiFiConnector *wifiConnector;

void gnd_humidity_sensor_read_handler(void){

  
  //logger
  logger->trace("GND HUMIDITY READ::\r\n");
}
void dht_sensor_read_handler(void){

  //logger
  logger->trace("DHT SENSOR READ::\r\n");
}
void keep_alive_handler(void){
  outMod->tick();
}

void wifi_connect(void){
  wifiConnector->WIFIConnect();
  logger->notice("connected to WIFI\r\n");
}

void setup() {
  //Serial and logger
  Serial.begin(115200);
  while(!Serial){};
  logger = new GeneralLogger();
  logger->begin(LOG_LEVEL_VERBOSE, &Serial);
  logger->notice("logger started\r\n");
  settings = new SettingsManager();
  logger->notice("settings manager started\r\n");

  outMod = new OutputModule();
  logger->notice("output module strated\r\n");

  logger->notice("command parser ready");

  weatherAPI = new WeatherAPI();
  logger->notice("made WeatherAPI object \r\n");
  
  //threading configuration
  GNDHumiditySensorTH.onRun(gnd_humidity_sensor_read_handler);
  GNDHumiditySensorTH.setInterval(1000);
  controller.add(&GNDHumiditySensorTH);
  
  DHTSensorReadTH.onRun(dht_sensor_read_handler);
  DHTSensorReadTH.setInterval(1000);
  controller.add(&DHTSensorReadTH);

  WiFiConnectorTH.onRun(wifi_connect);
  WiFiConnectorTH.setInterval(1000);
  controller.add(&WiFiConnectorTH);

  keepAliveThread.onRun(keep_alive_handler);
  keepAliveThread.setInterval(500);
  controller.add(&keepAliveThread);

  logger->notice("\r\nsetup done\r\n");
}

void loop() {
  controller.run();
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