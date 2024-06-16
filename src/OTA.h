#include <HTTPClient.h>
#include <Update.h>

const char* OTA_request_url = "http://192.168.0.116:5000/check_version";
const char* OTA_upgrade_url = "http://192.168.0.116:5000/upgrade/";


void OTA_update(String version){
    Serial.println("Start upgrading version:" + version);
    HTTPClient http;
    http.begin(OTA_upgrade_url + version);
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

void OTA_request(String current_version){
    HTTPClient http;
    http.begin(OTA_request_url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String postData = "version=" + current_version;
    int httpCode = http.POST(postData);

    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println("Server response: " + payload);
      if (payload == "NO_UPDATE") {
        Serial.println("No update needed.");
      } else {
        OTA_update(payload);
      }
    } else {
      Serial.println("Error on HTTP request");
    }
    http.end();
}
