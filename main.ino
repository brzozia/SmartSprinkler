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
#include "src/WebServer/WebServer.h"
#include "src/SDCardManager/SDCardManager.h"
#include "src/LogicExecutor/LogicExecutor.h"
#include "src/ClockProvider/ClockProvider.h"

Thread WiFiConnectorTH;
Thread keepAliveThread;
Thread logicExecutorTH;
Thread pumpOffTH;
Thread clockSynchronizationTH;

ThreadController controller(10);
CommandParser cmdParser;

//globals
GeneralLogger *logger;
SettingsManager *settings;
OutputModule *outMod;
WeatherAPI *weatherAPI;
WiFiConnector *wifiConn;
WebServer *server;
SDCardManager *sdCard;
LogicExecutor * logicExec;
ClockProvider *clockProvider;


void keep_alive_handler(void){
  outMod->tick();
}

void wifi_connect(void){
  wifiConn->WIFIConnect();
}

void logic_executor_handler(void){
  logicExec->tick();
}
void pump_off_check_handler(void){
  outMod->pumpOffCheck();
}

void clock_synchronize_handler(void){
  clockProvider->ntpSynchronize();
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
  logger->notice("output module started\r\n");

  weatherAPI = new WeatherAPI();
  logger->notice("made WeatherAPI object \r\n");

  sdCard = new SDCardManager();

  wifiConn = new WiFiConnector();
  logger->notice("wifi connector created\r\n");

  server = new WebServer();
  logger->notice("web server created and started\r\n");
  
  clockProvider = new ClockProvider();
  logger->notice("clockProvider created and started\r\n");

  logicExec = new LogicExecutor();
  logger->notice("logic executor started\r\n");

  //threading configuration

  WiFiConnectorTH.onRun(wifi_connect);
  WiFiConnectorTH.enabled=true;
  WiFiConnectorTH.setInterval(7000);
  controller.add(&WiFiConnectorTH);


  keepAliveThread.onRun(keep_alive_handler);
  keepAliveThread.setInterval(500);
  controller.add(&keepAliveThread);

  logicExecutorTH.onRun(logic_executor_handler);
  logicExecutorTH.setInterval(60000);
  controller.add(&logicExecutorTH);

  pumpOffTH.onRun(pump_off_check_handler);
  pumpOffTH.setInterval(1000);
  controller.add(&pumpOffTH);

  clockSynchronizationTH.onRun(clock_synchronize_handler);
  clockSynchronizationTH.setInterval(60000);
  controller.add(&clockSynchronizationTH);



  logger->notice("\r\nsetup done\r\n");

  if (WiFi.status() == WL_CONNECTED) {
      logger->notice("connected to WIFI \r\n");
      Serial.println(WiFi.localIP()); // TODO
  }
}

void loop() {
  controller.run();
  cmdParser.tick();
  server->handleClient();

}
