#ifndef __GLOBALS_H__
#define __GLOBALS_H__
#include <ArduinoJson.h>

class OutputModule;
class GeneralLogger;
class SettingsManager;
class WeatherAPI;
class WiFiConnector;
class WebServer;
class SDCardManager;
class LogicExecutor;
class ClockProvider;

extern StaticJsonDocument<512> jsonDoc;
extern OutputModule *outMod;
extern SettingsManager *settings;
extern GeneralLogger *logger;
extern WeatherAPI *weatherAPI;
extern WiFiConnector *wifiConn;
extern WebServer *server;
extern SDCardManager *sdCard;
extern LogicExecutor *logicExec;
extern ClockProvider *clockProvider;

#endif // __GLOBALS_H__