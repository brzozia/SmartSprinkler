#include "WiFiConnector.h"


WiFiConnector::WiFiConnector(){}

void WiFiConnector::WIFIConnect(){
    if (WiFi.status() != WL_CONNECTED) {
        logger->notice("trying to connect to WIFI\r\n");
        WiFi.begin(settings->get()->ssid, settings->get()->password);

        if (WiFi.status() == WL_CONNECTED) {
            logger->notice("connected to WIFI \r");
            Serial.println(WiFi.localIP()); // TODO
        }
    }

    
    

}