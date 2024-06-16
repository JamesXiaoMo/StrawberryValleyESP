#include <Arduino.h>
#include <Wi-Fi.h>
#include <Ticker.h>
#include <OTA.h>

const char* current_version = "1.0.0";

Ticker WiFiCheckTimeer(CheckWiFi, 60000);
Ticker SocketCheckTimer(CheckSocket, 65000);
Ticker OTACheckTimer([]() {OTA_request(current_version);}, 60000);

void setup(){
  Serial.begin(115200);
  WiFiSetup();
  WiFiCheckTimeer.start();
  //OTA_request(current_version);
  //TcpSocketSetup();
  //SocketCheckTimer.start();
  OTACheckTimer.start();
  addRelay(0, 15);
  addRelay(1, 16);
}

void loop(){
  WiFiCheckTimeer.update();
  OTACheckTimer.update();
  //SocketCheckTimer.update();
  //RecvData();
}