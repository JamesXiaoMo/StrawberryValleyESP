#include <WiFi.h>
#include <WiFiClient.h>

//初始化WiFiClient
WiFiClient client;

//设置网络参数
const char* SSID = "Jameswu_2.4G";
const char* PWD = "20030521";
const char* TCPServerAddr = "192.168.0.117";
int ServerPort = 1105;

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
    String receiveddata = "";
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
        if(receiveddata == "HB"){
            HeartBeat();
        }
    }
}