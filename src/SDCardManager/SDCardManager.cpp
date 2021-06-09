#include "SDCardManager.h"

SDCardManager::SDCardManager(){
    connectSDCard();
}

bool SDCardManager::connectSDCard(){
    pinMode(4,OUTPUT);
    digitalWrite(4,HIGH);
    bool status = SD.begin(4);
    // logger->notice("initialization done.");

//   File root = SD.open("/");

//   printDirectory(root, 0);

  logger->notice("done!\n\r");

    if(status==false){
        logger->error("SD failed to begin\r\n");
    }
    return status;
}

File SDCardManager::openFile(const char *filename, uint8_t mode) 
{
    if(!SD.exists(filename) && mode == FILE_READ){
        logger->error("file does not exist %s\r\n", filename);
        return SD.open(filename, mode);
    }else{
        return SD.open(filename, mode); // TODO
    }
    
}

bool SDCardManager::deleteFile(const char *filename) 
{
    return SD.remove(filename);
}

