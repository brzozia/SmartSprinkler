#include "WebServer.h"


WebServer::WebServer():server(PORT_NUMBER){
    this->StartServer();
}

void WebServer::StartServer(){
    server.on("/", [this]{handleHomePage();});
    server.on("/start", [this]{handleStartWatering();});
    server.on("/stop", [this]{handleStopWatering();});
    server.on("/status", [this]{handleGetStatus();});

    UriGlob strategyUri("/strategy/*");
    server.on(strategyUri, HTTP_GET, [this]{handleGetStrategy();});
    server.on(strategyUri, HTTP_PUT, [this]{handleUpdateStrategy();});
    server.on(strategyUri, HTTP_DELETE, [this]{handleDeleteStrategy();});
    server.on("/strategies", HTTP_GET, [this]{handleListStrategies();});
    server.on("/strategies", HTTP_POST, [this]{handleAddStrategy();});
    server.on("/sensors", HTTP_GET, [this]{handleGetSensors();});

    server.begin();
}

void WebServer::handleHomePage(){
    File dataFile = sdCard->openFile("index.html");
    int fsizeDisk = dataFile.size();
    server.sendHeader("Content-Length", (String)(fsizeDisk));
    // server.sendHeader("Cache-Control", "max-age=2628000, public"); // cache for 30 days
    size_t fsizeSent = server.streamFile(dataFile, "text/html");
    dataFile.close();


    //turn on sprinkler if was turned off
    //disable buton (in browser)
}

void WebServer::handleStartWatering(){
    int duration;
    String durationStr = server.arg("duration"); // in minnutes
    if(durationStr == ""){
        server.send(400, "application/json", "{\"error\": \"wrong request parameters\"}");
    }
    duration = durationStr.toInt();
    outMod->pumpOnForTimeSec(duration);
    server.send(200, "application/json", "{\"status\": 1, \"duration\":"+durationStr+"}");
}

void WebServer::handleStopWatering(){
    outMod->pumpOff();
    server.send(200, "application/json", "{\"status\": 0, \"duration\": 0}");

}

void WebServer::handleTestPage() 
{
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

void WebServer::handleGetSensors() 
{
    jsonDoc.clear();
    JsonObject sensor = jsonDoc.createNestedObject();
    sensor["name"] = "air_temp";
    sensor["value"] = outMod->readAirTemp();
    sensor["unit"] = "C";

    sensor = jsonDoc.createNestedObject();
    sensor["name"] = "air_humidity";
    sensor["value"] = outMod->readAirHumidity();
    sensor["unit"] = "%";
    
    sensor = jsonDoc.createNestedObject();    
    sensor["name"] = "soil_humidity";
    sensor["value"] = outMod->readSoilHumidity();  
    sensor["unit"] = "%";

    sensor = jsonDoc.createNestedObject();    
    sensor["name"] = "max_humidity24";
    sensor["value"] = weatherAPI->getMaxHumidity();
    sensor["unit"] = "%";

    sensor = jsonDoc.createNestedObject();    
    sensor["name"] = "max_temperature24";
    sensor["value"] = weatherAPI->getMaxTemperature();
    sensor["unit"] = "C";

    WeatherAPI::rainData raindData = weatherAPI->getTodaysRainInfo();
    sensor = jsonDoc.createNestedObject();    
    sensor["name"] = "rain_max_prop24";
    sensor["value"] = raindData.rainMaxProbability;
    sensor["unit"] = "";

    char buffer[512];
    serializeJson(jsonDoc, buffer, 512);
    server.send(200, "application/json", buffer);

}

void WebServer::handleGetStatus() 
{
    char message[64];
    sprintf(message, "{\"status\": %d, \"leftTimeSec\": %d}", outMod->pumpStatus(), outMod->getLeftTimeInSec());
    server.send(200, "application/json", message);
}

void WebServer::handleAddStrategy(){
    String name = server.arg("name");
    int enabled = server.arg("enabled").toInt();
    int interval = server.arg("interval").toInt();
    logicExec->addStrategy(name, server.arg("body"), enabled, interval);
    server.send(200, "text/json", "{\"name\":\""+name+"\"}");
}

void WebServer::handleUpdateStrategy() 
{
    String name = server.uri().substring(10);
    if(server.hasArg("body")){
        logicExec->updateStrategyBody(name, server.arg("body"));
    }
    if(server.hasArg("enabled")){
        logicExec->updateStrategyState(name, server.arg("enabled").toInt());
    }
    if(server.hasArg("interval")){
        logicExec->updateStrategyInterval(name, server.arg("interval").toInt());
    }
    server.send(200, "text/json", "{\"name\":\""+name+"\"}");
}

void WebServer::handleDeleteStrategy() 
{
    String uri = server.uri().substring(10);
    logicExec->deleteStrategy(uri);
    server.send(200, "application/json", "{\"name\":\""+uri+"\"}");
}


void WebServer::handleGetStrategy() 
{
    String uri = server.uri().substring(10);
    File dataFile = logicExec->getStrategyFile(uri);
    int fsizeDisk = dataFile.size();
    logger->notice("fsizeDisk: %d", fsizeDisk);
    // server.sendHeader("Cache-Control", "max-age=2628000, public"); // cache for 30 days
    size_t fsizeSent = server.streamFile(dataFile, "text/html");
    dataFile.close();        
}

void WebServer::handleListStrategies() 
{
    File dataFile = logicExec->getStrategyConfigFile();
    int fsizeDisk = dataFile.size();
    logger->notice("fsizeDisk: %d", fsizeDisk);
    // server.sendHeader("Content-Length", (String)(fsizeDisk));
    // server.sendHeader("Cache-Control", "max-age=2628000, public"); // cache for 30 days
    size_t fsizeSent = server.streamFile(dataFile, "application/json");
    dataFile.close();    
}



void WebServer::handleStrategyStatus() 
{
    
}

void WebServer::handleRunStrategy() 
{
    
}

void WebServer::handleClient() 
{
    server.handleClient();
}