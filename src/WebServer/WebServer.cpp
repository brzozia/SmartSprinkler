#include "WebServer.h"


WebServer::WebServer():server(PORT_NUMBER){
    this->StartServer();
}

void WebServer::StartServer(){
    server.on("/index.html", [this]{handleHomePage();});
    server.on("/index.bundle.js", [this]{handleHomePage();});
    server.on("/start", [this]{handleStartWatering();});
    server.on("/stop", [this]{handleStopWatering();});
    server.on("/status", [this]{handleGetStatus();});

    UriGlob strategyUri("/strategy/*");
    server.on(strategyUri, HTTP_GET, [this]{handleGetStrategy();});
    server.on(strategyUri, HTTP_PATCH, [this]{handleUpdateStrategy();});
    server.on(strategyUri, HTTP_DELETE, [this]{handleDeleteStrategy();});
    server.on("/strategies", HTTP_GET, [this]{handleListStrategies();});
    server.on("/strategies", HTTP_POST, [this]{handleAddStrategy();});
    server.on("/sensors", HTTP_GET, [this]{handleGetSensors();});
    server.begin();
}

void WebServer::handleHomePage(){
    String name = server.uri().substring(1);
    File dataFile = sdCard->openFile(name.c_str());
    int fsizeDisk = dataFile.size();
    // server.sendHeader("Content-Length", (String)(fsizeDisk));
    server.sendHeader("Cache-Control", "max-age=2628000, public"); // cache for 30 days
    size_t fsizeSent = server.streamFile(dataFile, "text/html");
    dataFile.close();
}

void WebServer::handleStartWatering(){
    int duration;
    String durationStr = server.arg("duration"); // in minnutes
    if(durationStr == ""){
        server.send(400, "application/json", "{\"error\": \"wrong request parameters\"}");
        return;
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

    sensor = jsonDoc.createNestedObject();    
    sensor["name"] = "system_time";
    sensor["value"] = clockProvider->geTime();
    sensor["unit"] = "ms";
    
    WeatherAPI::rainData raindData = weatherAPI->getTodaysRainInfo();
    sensor = jsonDoc.createNestedObject();    
    sensor["name"] = "rain_max_prop24";
    sensor["value"] = raindData.rainMaxProbability;
    sensor["unit"] = "%";

    char buffer[1024];
    serializeJson(jsonDoc, buffer, 1024);
    server.send(200, "application/json", buffer);

}

void WebServer::handleGetStatus() 
{
    char message[64];
    sprintf(message, "{\"status\": %d, \"leftTimeSec\": %d, reason: %d}", outMod->pumpStatus(), outMod->getLeftTimeInSec(), outMod->getOnReason());
    server.send(200, "application/json", message);
}

void WebServer::handleAddStrategy(){
    if(!server.hasArg("body") || !server.hasArg("enabled") || !server.hasArg("interval")){
        server.send(400, "application/json", "{\"error\": \"wrong request parameters\"}");
        return;
    }
    String name = server.arg("name");
    if(name.length() == 0){
        server.send(400, "application/json", "{\"error\": \"no strategy name\"}");
        return;        
    }
    int enabled = server.arg("enabled").toInt();
    int interval = server.arg("interval").toInt();
    bool op_success = logicExec->addStrategy(name, server.arg("body"), enabled, interval);
    if(!op_success){
        server.send(500, "application/json", "{\"error\": \"couldnt add strategy\"}");
        return;          
    }
    server.send(200, "text/json", "{\"name\":\""+name+"\"}");
}

void WebServer::handleUpdateStrategy() 
{
    String name = server.uri().substring(10);
    bool op_success = false;
    if(server.hasArg("body")){
        op_success = logicExec->updateStrategyBody(name, server.arg("body"));
    }
    if(server.hasArg("enabled")){
        op_success = logicExec->updateStrategyState(name, server.arg("enabled").toInt());
    }
    if(server.hasArg("interval")){
        op_success = logicExec->updateStrategyInterval(name, server.arg("interval").toInt());
    }
    if(op_success){
        server.send(200, "application/json", "{\"name\":\""+name+"\"}");
    }else{
        server.send(500, "application/json", "{\"error\": \"couldnt update strategy\"}");
    }
}

void WebServer::handleDeleteStrategy() 
{
    String uri = server.uri().substring(10);
    if(uri.length() == 0){
        server.send(400, "application/json", "{\"error\": \"no strategy name\"}");
        return;        
    }    
    bool op_success = logicExec->deleteStrategy(uri);
    if(op_success){
        server.send(200, "application/json", "{\"name\":\""+uri+"\"}");
    }else{
        server.send(500, "application/json", "{\"error\": \"couldnt delete strategy\"}");
    }
}


void WebServer::handleGetStrategy() 
{
    String uri = server.uri().substring(10);
    File dataFile = logicExec->getStrategyFile(uri);
    if(!dataFile.isFile()){
        server.send(400, "application/json", "{\"error\": \"no such strategy file\"}");
        return;        
    }
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