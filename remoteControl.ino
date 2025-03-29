#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Your-WiFi-SSID"; // I am not stupid enough to type my wifi credentials
const char* password = "Your-WiFi-Password";

#define JOY1_X 36
#define JOY1_Y 39
#define JOY2_X 35
#define JOY2_Y 34

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED){
        delay(1000);
    }
}

void loop() {
    int xValue1 = analogRead(JOY1_X);
    int yValue1 = analogRead(JOY1_Y);
    int xValue2 = analogRead(JOY2_X);
    int yValue2 = analogRead(JOY2_Y);

    String command = "STOP";

    if (xValue1 > 1885) {
        command = "RIGHT";
    } else if (xValue1 < 1750) {
        command = "LEFT";
    } else if (yValue1 > 1900) {
        command = "FORWARD";
    } else if (yValue1 < 1750) {
        command = "BACKWARD";
    } else if (xValue2 > 1900) {
        command = "ROTATE_RIGHT";
    } else if (xValue2 < 1750) {
        command = "ROTATE_LEFT";
    } else if (yValue2 > 1900) {
        command = "HEIGHT_UP";
    } else if (yValue2 < 1750) {
        command = "HEIGHT_DOWN";
    } else {
        command = "STOP";
    }

    sendCommandToRobot(command);
    delay(100);
}

void sendCommandToRobot(String command) {
    HTTPClient http;
    http.begin("http://robot-ip-address/command");
    http.addHeader("Content-Type", "application/json");
    String payload = "{\"command\":\"" + command + "\"}";
    http.POST(payload);
    http.end();
}
