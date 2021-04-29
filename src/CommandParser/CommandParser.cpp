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
                char * apiLink = cmdParser.getCmdParam(3);
                if(ssid != NULL && pass != NULL){
                    Serial.println("---WIFI SETUP---");
                    Serial.print("ssid: ");
                    Serial.print(ssid);
                    Serial.print("\r\npassword: ");
                    Serial.print(pass);
                    Serial.println("\r\n");
                    strncpy(settings->get()->password, pass, SETTINGS_WIFI_CRED_LENGTH);
                    strncpy(settings->get()->ssid, ssid, SETTINGS_WIFI_CRED_LENGTH);
                    strncpy(settings->get()->apiLink, apiLink, SETTINGS_APILINK_CRED_LENGTH);

                }else{
                    Serial.println("wrong parameters\r\nusage:  setup-wifi <ssid> <password>");
                }
            }else if(cmdParser.equalCommand("print-settings")){
                char buf[SETTINGS_JSON_BUFFER_SIZE];
                settings->getConfigJson(buf, SETTINGS_JSON_BUFFER_SIZE);
                Serial.println(buf);

            }else{
                Serial.println("There is no such command\r\n");
            }


        }else{
            logger->warning("command parsing error\r\n");
        }
    }
}
