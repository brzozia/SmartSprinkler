#ifndef __GLOBALS_H__
#define __GLOBALS_H__


class OutputModule;
class GeneralLogger;
class SettingsManager;
class WeatherAPI;
class WiFiConnector;
class WebServer;

extern OutputModule *outMod;
extern SettingsManager *settings;
extern GeneralLogger *logger;
extern WeatherAPI *weatherAPI;
extern WiFiConnector *wifiConn;
extern WebServer *server;

#endif // __GLOBALS_H__