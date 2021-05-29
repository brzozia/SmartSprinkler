#ifndef __WIFICONNECTOR_H__
#define __WIFICONNECTOR_H__

#include "../globals.h"
#include <ESP8266WiFi.h>
#include "../SettingsManager/SettingsManager.h"

class WiFiConnector{

    private:
        wl_status_t last_status;
    public:
        WiFiConnector();
        void WIFIConnect();
        void disconnect();

};

#endif // __WIFICONNECTOR_H__