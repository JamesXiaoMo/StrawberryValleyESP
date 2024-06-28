#include <Arduino.h>
#include <Wi-Fi.h>
#include <Ticker.h>
#include <OTA.h>

const char* current_version = "1.0.0";
const char* project = "StrawberryValleyProject";


Ticker WiFiCheckTimeer(CheckWiFi, 60000);
Ticker SocketCheckTimer(CheckSocket, 65000);
Ticker OTACheckTimer([]() {OTA_update(current_version, project);}, 60000);

void setup(){
  Serial.begin(115200);
  WiFiSetup();
  mqtt_setup();
  WiFiCheckTimeer.start();
  //OTA_request(current_version);
  //TcpSocketSetup();
  //SocketCheckTimer.start();
  OTACheckTimer.start();
  addRelay(0, 15);
  addRelay(1, 16);
  OTA_update(current_version, project);
}

void loop(){
  WiFiCheckTimeer.update();
  OTACheckTimer.update();
  if (!client.connected()) {
    mqtt_reconnect();
  }
  mqtt_client.loop();
  
  // 发布消息
  //mqtt_client.publish("StrawberryValley", "Hello from ESP32");
  delay(100); 
  //SocketCheckTimer.update();
  //RecvData();
}