#ifndef __SDCARDMANAGER_H__
#define __SDCARDMANAGER_H__

#include <SD.h>
#include <SPI.h>


#define CS_PIN 8
class SDCardManager{

    private:

    public:
    SDCardManager();
    bool connectSDCard();

};

#endif //__SDCARDMANAGER_H__