#ifndef __SDCARDMANAGER_H__
#define __SDCARDMANAGER_H__

#include <SD.h>
#include <SPI.h>
#include "../globals.h"
#include "../GeneralLogger/GeneralLogger.h"


#define CS_PIN 4
class SDCardManager{

    private:

    public:
    SDCardManager();
    bool connectSDCard();
    File openFile(const char *filename, uint8_t mode = FILE_READ);
    bool deleteFile(const char *filename);

};

#endif //__SDCARDMANAGER_H__