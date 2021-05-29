#ifndef __LOGICEXECUTOR_H__
#define __LOGICEXECUTOR_H__

#include <ArduinoJson.h>
#include "../globals.h"
#include "../SDCardManager/SDCardManager.h"
#include "../OutputModule/OutputModule.h"
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

#define STRATEGY_JSON_BUFFER_SIZE 200
#define STRATEGIES_FILE_BUFFER_SIZE 256
#define MAX_STRATEGIES_NUMBER 10

class LogicExecutor{
    typedef struct strategy_result{
        bool status;
        int duration_minutes; // in minutes
    } strategy_result_t;
    typedef struct strategy_config{
        int interval_minutes;
        char name[32];
    }strategy_config_t;

    private:
        strategy_config_t strategies[MAX_STRATEGIES_NUMBER];
        StaticJsonDocument<STRATEGY_JSON_BUFFER_SIZE> doc;
        float getSensorValue(byte v);
        strategy_result_t checkStrategy(const char * strategy);
    public:
        LogicExecutor(void);
        void loadConfiguration();
        void tick(); // execute all strategies check weather to run

};

#endif // __LOGICEXECUTOR_H__