#ifndef __SETTINGSMANAGER_H__
#define __SETTINGSMANAGER_H__

#include "../globals.h"
#include "../GeneralLogger/GeneralLogger.h"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <string.h>
#include <ESP_EEPROM.h>


#define SETTINGS_JSON_BUFFER_SIZE 512
#define SETTINGS_APILINK_CRED_LENGTH 512
#define SETTINGS_OBSERVER_ARR_SIZE 10
#define SETTINGS_WIFI_CRED_LENGTH 32
class ISettingsObserver
{
public:
    virtual ~ISettingsObserver() {}
    virtual void settings_notify() = 0;
};

class SettingsManager
{

    struct Config
    {
        char ssid[SETTINGS_WIFI_CRED_LENGTH];
        char password[SETTINGS_WIFI_CRED_LENGTH];
        char darkMode = false;
        char apiLink[SETTINGS_APILINK_CRED_LENGTH];
    };

private:
    struct Config config;
    ISettingsObserver *observers[SETTINGS_OBSERVER_ARR_SIZE];

public:
    SettingsManager();
    Config *get();
    void notify_all();
    virtual void subscribe(ISettingsObserver *obs);
    void loadConfigFromJson(const char *msg);
    void getConfigJson(char *txt, int size);
    void persist();
    void loadFromMemory();
};

#endif // __SETTINGSMANAGER_H__