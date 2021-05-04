#ifndef __WEATHERAPI_H__
#define __WEATHERAPI_H__

#include <esp8266httpclient.h>
#include <Arduino_JSON.h>
#include <time.h>  
#include "../globals.h"
#include "../SettingsManager/SettingsManager.h"

#define WEATHER_ARRAY_SIZE 24
#define JSON_BUFFER_SIZE 24576

class WeatherAPI{

    struct rainData{
        uint8 startHour;
        uint8 endHour;
        float sumVolumMM;
        float rainMaxProbability;
    };

    private:
        HTTPClient http;
        DynamicJsonBuffer jsonBuffer(JSON_BUFFER_SIZE);
        JsonObject& data;
        boolean parsed;
        float temperature[WEATHER_ARRAY_SIZE];
        uint8 humidity[WEATHER_ARRAY_SIZE];
        float windSpeed[WEATHER_ARRAY_SIZE];
        float rainVolume[WEATHER_ARRAY_SIZE];
        float rainProbability[WEATHER_ARRAY_SIZE];
        void fulfillArrays();

    public:
        WeatherAPI();
        void downloadData();
        float getMaxTemperature();
        uint8 getMaxHumidity();
        uint8 getMeanHumidity();
        float getMaxWindSpeed();
        float getMeanWindSpeed();
        rainData getTodaysRainInfo();

};

#endif // __WEATHERAPI_H__