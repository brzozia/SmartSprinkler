#include <WiFiConnector.h>


WiFiConnector::WiFiConnector(){}

void WiFiConnector::WIFIConnect(){
    logger->notice("trying to connect to WIFI\r\n");
    WiFi.begin(settings->get()->ssid, settings->get()->password);
    
    int i=0;
    while (WiFi.status() != WL_CONNECTED) {
        i+=1;
        if(i%3000==0){
            logger->notice("Connecting..\r\n");
        }
    }

}