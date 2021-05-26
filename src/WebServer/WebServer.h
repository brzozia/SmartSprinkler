#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include "ESP8266WebServer.h"

#define PORT_NUMBER 80

class WebServer{

    private:
    ESP8266WebServer server;
    void handleHomePage();
    void handleStartWatering();
    void handleStopWatering();
    void handleAddRule();

    public:
    WebServer();
    void StartServer();
    void handleClient();


};


#endif //__WEBSERVER_H__