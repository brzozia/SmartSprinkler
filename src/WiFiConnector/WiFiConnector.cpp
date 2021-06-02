#include "WiFiConnector.h"


WiFiConnector::WiFiConnector(){
    WiFi.mode(WIFI_STA);
    last_status = WiFi.status();
}

void WiFiConnector::WIFIConnect(){

    if(WiFi.status() ==  WL_CONNECTED && last_status != WL_CONNECTED){
            logger->notice("connected to WIFI \r\n");
            outMod->ledSetG(255);
            Serial.println(WiFi.localIP()); // TODO
            
            clockProvider->ntpSynchronize();
            weatherAPI->downloadData();
    }

    if (WiFi.status() != WL_CONNECTED) {
        outMod->ledSetG(0);
        logger->notice("trying to connect to WIFI\r\n");
        WiFi.disconnect();
        wl_status_t status = WiFi.begin(settings->get()->ssid, settings->get()->password);
        if(status == WL_CONNECT_FAILED){
            logger->error("wifi connection failed - wrong credentials\r\n");
        }

    }
    last_status = WiFi.status();
    
}

void WiFiConnector::disconnect() 
{
    WiFi.disconnect();
}