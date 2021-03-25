#include <ArduinoJson.h>
#include <Arduino.h>
#define SETTINGS_JSON_BUFFER_SIZE 128
#define SETTINGS_OBSERVER_ARR_SIZE 10

class ISettingsObserver{
    public:
        virtual ~ISettingsObserver() {}
        virtual void settings_notify() = 0;
};


class SettingsManager{
  struct Config
  {
      char ssid[64];
      char password[64];
  };

private:
  StaticJsonDocument<SETTINGS_JSON_BUFFER_SIZE> doc;
  struct Config config;
  ISettingsObserver * observers[SETTINGS_OBSERVER_ARR_SIZE];
public:
  Config* get(){
    return &config;
  }
  void notify_all(){
    for(uint8_t i = 0; i < SETTINGS_OBSERVER_ARR_SIZE; i++){
      if(observers[i] != NULL){
        observers[i]->settings_notify();
      }
    }
  }
  virtual void subscribe(ISettingsObserver * obs){
    for(uint8_t i = 0; i < SETTINGS_OBSERVER_ARR_SIZE; i++){
        if(observers[i] == NULL){
            observers[i] = obs;
            return;
        }
    }
    //LOG ERROR
  }
  void loadConfiguration(const char *msg)
  {

    DeserializationError error = deserializeJson(doc, msg);
    if (error)
    {
      Serial.println(F("Failed to read configuration")); // LOG ERROR
      return;
    }

        
    strlcpy(config.ssid, doc["ssid"] | "errr", 64);
    notify_all();

  }
  int get_config_json(char *txt, int size)
  {
    if (size < SETTINGS_JSON_BUFFER_SIZE)
    {
      //LOG ERROR
      Serial.println(F("Failed to write config: buffer is too short"));
      return -1;
    }
    doc.clear();
    doc["ssid"].set(config.ssid);

    doc["password"].set(config.password);
    if (serializeJson(doc, txt, size) == 0)
    {
      //ERROR
      Serial.println(F("Failed to write to file"));
    }
  }
};
// SettingsManager settings;
