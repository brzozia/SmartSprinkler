#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include "ESP8266WebServer.h"
#include "../globals.h"
#include "../SDCardManager/SDCardManager.h"
#include "../OutputModule/OutputModule.h"
#include "../LogicExecutor/LogicExecutor.h"
#include "../WeatherAPI/WeatherAPI.h"
#include <ArduinoJson.h>
#include "uri/UriGlob.h"

#define PORT_NUMBER 80
#define JSON_DOCUMENT_SIZE 512


class WebServer{

    private:
    ESP8266WebServer server;
    void handleHomePage();
    void handleStartWatering();
    void handleStopWatering();
    void handleTestPage();
    void handleGetSensors();
    void handleGetStatus();
    void handleAddStrategy();
    void handleUpdateStrategy();
    void handleDeleteStrategy();
    void handleGetStrategy();
    void handleListStrategies();
    void handleStrategyStatus();
    void handleRunStrategy();

    public:
    WebServer();
    void StartServer();
    void handleClient();


};


#endif //__WEBSERVER_H__