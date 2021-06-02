#ifndef __WEATHERAPI_H__
#define __WEATHERAPI_H__

#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "../globals.h"
#include "../SettingsManager/SettingsManager.h"
#include "../ClockProvider/ClockProvider.h"

#define WEATHER_ARRAY_SIZE 24
#define JSON_BUFFER_SIZE 18000

class WeatherAPI{


    private:
        HTTPClient http;
        StaticJsonDocument<JSON_BUFFER_SIZE> data;
        // JsonObject& data;
        bool parsed;
        int downloadHour;
        float temperature[WEATHER_ARRAY_SIZE];
        uint8 humidity[WEATHER_ARRAY_SIZE];
        float windSpeed[WEATHER_ARRAY_SIZE];
        float rainVolume[WEATHER_ARRAY_SIZE];
        float rainProbability[WEATHER_ARRAY_SIZE];
        void fillArrays();
        int getCurrentHour();
        int getCurrentID();

    public:
        struct rainData{
            uint8 startHour;
            uint8 endHour;
            float sumVolumMM;
            float rainMaxProbability;
        };
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