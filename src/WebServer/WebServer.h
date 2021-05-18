#ifndef __WEBSERVER_H__
#define __WEBSERVER_H__

#include "ESP8266WebServer.h"

#define PORT_NUMBER 80

class WebServer{

    private:
    ESP8266WebServer server(PORT_NUMBER);
    handleHomePage();
    handleStartWatering();
    handleStopWatering();
    handleAddRule();

    public:
    WebServer();


};


#endif //__WEBSERVER_H__