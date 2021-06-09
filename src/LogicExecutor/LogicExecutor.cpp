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
    if(!strategies_config.isFile()){
        logger->error("load strategies config failed\r\n");
        return;
    }
    uint8_t buffer[STRATEGIES_FILE_BUFFER_SIZE];
    strategies_config.read(buffer, STRATEGIES_FILE_BUFFER_SIZE);
    jsonDoc.clear();
    DeserializationError err=deserializeJson(jsonDoc, buffer);
    if(err) {
        logger->error("deserializeJson() of strategy configs failed with code %s \r\n", err.c_str());
    }   
    JsonArray arr = jsonDoc.as<JsonArray>();
    uint8_t idx = 0;
    for(JsonObject element: arr) {
        strategies[idx].interval_minutes = element["interval"];
        strategies[idx].enabled = element["enabled"];
        strncpy(strategies[idx].name, element["name"], 16);


        logger->notice("loaded strategy %s interval%d \r\n", strategies[idx].name, strategies[idx].interval_minutes);
        idx++;
        if(idx == MAX_STRATEGIES_NUMBER){
            logger->error("too many strategies in file\r\n");
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
    if(idx == -1){
        logger->error("add strategy failed\r\n");
        return false;
    } 
    File strategyFile = sdCard->openFile(("strategies/"+name+".json").c_str(), sdfat::O_WRITE | sdfat::O_CREAT);
    if(!strategyFile.isFile()){
        logger->error("add strategy failed\r\n");
        return false;
    }
    strategyFile.print(strategy);
    strncpy(strategies[idx].name, name.c_str(), 16);
    strategies[idx].enabled = enabled;
    strategies[idx].interval_minutes = interval;
    
    return persistConfiguration();
}

bool LogicExecutor::updateStrategyBody(const String &name, const String &strategy) 
{
    int idx = _find_strategy(name.c_str());
    if(idx == -1){
        return false;
    } 
    File strategyFile = sdCard->openFile(("strategies/"+name+".json").c_str(), sdfat::O_WRITE | sdfat::O_CREAT);
    if(!strategyFile.isFile()){
        logger->error("update strategy failed\r\n");
        return false;
    }
    strategyFile.print(strategy);
    strategyFile.close();
    return true;
}

bool LogicExecutor::updateStrategyState(const String &name, int enabled) 
{
    int idx = _find_strategy(name.c_str());
    if(idx == -1){
        logger->error("update strategy failed\r\n");
        return false;
    } 
    strategies[idx].enabled = enabled;
    persistConfiguration();
    return true;
}

bool LogicExecutor::updateStrategyInterval(const String &name, int interval) 
{
    int idx = _find_strategy(name.c_str());
    if(idx == -1){
        logger->error("update strategy failed\r\n");
        return false;
    }    
    strategies[idx].interval_minutes = interval;
    persistConfiguration();
    return true;    
}

bool LogicExecutor::persistConfiguration() 
{
    File strategies_config = sdCard->openFile("strategies/strategies_config.json", sdfat::O_WRITE);
    if(!strategies_config.isFile()){
        logger->error("persistConfig failed; couldnt open file\r\n");
        return false;
    }
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
        return false;
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
    logger->error("no strategy with name %s\r\n", c_name);
    return -1;
}
bool LogicExecutor::deleteStrategy(String &name) 
{
    const char * c_name = name.c_str();
    int idx = _find_strategy(c_name);
    if(idx == -1){
        return false;
    }

    strategies[idx].name[0] = '\0';
    strategies[idx].enabled = false;
    sdCard->deleteFile(("strategies/"+name+".json").c_str());
    persistConfiguration();
    return true;
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
    strategy_result_t tick_result;

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
                result.strategy_id = i;
                if(result.status){
                    if(result.duration_seconds > tick_result.duration_seconds){
                        tick_result = result;
                    }
                }
                logger->notice("status: %T \r\n", result.status);
                logger->notice("duration: %d \r\n", result.duration_seconds);
            }
        }
    }
    if(tick_result.status){
        if(outMod->pumpStatus() && outMod->getOnReason() == -2){
            logger->trace("pump already on by user; skipping");
        }else{
            outMod->pumpOnForTimeSec(tick_result.duration_seconds, tick_result.strategy_id);
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
                logger->notice("start watering\r\n");
                result.status = true;
                result.duration_seconds = element["V"];
            }else{
                logger->notice("off watering\r\n");
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
    }else if(v == TIME_HOURS){
        return clockProvider->getHours();
    }else if(v == TIME_MINUTES){
        return clockProvider->getHours();
    }else if(v == MAX_HUMIDITY_24){
        return weatherAPI->getMaxHumidity();
    }else if(v == MAX_TEMPERATURE_24){
        return weatherAPI->getMaxTemperature();
    }else if(v == RAIN_MAX_PROP_24){
        WeatherAPI::rainData raindData = weatherAPI->getTodaysRainInfo();
        return raindData.rainMaxProbability;
    }
    logger->error("parsing strategy; sensor %d does not exist\r\n", v);
    return 0.0;
}

