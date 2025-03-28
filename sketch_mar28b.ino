#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Your-WiFi-SSID";
const char* password = "Your-WiFi-Password";

AsyncWebServer server(80);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(1000);
    server.on("/command", HTTP_POST, [](AsyncWebServerRequest *request) {
        String command;
        if (request->hasParam("command")) {
            command = request->getParam("command")->value();
            Serial.println(command);
        }
        request->send(200, "text/plain", "Command Received");
    });
    server.begin();
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        Serial.println("Sending to Arduino: " + command);
    }
}
