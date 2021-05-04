#include "src/WeatherAPI/WeatherAPI.h"


WeatherAPI::WeatherAPI(){
    parsed = false;
}

void WeatherAPI::downloadData(){
    http.begin(settings->get()->apiLink);
    int httpCode = http.GET();

    if(httpCode>0){ 
      data = jsonBuffer.parseObject(http.getString())["hourly"];
    }else{
        logger->warning("http GET request error\r\n");
    }

    if(!data.success()){
          logger->warning("parse data failed!\r\n");
          parsed = false;
    }else{
        fulfillArrays();
    }

    http.end();
    
}

void fulfillArrays(){
    for(int i=0;i<WEATHER_ARRAY_SIZE;i++){
        temperature[i] = data[i]["temperature"];
        humidity[i] = data[i]["humidity"];
        windSpeed[i] = data[i]["wind_speed"];
        rainVolume[i] = (data[i]["rain"] == null ? 0.0 : data[i]["rain"]["1h"]);
        rainProbability[i] = data[i]["pop"];
    }
    parsed = true;
    logger -> notice("Arrays fulfilled\r\n");
}

float WeatherAPI::getMaxTemperature(){
    if(parsed==false){
        logger->warning("Data not parsed. Cannot calculate.");
        return -1.0;
    }

    float max = -17.0;
    for(int i=0;i<WEATHER_ARRAY_SIZE;i++){
        if(temperature[i]>max){
            max = temperature[i];
        }
    }
    return max;
}

uint8 WeatherAPI::getMaxHumidity(){
    if(parsed==false){
        logger->warning("Data not parsed. Cannot calculate.");
        return;
    }

    uint8 max = 0;
    for(int i=0;i<WEATHER_ARRAY_SIZE;i++){
        if(humidity[i]>max){
            max = humidity[i];
        }
    }
    return max;
}

uint8 WeatherAPI::getMeanHumidity(){
    uint8 sum = 0;
    for(int i=0;i<WEATHER_ARRAY_SIZE;i++){
            sum += humidity[i];
    }
    return sum/WEATHER_ARRAY_SIZE;
}

float WeatherAPI::getMaxWindSpeed(){
    float max = 0.0;
    for(int i=0;i<WEATHER_ARRAY_SIZE;i++){
        if(windSpeed[i]>max){
            max = windSpeed[i];
        }
    }
    return max;
}

float WeatherAPI::getMeanWindSpeed(){
    float sum = 0.0;
    for(int i=0;i<WEATHER_ARRAY_SIZE;i++){
            sum += windSpeed[i];
    }
    return sum/WEATHER_ARRAY_SIZE;
}

rainData WeatherAPI::getTodaysRainInfo(){
    struct rainData rainData;
    rainData.sumVolumMM = 0;
    rainData.startHour = 30;
    rainData.endHour = 30;
    rainData.rainMaxProbability = 0;
    time_t timer;
    time(&timer);
    struct tm * timeinfo;
    timeinfo = localtime (&timer)

    for(int i=0;i<WEATHER_ARRAY_SIZE;i++){
        if(rainProbability[i] > rainData.rainMaxProbability){
            rainData.rainMaxProbability = rainProbability[i];
        }

        if(rainVolume[i] > 0){
            if(rainData.startHour == 30){
                rainData.startHour = (timeinfo->th_hour+i)%24;
            }
            rainData.sumVolumMM += rainVolume[i];
        }
        else{
            if(rainData.startHour != 30 && rainData.endHour == 30){
                rainData.endHour = (timeinfo->th_hour+i)%24;
            } 
        }
    }
    return rainData;
}

