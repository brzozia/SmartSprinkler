#include "LogicExecutor.h"

LogicExecutor::LogicExecutor(void) 
{
    for(int i = 0; i < MAX_STRATEGIES_NUMBER; i++){
        strategies[i].name[0] = '\0';
        strategies[i].last_triggered_time = 0;
    }
    loadConfiguration();


}

void LogicExecutor::loadConfiguration() 
{
    File strategies_config = sdCard->openFile("strategies/strategies_config.json");
    uint8_t buffer[STRATEGIES_FILE_BUFFER_SIZE];
    strategies_config.read(buffer, STRATEGIES_FILE_BUFFER_SIZE);
    DeserializationError err=deserializeJson(doc, buffer);
    if(err) {
        logger->error("deserializeJson() failed with code %s \r\n", err.c_str());
    }   
    JsonArray arr = doc.as<JsonArray>();
    uint8_t idx = 0;
    for(JsonObject element: arr) {
        strategies[idx].interval_minutes = element["interval"];
        strncpy(strategies[idx].name, element["name"], 16);

        logger->notice("loaded strategy %s #%d \r\n", strategies[idx].name, strategies[idx].interval_minutes);
        idx++;
        if(idx == MAX_STRATEGIES_NUMBER){
            logger->error("too many strategies in file");
            break;
        }
    }
}

void LogicExecutor::tick() 
{

    for(int i = 0; i < MAX_STRATEGIES_NUMBER; i++){
        if(strategies[i].name != '\0'){
            if(strategies[i].last_triggered_time + strategies[i].interval_minutes*60*1000 < millis()){
                logger->notice("starting strategy %s \r\n", strategies[i].name);
                strategies[i].last_triggered_time = millis();
                char buff[STRATEGY_FILE_SIZE];
                sprintf(buff, "strategies/%s", strategies[i].name);
                File file = sdCard->openFile(buff);
                file.read((uint8_t *) buff, STRATEGY_FILE_SIZE);
                strategy_result_t result = checkStrategy(buff);
                logger->notice("status: %T \r\n", result.status);
                logger->notice("duration: %d \r\n", result.duration_minutes);
            }
        }
    }
}

LogicExecutor::strategy_result_t LogicExecutor::checkStrategy(const char * strategy) 
{
    logger->notice("checking strategy\r\n");
    //TODO JSON PARSING ERRORS CAPACITY OVERFLOW ITP.
    DeserializationError err = deserializeJson(doc, strategy);
    if(err) {
        logger->error("deserializeJson() failed with code %s \r\n", err.c_str());
    }  
    JsonArray arr = doc.as<JsonArray>();
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
                result.duration_minutes = element["V"];
            }else{
                Serial.println("off watering\r\n");
                result.status = false;
            }
        }
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

