#include "CommandParser.h"
CommandParser::CommandParser() 
{
    cmdParser.setOptKeyValue(true);
}

void CommandParser::tick() 
{
    if(myBuffer.readSerialChar(&Serial)){
        if (cmdParser.parseCmd(&myBuffer) != CMDPARSER_ERROR) {
            if(cmdParser.equalCommand("setup-wifi")){
                char * ssid = cmdParser.getCmdParam(1);
                char * pass = cmdParser.getCmdParam(2);
                if(ssid != NULL && pass != NULL){
                    Serial.println("---WIFI SETUP---");
                    Serial.print("ssid: ");
                    Serial.print(ssid);
                    Serial.print("\r\npassword: ");
                    Serial.print(pass);
                    Serial.println("\r\n");
                    strncpy(settings->get()->password, pass, SETTINGS_WIFI_CRED_LENGTH);
                    strncpy(settings->get()->ssid, ssid, SETTINGS_WIFI_CRED_LENGTH);
                }else{
                    Serial.println("wrong parameters\r\nusage:  setup-wifi <ssid> <password>");
                }
            }else if(cmdParser.equalCommand("print-settings")){
                char buf[SETTINGS_JSON_BUFFER_SIZE];
                settings->getConfigJson(buf, SETTINGS_JSON_BUFFER_SIZE);
                Serial.println(buf);

            }else if(cmdParser.equalCommand("print-time")){
                clockProvider->printTime();

            }else if(cmdParser.equalCommand("sync-time")){
                clockProvider->ntpSynchronize();
                Serial.println("time synced");

            }else if(cmdParser.equalCommand("load-settings")){
                settings->loadFromMemory();
                Serial.println("loading from memory");

            }else if(cmdParser.equalCommand("print-free-memory")){
                Serial.print("Free memory: ");
                Serial.print(ESP.getFreeHeap());
                Serial.println("");

            }else if(cmdParser.equalCommand("weather-download")){
                weatherAPI->downloadData();

            }else if(cmdParser.equalCommand("persist-settings")){
                settings->persist();
                Serial.println("saving to memory");

            }else if(cmdParser.equalCommand("disconnect-wifi")){
                Serial.println("disconnecting...");
                wifiConn->disconnect();

            }else if(cmdParser.equalCommand("setup-api")){
                char * apiLink = cmdParser.getCmdParam(0);
                if(apiLink != NULL){
                    Serial.println("---API SETUP---");
                    Serial.print("api url: ");
                    Serial.print(apiLink);
                    Serial.println("\r\n");
                    strncpy(settings->get()->apiLink, apiLink, SETTINGS_APILINK_CRED_LENGTH);
                }else{
                    Serial.println("wrong parameters\r\nusage:  setup-api <url> ");
                }
            }else{
                Serial.println("There is no such command\r\n");
            }


        }else{
            logger->warning("command parsing error\r\n");
        }
    }
}
