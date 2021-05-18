#ifndef __GLOBALS_H__
#define __GLOBALS_H__


class OutputModule;
extern OutputModule *outMod;
class GeneralLogger;
class SettingsManager;
extern SettingsManager *settings;
extern GeneralLogger *logger;
extern WeatherAPI *weatherAPI;
extern wifiConnector *wifiConnector;

#endif // __GLOBALS_H__