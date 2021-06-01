#ifndef __COMMANDPARSER_H__
#define __COMMANDPARSER_H__

#include <CmdParser.hpp>
#include "../globals.h"
#include "../GeneralLogger/GeneralLogger.h"
#include "../SettingsManager/SettingsManager.h"
#include "../WiFiConnector/WiFiConnector.h"
#include "../ClockProvider/ClockProvider.h"
class CommandParser{

    private:
        CmdParser cmdParser;
        CmdBuffer<64> myBuffer;
        const int BUFFER_SIZE = 64;
    public:
        void tick();
        CommandParser();

};

#endif // __COMMANDPARSER_H__