#include "SDCardManager.h"

SDCardManager::SDCardManager(){
    this->connectSDCard();
}

void printDirectory(File dir, int numTabs) {

  while (true) {

    File entry =  dir.openNextFile();

    if (! entry) {

      // no more files

      break;

    }

    for (uint8_t i = 0; i < numTabs; i++) {

     Serial.println('\t');

    }

    Serial.println(entry.name());

    if (entry.isDirectory()) {

      Serial.println("/");

      printDirectory(entry, numTabs + 1);

    } else {

      // files have sizes, directories do not

      Serial.println("\t\t");

      Serial.print(entry.size(), DEC);

    }

    entry.close();

  }
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
        logger->error("SD failed to begin");
    }
    return status;
}

File SDCardManager::openFile(const char *filename, uint8_t mode) 
{

    if(SD.exists(filename) && mode == FILE_READ){
        return SD.open(filename, mode);
    }else{
        logger->error("file does not exist");
        return SD.open(filename, mode); // TODO
    }
    
}
