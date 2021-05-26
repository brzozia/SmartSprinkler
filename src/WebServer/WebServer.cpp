#include "WebServer.h"


WebServer::WebServer():server(PORT_NUMBER){
    this->StartServer();
}

void WebServer::StartServer(){
    server.on("/", [this]{handleHomePage();});
    server.on("/start", [this]{handleStartWatering();});
    server.on("/stop", [this]{handleStopWatering();});
    server.on("/add_rule", [this]{handleAddRule();});

    server.begin();
}

void WebServer::handleHomePage(){
    server.send(200,"text/html"," \
    <html> \
    <head>  \ 
        <title>Sprinkler Page</title> \ 
        <meta name='viewport' content='width=device-width, initial-scale=1.0'> \  
    </head>  \
    <body>    \
        <h1>Smart Sprinkler</h1>  \
        <div id='div1'>        \
            <h4>TEST MIKROFONU</h4>\
        </div> \
        </body> \ 
    </html>\
    ");
}

void WebServer::handleStartWatering(){
    //turn on sprinkler if was turned off
    //disable buton (in browser)
}

void WebServer::handleStopWatering(){
    //turn off sprinkler if was turned on
    //disable buton (in browser)
}

void WebServer::handleAddRule(){}

void WebServer::handleClient() 
{
    server.handleClient();
}