#include "WiFiConnector.h"


WiFiConnector::WiFiConnector(){}

void WiFiConnector::WIFIConnect(){
    if (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(settings->get()->ssid, settings->get()->password);
        logger->notice("trying to connect to WIFI\r\n");
    }
    if (WiFi.status() == WL_CONNECTED) {
        logger->notice("connected to WIFI\r\n");
    }

}