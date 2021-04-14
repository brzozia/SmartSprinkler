#include <ArduinoLog.h>

class GeneralLogger: Logging{

    template <class T, typename... Args> void error(T msg, Args... args){
        Logging::error(msg, args...);
        //TODO blink red light
    }
}
