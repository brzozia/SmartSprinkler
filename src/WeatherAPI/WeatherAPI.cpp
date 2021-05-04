#include "src/WeatherAPI/WeatherAPI.h"


WeatherAPI::WeatherAPI(){}

void WeatherAPI::downloadData(){
    http.begin(settings->get()->apiLink);
    int httpCode = http.GET();

    if(httpCode>0){
        payload = http.getString();
        jsonData = JSON.parse(payload);
    }else{
        logger->warning("http GET request error\r\n");
    }

    http.end();

    if (JSON.typeof(jsonData) == "undefined") {
        logger->warning("Parsing input failed!\r\n");
    }
}

JSONVar WeatherAPI::getJsonData(){
    return &jsonData;
}


// https://techtutorialsx.com/2016/07/17/esp8266-http-get-requests/