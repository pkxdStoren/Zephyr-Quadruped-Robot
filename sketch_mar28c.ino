#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Your-WiFi-SSID";
const char* password = "Your-WiFi-Password";

// Joystick Pins
#define JOY1_X 34   // Joystick 1 - X axis
#define JOY1_Y 35   // Joystick 1 - Y axis
#define JOY2_X 32   // Joystick 2 - X axis
#define JOY2_Y 33   // Joystick 2 - Y axis

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) delay(1000);
}

void loop() {
    int joy1X = analogRead(JOY1_X);
    int joy1Y = analogRead(JOY1_Y);
    int joy2X = analogRead(JOY2_X);
    int joy2Y = analogRead(JOY2_Y);

    String command = "STOP";

    // **Joystick 1 - Movement Controls**
    if (joy1X > 800) command = "FORWARD";
    else if (joy1X < 200) command = "BACKWARD";
    else if (joy1Y > 800) command = "LEFT";
    else if (joy1Y < 200) command = "RIGHT";

    // **Joystick 2 - Rotation & Height Controls**
    else if (joy2X > 800) command = "ROTATE_RIGHT";
    else if (joy2X < 200) command = "ROTATE_LEFT";
    else if (joy2Y > 800) command = "HEIGHT_UP";
    else if (joy2Y < 200) command = "HEIGHT_DOWN";

    sendCommandToRobot(command);
    delay(100); // Small delay to avoid flooding requests
}

void sendCommandToRobot(String command) {
    HTTPClient http;
    http.begin("http://robot-ip-address/command");
    http.addHeader("Content-Type", "application/json");
    String payload = "{\"command\":\"" + command + "\"}";
    http.POST(payload);
    http.end();
}
