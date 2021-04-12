#ifndef __SETTINGSMANAGER_H__
#define __SETTINGSMANAGER_H__
#include "../globals.cpp"
#include <ArduinoJson.h>
#include <Arduino.h>
#include <string.h>
#include <ESP_EEPROM.h>

#define SETTINGS_JSON_BUFFER_SIZE 128
#define SETTINGS_OBSERVER_ARR_SIZE 10
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
        char ssid[64];
        char password[64];
        char darkMode = false;
    };

private:
    StaticJsonDocument<SETTINGS_JSON_BUFFER_SIZE> doc;
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