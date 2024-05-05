#include <Arduino.h>

int RelayPins[10];
int RelayState[10];

void RelayList(){
    Serial.println(
        "----------------Relay List----------------"
    );
    Serial.println(
        "-----NO.----|-----PIN------|----State-----"
    );
    for (int i = 0; i < 10; i++)
    {
        Serial.print("----[");
        Serial.print(i);
        Serial.print("]-----|-----[");
        Serial.print(RelayPins[i]);
        if (RelayPins[i]>9){
            Serial.print("]-----|-----");
        }else{
            Serial.print("]------|-----");
        }
        if (RelayState[i] == 1){
            Serial.print("ON-");
        }else if (RelayState[i] == 0)
        {
            Serial.print("OFF");
        }else{
            Serial.print("ERR");
        }
        Serial.println("------");
    }
    Serial.println(
        "------------------------------------------"
    );
}

void initRelay(){
    for (int i = 0; i < 10; i++)
    {
        pinMode(RelayPins[i], OUTPUT);
    }
}

void addRelay(int Addr ,int pinNum){
    RelayPins[Addr] = pinNum;
    RelayState[Addr] = 0;
    initRelay();
    RelayList();
}

void SwitchRelay(int Addr, int state){
    if (state == 0){
        digitalWrite(RelayPins[Addr], LOW);
        RelayState[Addr] = 0;
    }else if (state == 1){
        digitalWrite(RelayPins[Addr], HIGH);
        RelayState[Addr] = 1;
    }
    delay(1000);
    RelayList();
}