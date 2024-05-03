#include <Arduino.h>
#include <Wi-Fi.h>
#include <Ticker.h>

Ticker WiFiCheckTimeer(CheckWiFi, 60000);
Ticker SocketCheckTimer(CheckSocket, 65000);

void setup(){
  Serial.begin(115200);
  WiFiSetup();
  WiFiCheckTimeer.start();
  TcpSocketSetup();
  SocketCheckTimer.start();
}

void loop(){
  WiFiCheckTimeer.update();
  SocketCheckTimer.update();
  RecvData();
}