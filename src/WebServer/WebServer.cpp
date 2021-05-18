#include "WebServer.h"

WebServer::WebServer(){
    server.on('/', handleHomePage);
    server.on('/start', handleStartWatering);
    server.on('/stop', handleStopWatering);
    server.on('/add_rule', handleAddRule);

    server.begin();
}

void handleHomePage(){
    server.send(200,"text/html","
    <html> 
    <head>   
        <title>Sprinkler Page</title> 
        <meta name='viewport' content='width=device-width, initial-scale=1.0'>  
    </head>  
    <body>    
        <h1>Smart Sprinkler</h1>    
        <div id='div1'>        
            <h4>TEST MIKROFONU</h4>
        </div> 
        </body>  
    </html>
    ");
}

void handleStartWatering(){
    //turn on sprinkler if was turned off
    //disable buton (in browser)
}

void handleStopWatering(){
    //turn off sprinkler if was turned on
    //disable buton (in browser)
}

void handleAddRule(){}