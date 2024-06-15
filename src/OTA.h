#include <HTTPClient.h>
#include <Update.h>


const char* OTA_url = ""

void OTA(){
    HTTPClient http;
    http.begin(firmware_url);
    int httpCode = http.GET();
    if (httpCode == HTTP_CODE_OK) {
      int contentLength = http.getSize();
      bool canBegin = Update.begin(contentLength);
      if (canBegin) {
        WiFiClient* client = http.getStreamPtr();
        size_t written = Update.writeStream(*client);
        if (written == contentLength) {
          Serial.println("Written : " + String(written) + " successfully");
          if (Update.end()) {
            Serial.println("OTA done!");
            if (Update.isFinished()) {
              Serial.println("Update successfully completed. Rebooting.");
              ESP.restart();
            } else {
              Serial.println("Update not finished? Something went wrong!");
            }
          } else {
            Serial.println("Error Occurred. Error #: " + String(Update.getError()));
          }
        } else {
          Serial.println("Written only : " + String(written) + "/" + String(contentLength) + ". Retry?");
        }
      } else {
        Serial.println("Not enough space to begin OTA");
      }
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end();
  }
}