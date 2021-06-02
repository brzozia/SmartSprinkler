#include "WeatherAPI.h"

WeatherAPI::WeatherAPI(){
    parsed = false;
}

void WeatherAPI::downloadData(){
    http.begin(settings->get()->apiLink);
    int httpCode = http.GET();
    DeserializationError error;
    // Serial.println(http.getString());
    if(httpCode == 200){ 
     error = deserializeJson(data, http.getStream());
    //   data = jsonBuffer.parseObject(http.getString())["hourly"];
    }else{
        logger->warning("http GET request error\r\n error code: %d \r\n", httpCode);
    }

    if(error && httpCode == 200){
          logger->warning("parse data failed! %s\r\n", error.c_str());
          parsed = false;
    }else{
        fillArrays();
    }

    http.end();
    
}

void WeatherAPI::fillArrays(){
    for(int i=0;i<WEATHER_ARRAY_SIZE;i++){
        temperature[i] = data["hourly"][i]["temp"];
        humidity[i] = data["hourly"][i]["humidity"];
        windSpeed[i] = data["hourly"][i]["wind_speed"];
        rainVolume[i] = (data["hourly"][i]["rain"] == NULL ? 0.0 : data["hourly"][i]["rain"]["1h"]);
        rainProbability[i] = data["hourly"][i]["pop"];
    }
    
    downloadHour = clockProvider->getHours();
    parsed = true;
    logger -> notice("Arrays fulfilled\r\n");
}

float WeatherAPI::getMaxTemperature(){
    if(parsed==false){
        logger->warning("Data not parsed. Cannot calculate.\r\n");
        return -1.0;
    }

    float max = -237.0;
    for(int i=getCurrentID();i<WEATHER_ARRAY_SIZE;i++){
        if(temperature[i]>max){
            max = temperature[i];
        }
    }
    return max;
}

uint8 WeatherAPI::getMaxHumidity(){
    if(parsed==false){
        logger->warning("Data not parsed. Cannot calculate.\r\n");
        return 0;
    }

    uint8 max = 0;
    for(int i=getCurrentID();i<WEATHER_ARRAY_SIZE;i++){
        if(humidity[i]>max){
            max = humidity[i];
        }
    }
    return max;
}

uint8 WeatherAPI::getMeanHumidity(){
    if(parsed==false){
        logger->warning("Data not parsed. Cannot calculate.\r\n");
        return 0;
    }

    uint8 sum = 0;
    for(int i=getCurrentID();i<WEATHER_ARRAY_SIZE;i++){
            sum += humidity[i];
    }
    return sum/WEATHER_ARRAY_SIZE;
}

float WeatherAPI::getMaxWindSpeed(){
    if(parsed==false){
        logger->warning("Data not parsed. Cannot calculate.\r\n");
        return -1.0;
    }

    float max = 0.0;
    for(int i=getCurrentID();i<WEATHER_ARRAY_SIZE;i++){
        if(windSpeed[i]>max){
            max = windSpeed[i];
        }
    }
    return max;
}

float WeatherAPI::getMeanWindSpeed(){
    if(parsed==false){
        logger->warning("Data not parsed. Cannot calculate.\r\n");
        return -1.0;
    }

    float sum = 0.0;
    for(int i=getCurrentID();i<WEATHER_ARRAY_SIZE;i++){
            sum += windSpeed[i];
    }
    return sum/WEATHER_ARRAY_SIZE;
}

WeatherAPI::rainData WeatherAPI::getTodaysRainInfo(){
    struct rainData rainData;
    rainData.sumVolumMM = 0;
    rainData.startHour = WEATHER_ARRAY_SIZE+5;
    rainData.endHour = WEATHER_ARRAY_SIZE+5;
    rainData.rainMaxProbability = 0;

    if(parsed==false){
        logger->warning("Data not parsed. Cannot calculate.\r\n");
        return rainData;
    }

    for(int i=getCurrentID();i<WEATHER_ARRAY_SIZE;i++){
        if(rainProbability[i] > rainData.rainMaxProbability){
            rainData.rainMaxProbability = rainProbability[i];
        }

        if(rainVolume[i] > 0){
            if(rainData.startHour == (WEATHER_ARRAY_SIZE+5)){
                rainData.startHour = (downloadHour+i)%24;
            }
            rainData.sumVolumMM += rainVolume[i];
        }
        else{
            if(rainData.startHour != (WEATHER_ARRAY_SIZE+5) && rainData.endHour == (WEATHER_ARRAY_SIZE+5)){
                rainData.endHour = (downloadHour+i)%24;
            } 
        }
    }
    return rainData;
}



int WeatherAPI::getCurrentID(){
    int id = downloadHour - clockProvider->getHours();
    if(id<0){
        id = id + WEATHER_ARRAY_SIZE;
    }
    if(id >= 17){
        logger->warning("Need to download new data\r\n");
    }

    if(id == 23){
        logger->warning("Need to download new data!! Todays data not in memory!\r\n");
        id = 0;
    }
    return id;
}
