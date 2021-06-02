#ifndef __LOGICEXECUTOR_H__
#define __LOGICEXECUTOR_H__

#include "../globals.h"
#include <ArduinoJson.h>
#include "../SDCardManager/SDCardManager.h"
#include "../OutputModule/OutputModule.h"
#include <Arduino.h>

//ELEMENT TYPES
#define CONDITION_CHECK 1
#define CONNECTOR 2
#define RESULT 3

//DATA TYPES
#define AIR_HUMIDITY 1
#define SOIL_HUMIDITY 2
#define AIR_TEMPERATURE 3
#define TIME 4

//CONDITION TYPES
#define LOWER 1
#define LOWER_OR_EQUALS 2
#define HIGHER 3
#define HIGHER_OR_EQUALS 4
#define EQUALS 5

//CONNECTOR TYPES
#define AND_CONNECTOR 1
#define OR_CONNECTOR 2

#define STRATEGY_JSON_BUFFER_SIZE 1024
#define STRATEGIES_FILE_BUFFER_SIZE 1024
#define STRATEGY_FILE_SIZE 1024
#define MAX_STRATEGIES_NUMBER 10

class LogicExecutor{
    typedef struct strategy_result{
        bool status;
        int duration_seconds; // in seconds
    } strategy_result_t;



    private:
        float getSensorValue(byte v);
        strategy_result_t checkStrategy(const char * strategy);
        int _find_strategy(const char * name);
    public:
        typedef struct strategy_config{
        int interval_minutes;
        unsigned long last_triggered_time;
        bool enabled;
        char name[16];
        }strategy_config_t;

        LogicExecutor(void);
        strategy_config_t strategies[MAX_STRATEGIES_NUMBER];
        void loadConfiguration();
        bool addStrategy(const String &name, const String &strategy, int enabled, int interval);
        bool updateStrategyBody(const String &name, const String &strategy);
        bool updateStrategyState(const String &name, int enabled);
        bool updateStrategyInterval(const String &name, int interval);
        void persistConfiguration();
        bool deleteStrategy(String &name);
        File getStrategyConfigFile();
        File getStrategyFile(String &name);
        void tick(); // execute all strategies check weather to run

};

#endif // __LOGICEXECUTOR_H__