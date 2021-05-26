#ifndef __WIFICONNECTOR_H__
#define __WIFICONNECTOR_H__

#include "../globals.h"
#include <ESP8266WiFi.h>
#include "../SettingsManager/SettingsManager.h"

class WiFiConnector{

    private:

    public:
        WiFiConnector();
        void WIFIConnect();

};

#endif // __WIFICONNECTOR_H__