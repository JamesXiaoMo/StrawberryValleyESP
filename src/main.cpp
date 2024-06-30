#include <Arduino.h>
#include <Wi-Fi.h>
#include <Ticker.h>
#include <OTA.h>
#include <DHT22.h>

const char* current_version = "1.0.0";
const char* project = "StrawberryValleyProject";

DHT_Unified dht_sensor = DHT22_setup();

Ticker WiFiCheckTimeer(CheckWiFi, 60000);
Ticker SocketCheckTimer(CheckSocket, 65000);
Ticker OTACheckTimer([]() {OTA_update(current_version, project);}, 60000);
Ticker DHTUpload([]() {mqtt_client.publish("StrawberryValley", DHT22_value(dht_sensor));}, 60000);

void setup(){
  Serial.begin(115200);
  WiFiSetup();
  mqtt_setup();
  
  WiFiCheckTimeer.start();
  //OTA_request(current_version);
  //TcpSocketSetup();
  //SocketCheckTimer.start();
  OTACheckTimer.start();
  DHTUpload.start();
  addRelay(0, 15);
  addRelay(1, 16);
  OTA_update(current_version, project);
}

void loop(){
  WiFiCheckTimeer.update();
  OTACheckTimer.update();
  DHTUpload.update();
  if (!client.connected()) {
    mqtt_reconnect();
  }
  mqtt_client.loop();
  //mqtt_client.publish("StrawberryValley", DHT22_value(dht_sensor)); 
  // 发布消息
  //SocketCheckTimer.update();
  //RecvData();
}