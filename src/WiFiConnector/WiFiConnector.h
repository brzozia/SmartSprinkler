#ifndef __WIFICONNECTOR_H__
#define __WIFICONNECTOR_H__

#include "../globals.h"
#include <esp8266wifi.h>

class WiFiConnector{

    private:

    public:
        WiFiConnector();
        void WIFIConnect();

};

#endif // __WIFICONNECTOR_H__