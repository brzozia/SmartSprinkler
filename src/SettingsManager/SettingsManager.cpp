#include "SettingsManager.h"
SettingsManager::SettingsManager() 
{
  EEPROM.begin(sizeof(SettingsManager::Config));
  config.ssid[0] = '\0';
  config.password[0] = '\0';
  strcpy(config.apiLink,"http://api.openweathermap.org/data/2.5/onecall?lat=50.097360&lon=19.893290&exclude=minutely,alerts,daily&appid=32f6d53f235e72a761414f1059f3df31&units=metric");
  config.darkMode = false;

}

SettingsManager::Config* SettingsManager::get(){
  return &config;
}

void SettingsManager::notify_all(){
  for(uint8_t i = 0; i < SETTINGS_OBSERVER_ARR_SIZE; i++){
    if(observers[i] != NULL){
      observers[i]->settings_notify();
    }
  }
}

void SettingsManager::subscribe(ISettingsObserver * obs){
  for(uint8_t i = 0; i < SETTINGS_OBSERVER_ARR_SIZE; i++){
      if(observers[i] == NULL){
          observers[i] = obs;
          return;
      }
  }
  logger->fatal(F("couldnt subscribe to settings; index reached end of array\r\n"));
  //LOG ERROR
}

void SettingsManager::loadConfigFromJson(const char *msg)
{

  DeserializationError error = deserializeJson(jsonDoc, msg);
  if (error)
  {
    logger->error(F("Failed to read configuration\r\n"));
    return;
  }

      
  strncpy(config.ssid, jsonDoc["ssid"] | "errr", SETTINGS_WIFI_CRED_LENGTH);
  strncpy(config.password, jsonDoc["password"] | "errr", SETTINGS_WIFI_CRED_LENGTH);
  strncpy(config.apiLink, jsonDoc["api"] | "errr", SETTINGS_APILINK_CRED_LENGTH);
  config.darkMode = jsonDoc['darkMode'] | false;

  notify_all();
}
void SettingsManager::getConfigJson(char *txt, int size)
{
  if (size < SETTINGS_JSON_BUFFER_SIZE)
  {
    //LOG ERROR
    logger->error("Failed to write config: buffer is too short\r\n");
  }
  jsonDoc.clear();
  jsonDoc["ssid"] = config.ssid;
  jsonDoc["password"] = config.password;
  jsonDoc["apiLink"] = config.apiLink;
  jsonDoc["darkMode"].set(config.darkMode);
  if (serializeJson(jsonDoc, txt, size) == 0)
  {
    //ERROR
    logger->error(F("Failed to write config to buffer\r\n"));
  }
}

void SettingsManager::loadFromMemory(){
  EEPROM.get(0, config);
  logger->notice(F("loaded config from memory\r\n"));
}

void SettingsManager::persist(){
  //remember to not call very often due to limited FLASH memory R/W cycles
  EEPROM.put(0, config);
  bool status = EEPROM.commit();
  if(status){
    logger->notice(F("wrote config to memory\r\n"));
  }else{
    logger->error("error writing settings to eeprom\r\n");
  }
}
