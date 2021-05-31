#include "SDCardManager.h"

SDCardManager::SDCardManager(){
    connectSDCard();
}

bool SDCardManager::connectSDCard(){

    return SD.begin(CS_PIN);
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

