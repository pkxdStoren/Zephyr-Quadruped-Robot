#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <HardwareSerial.h>

const char* ssid = "Your-WiFi-SSID";
const char* password = "Your-WiFi-Password";

HardwareSerial nanoSerial(2);

#define FLAME_SENSOR_PIN 15
#define DHTPIN 14
#define DHTTYPE DHT11

AsyncWebServer server(80);

void setup() {
    Serial.begin(115200);
    nanoSerial.begin(9600, SERIAL_8N1, 16, 13);
    
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
    }
    
    server.on("/command", HTTP_POST, [](AsyncWebServerRequest *request) {
        String command;
        if (request->hasParam("command")) {
            command = request->getParam("command", true)->value();
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
