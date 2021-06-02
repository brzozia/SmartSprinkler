#ifndef __GENERALLOGGER_H__
#define __GENERALLOGGER_H__
#include <ArduinoLog.h>
#include "../globals.h"
#include "../OutputModule/OutputModule.h"
class GeneralLogger : public Logging
{
public:
    template <class T, typename... Args>
    void error(T msg, Args... args)
    {
        if(outMod != NULL) outMod->ledSetR(255);
        Logging::error(msg, args...);
    }
    // TODO
};

#endif // __GENERALLOGGER_H__