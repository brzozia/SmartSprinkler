#ifndef __WEATHERAPI_H__
#define __WEATHERAPI_H__

#include <esp8266httpclient.h>
#include <Arduino_JSON.h>
#include "../globals.h"
#include "../SettingsManager/SettingsManager.h"

class WeatherAPI{

    private:
        HTTPClient http;
        String payload;
        JSONVar jsonData;
    public:
        void tick();
        WeatherAPI();
        JSONVar getData();

};

#endif // __WEATHERAPI_H__