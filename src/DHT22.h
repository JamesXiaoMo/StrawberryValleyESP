#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 18
#define DHTTYPE DHT22

DHT_Unified DHT22_setup(){
    DHT_Unified sensor(DHTPIN, DHTTYPE);
    sensor.begin();
    return sensor;
}

char* DHT22_value(DHT_Unified sensor) {
    char* result = new char[10];
    char temperature[6];
    char humidity[6];
    sensors_event_t event;

    sensor.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        Serial.println("ReadingTemperatureError");
        delete[] result;
        return nullptr;
    }
    Serial.print("温度: ");
    Serial.print(event.temperature);
    Serial.println(" °C");
    dtostrf(event.temperature, 4, 1, temperature);

    sensor.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        Serial.println("ReadingHumidityError");
        delete[] result;
        return nullptr;
    }
    Serial.print("湿度: ");
    Serial.print(event.relative_humidity);
    Serial.println(" %");
    dtostrf(event.relative_humidity, 4, 1, humidity);

    snprintf(result, 10, "%s-%s", temperature, humidity);

    return result;
}