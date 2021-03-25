#include <StaticThreadController.h>
#include <Thread.h>
#include <ThreadController.h>

Thread GNDHumiditySensorTH;
Thread DHTSensorReadTH;
ThreadController controller(10);

void gnd_humidity_sensor_read_handler(void){

  //logger
}
void dht_sensor_read_handler(void){

  //logger
}

void setup() {
  //threading configuration
  GNDHumiditySensorTH.onRun(gnd_humidity_sensor_read_handler);
  GNDHumiditySensorTH.setInterval(1000);
  controller.add(&GNDHumiditySensorTH);

  DHTSensorReadTH.onRun(dht_sensor_read_handler);
  DHTSensorReadTH.setInterval(1000);
  controller.add(&DHTSensorReadTH);


  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  controller.run();

  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}