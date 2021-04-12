#include "SettingsManager.h"
SettingsManager::SettingsManager() 
{
  EEPROM.begin(sizeof(SettingsManager::Config));

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
  logger.fatal(F("couldnt subscribe to settings; index reached end of array"));
  //LOG ERROR
}

void SettingsManager::loadConfigFromJson(const char *msg)
{

  DeserializationError error = deserializeJson(doc, msg);
  if (error)
  {
    logger.error(F("Failed to read configuration"));
    return;
  }

      
  strncpy(config.ssid, doc["ssid"] | "errr", 64);
  strncpy(config.password, doc["password"] | "errr", 64);
  config.darkMode = doc['darkMode'] | false;

  notify_all();
}
void SettingsManager::getConfigJson(char *txt, int size)
{
  if (size < SETTINGS_JSON_BUFFER_SIZE)
  {
    //LOG ERROR
    logger.error(F("Failed to write config: buffer is too short"));
  }

  doc.clear();
  doc["ssid"].set(config.ssid);
  doc["password"].set(config.password);
  doc["darkMode"].set(config.darkMode);

  if (serializeJson(doc, txt, size) == 0)
  {
    //ERROR
    logger.error(F("Failed to write config to buffer"));
  }
}
void SettingsManager::loadFromMemory(){
  EEPROM.get(sizeof(SettingsManager::Config), config);
  logger.notice(F("loaded config from memory"));
}
void SettingsManager::persist(){
  //remember to not call very often due to limited FLASH memory R/W cycles
  EEPROM.put(sizeof(SettingsManager::Config), config);
  EEPROM.commit();
  logger.notice(F("wrote config to memory"));
}

