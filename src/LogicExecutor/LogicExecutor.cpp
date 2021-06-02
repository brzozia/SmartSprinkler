#include "LogicExecutor.h"

LogicExecutor::LogicExecutor(void) 
{
    for(int i = 0; i < MAX_STRATEGIES_NUMBER; i++){
        strategies[i].name[0] = '\0';
        strategies[i].last_triggered_time = 0;
        strategies[i].enabled = false;
    }
    loadConfiguration();


}

void LogicExecutor::loadConfiguration() 
{
    File strategies_config = sdCard->openFile("strategies/strategies_config.json");
    uint8_t buffer[STRATEGIES_FILE_BUFFER_SIZE];
    strategies_config.read(buffer, STRATEGIES_FILE_BUFFER_SIZE);
    jsonDoc.clear();
    DeserializationError err=deserializeJson(jsonDoc, buffer);
    if(err) {
        logger->error("deserializeJson() failed with code %s \r\n", err.c_str());
    }   
    JsonArray arr = jsonDoc.as<JsonArray>();
    uint8_t idx = 0;
    for(JsonObject element: arr) {
        strategies[idx].interval_minutes = element["interval"];
        strategies[idx].enabled = element["enabled"];
        strncpy(strategies[idx].name, element["name"], 16);


        logger->notice("loaded strategy %s #%d \r\n", strategies[idx].name, strategies[idx].interval_minutes);
        idx++;
        if(idx == MAX_STRATEGIES_NUMBER){
            logger->error("too many strategies in file");
            break;
        }
    }
}

bool LogicExecutor::addStrategy(const String &name, const String &strategy, int enabled, int interval) 
{
    int idx = 0;
    for(int i = 0; i < MAX_STRATEGIES_NUMBER; i++){
        if(strategies[i].name[0] == '\0'){
            idx = i;
            break;
            // TODO handle max strateges number exceeded;
        }
    }
    File strategyFile = sdCard->openFile(("strategies/"+name+".json").c_str(), sdfat::O_WRITE | sdfat::O_CREAT);
    strategyFile.print(strategy);
    strncpy(strategies[idx].name, name.c_str(), 16);
    strategies[idx].enabled = enabled;
    strategies[idx].interval_minutes = interval;
    persistConfiguration();

}

bool LogicExecutor::updateStrategyBody(const String &name, const String &strategy) 
{
    int idx = _find_strategy(name.c_str());
    File strategyFile = sdCard->openFile(("strategies/"+name+".json").c_str(), sdfat::O_WRITE | sdfat::O_CREAT);
    strategyFile.print(strategy);
    strategyFile.close();
    return true;
}

bool LogicExecutor::updateStrategyState(const String &name, int enabled) 
{
    int idx = _find_strategy(name.c_str());
    strategies[idx].enabled = enabled;
    persistConfiguration();
}

bool LogicExecutor::updateStrategyInterval(const String &name, int interval) 
{
    int idx = _find_strategy(name.c_str());
    strategies[idx].interval_minutes = interval;
    persistConfiguration();    
}

void LogicExecutor::persistConfiguration() 
{
    File strategies_config = sdCard->openFile("strategies/strategies_config.json", sdfat::O_WRITE);
    char buffer[STRATEGIES_FILE_BUFFER_SIZE];
    jsonDoc.clear();
    for(int i = 0; i < MAX_STRATEGIES_NUMBER; i++){
        if(strategies[i].name[0] != '\0'){
            JsonObject strategy = jsonDoc.createNestedObject();
            strategy["name"] = strategies[i].name;
            strategy["interval"] = strategies[i].interval_minutes;
            strategy["enabled"] = strategies[i].enabled;
        }   
    } 

    size_t serialized_size = serializeJson(jsonDoc, buffer, STRATEGIES_FILE_BUFFER_SIZE);
    strategies_config.write(buffer, serialized_size);
    if(serialized_size == 0) {
        logger->error("serializeJson() failed with size %s \r\n", serialized_size);
    }   
    strategies_config.close();
}
int LogicExecutor::_find_strategy(const char * c_name){
    for(int i = 0; i < MAX_STRATEGIES_NUMBER; i++){
        if(!strcmp(strategies[i].name,c_name)){
            return i;
            // TODO handle no strategy;
        }
    } 
}
bool LogicExecutor::deleteStrategy(String &name) 
{
    const char * c_name = name.c_str();
    int idx = _find_strategy(c_name);
    strategies[idx].name[0] = '\0';
    strategies[idx].enabled = false;
    sdCard->deleteFile(("strategies/"+name+".json").c_str());
    persistConfiguration();
}

File LogicExecutor::getStrategyConfigFile() 
{
    return sdCard->openFile("strategies/strategies_config.json");
}

File LogicExecutor::getStrategyFile(String &name) 
{
    return sdCard->openFile(("strategies/"+name+".json").c_str());
}



void LogicExecutor::tick() 
{

    for(int i = 0; i < MAX_STRATEGIES_NUMBER; i++){
        if(strategies[i].name[0] != '\0' && strategies[i].enabled == true){
            if(strategies[i].last_triggered_time + strategies[i].interval_minutes*60*1000 < millis()){
                logger->notice("starting strategy %s \r\n", strategies[i].name);
                strategies[i].last_triggered_time = millis();
                char buff[STRATEGY_FILE_SIZE];
                sprintf(buff, "strategies/%s.json", strategies[i].name);
                File file = sdCard->openFile(buff);
                file.read((uint8_t *) buff, STRATEGY_FILE_SIZE);
                strategy_result_t result = checkStrategy(buff);
                logger->notice("status: %T \r\n", result.status);
                logger->notice("duration: %d \r\n", result.duration_seconds);
                if(result.status) outMod->pumpOnForTimeSec(result.duration_seconds);
            }
        }
    }
}

LogicExecutor::strategy_result_t LogicExecutor::checkStrategy(const char * strategy) 
{
    logger->notice("checking strategy\r\n");
    //TODO JSON PARSING ERRORS CAPACITY OVERFLOW ITP.
    jsonDoc.clear();
    DeserializationError err = deserializeJson(jsonDoc, strategy);
    if(err) {
        logger->error("deserializeJson() failed with code %s \r\n", err.c_str());
    }  
    JsonArray arr = jsonDoc.as<JsonArray>();
    strategy_result_t result;
    bool curr_state = false;
    bool strategy_state = false;
    uint8_t elem_num = 0;
    for(JsonObject element: arr) {
        if(element["T"] == CONDITION_CHECK){
            logger->notice("type: condition_check\r\n");
            float sensor_value = getSensorValue(element["D"]);
            float condition_value = element["V"];
            logger->notice("sensor: %F\r\n", sensor_value);
            logger->notice("condition_value: %F\r\n", condition_value);
            if(element["C"] == LOWER){
                curr_state = sensor_value < condition_value ? true : false;
            }else if(element["C"] == LOWER_OR_EQUALS){
                curr_state = sensor_value <= condition_value ? true : false;
            }else if(element["C"] == HIGHER){
                curr_state = sensor_value > condition_value ? true : false;
            }else if(element["C"] == HIGHER_OR_EQUALS){
                curr_state = sensor_value >= condition_value ? true : false;
            }else if(element["C"] == EQUALS){
                curr_state = sensor_value == condition_value ? true : false;
            }

            if(elem_num == 0){
                strategy_state = curr_state;
            }
        }else if(element["T"] == CONNECTOR){
            if(element["C"] == AND_CONNECTOR){
                strategy_state = strategy_state && curr_state;
            }else if(element["C"] == OR_CONNECTOR){
                strategy_state = strategy_state || curr_state;
            }
        }else if(element["T"] == RESULT){
            // INVOKE ACTIONS
            if(strategy_state){
                Serial.println("start watering\r\n");
                result.status = true;
                result.duration_seconds = element["V"];
            }else{
                Serial.println("off watering\r\n");
                result.status = false;
            }
        }
        elem_num++;
    }

    return result;
}

float LogicExecutor::getSensorValue(byte v) 
{
    if(v == AIR_HUMIDITY){
        return outMod->readAirHumidity();
    }else if(v == SOIL_HUMIDITY){
        return outMod->readSoilHumidity();
    }else if(v == AIR_TEMPERATURE){
        return outMod->readAirTemp();
    }else if(v == TIME){
    }
    return 0.0;
}

