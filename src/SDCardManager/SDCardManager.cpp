#include "SDCardManager.h"

SDCardManager::SDCardManager(){
    connectSDCard();
}

bool SDCardManager::connectSDCard(){

    return SD.begin(CS_PIN);
}

File SDCardManager::openFile(const char *filename, uint8_t mode) 
{
    if(SD.exists(filename) && mode == FILE_READ){
        return SD.open(filename, mode);
    }else{
        logger->error("file does not exist\r\n");
        return SD.open(filename, mode); // TODO
    }
    
}
