#include <WiFiConnector.h>


WiFiConnector::WiFiConnector(){
    logger->notice("trying to connect to WIFI\r\n");
    WiFi.begin(settings->get()->ssid, settings->get()->password);
}

void WiFiConnector::WIFIConnect(){
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        logger->notice("Connecting..\r\n");
    }

}