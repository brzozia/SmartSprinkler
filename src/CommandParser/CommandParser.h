#ifndef __COMMANDPARSER_H__
#define __COMMANDPARSER_H__

#include <CmdParser.hpp>
#include "../globals.h"
#include "../GeneralLogger/GeneralLogger.h"
#include "../SettingsManager/SettingsManager.h"
class CommandParser{

    private:
        CmdParser cmdParser;
        CmdBuffer<32> myBuffer;
        const int BUFFER_SIZE = 64;
    public:
        void tick();
        CommandParser();

};

#endif // __COMMANDPARSER_H__