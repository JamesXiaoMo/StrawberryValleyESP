#include <WiFi.h>
#include <WiFiClient.h>
#include <Relay.h>
#include <PubSubClient.h>
#include <string.h>

//初始化WiFiClient
WiFiClient client;
PubSubClient mqtt_client(client);

//设置网络参数
const char* SSID = "Jameswu_2.4G";
const char* PWD = "20030521";
const char* TCPServerAddr = "192.168.0.117";
int ServerPort = 1105;
const char* mqtt_server = "192.168.0.102";
const int mqtt_port = 1883;
const char* mqtt_id= "ESP001";

//WiFi初始化，连接设定的Wi-Fi
void WiFiSetup(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PWD);
    Serial.print("Connecting Wi-Fi");
    while (WiFi.status()!= WL_CONNECTED){
        Serial.print(".");
        delay(1000);
    }
    Serial.println("");
    Serial.println("Successfully connected to the Wi-Fi!!!");
    Serial.print("SSID:");
    Serial.println(WiFi.SSID());
    Serial.print("IP:");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI:");
    Serial.println(WiFi.RSSI());
    delay(1000);
}

//检查Wi-Fi是否被断开，断开执行重连
void CheckWiFi(){
    if (WiFi.status() != WL_CONNECTED){
        Serial.println("Unable to connect to Wi-Fi.Reconnecting!!!");
        WiFiSetup();
    }
}

//初始化TCPSocket，连接设定的服务器
void TcpSocketSetup(){
    Serial.print("Start connecting to the server");
    while (!client.connected())
    {
        client.connect(TCPServerAddr, ServerPort);
        delay(1000);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("Successfully connected to the server!!!");
}

//检查服务器是否被断开，断开执行重连
void CheckSocket(){
    if (!client.connected()){
        Serial.println("Unable to connect to server.Reconnecting!!!");
        TcpSocketSetup();
    }
}

//心跳包测试网速测试
void HeartBeat(){
    client.print("HB_OK");
}

//TCP接收数据
void RecvData(){
    String receiveddata;
    if (client.available()){
        while (true)
        {
            char c = client.read();
            if (c == '\n'){
                break;
            }
            receiveddata += c;
        }
        Serial.println(receiveddata);
        if (receiveddata == "15"){
            SwitchRelay(0, 1);
            delay(2000);
            SwitchRelay(0, 0);
        }
    }
}

//MQTT重连服务器
void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (mqtt_client.connect(clientId.c_str())) {
      Serial.println("connected");
      // 订阅主题
      mqtt_client.subscribe("StrawberryValley");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void mqtt_command_handle(String command){
    int separator = command.indexOf(":");
    if(separator != -1){
        Serial.println(separator);
        String relay_index = command.substring(0, separator);
        String relay_state = command.substring(separator + 1, command.length());
        SwitchRelay(relay_index.toInt(), relay_state.toInt());
    }
}

//MQTT回调函数
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(" => Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];
  }
  Serial.println(messageTemp);
  mqtt_command_handle(messageTemp);
}

//MQTT初始化
void mqtt_setup(){
    mqtt_client.setServer(mqtt_server, mqtt_port);
    mqtt_client.setCallback(callback);
}