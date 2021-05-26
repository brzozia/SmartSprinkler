#include "SDCardManager.h"

SDCardManager::SDCardManager(){}

bool SDCardManager::connectSDCard(){
    return SD.begin(CS_PIN);
}