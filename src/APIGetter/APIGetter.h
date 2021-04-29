#ifndef __APIGETTER_H__
#define __APIGETTER_H__

#include <esp8266httpclient.h>
#include <Arduino_JSON.h>
#include "../globals.h"
#include "../SettingsManager/SettingsManager.h"

class APIGetter{

    private:
        HTTPClient http;
        String payload;
        JSONVar jsonData;
    public:
        void tick();
        APIGetter();
        JSONVar getData();

};

#endif // __APIGETTER_H__