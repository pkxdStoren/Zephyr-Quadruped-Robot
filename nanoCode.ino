#include <Wire.h>  // I2C communication
#include <Adafruit_PWMServoDriver.h> // PCA9685 Servo Library
#include <SoftwareSerial.h> // Serial Communictaion with esp32  

Adafruit_PWMServoDriver pca9685 = Adafruit_PWMServoDriver(0x40); // PCA9685 I2C Address
SoftwareSerial mqSerial(2, 3); // RX -> D2, TX -> D3

// Configuration Constants
const int SERVO_FREQ = 50;
const int MIN_PULSE = 150;
const int MAX_PULSE = 600;
const int MOVEMENT_DELAY = 300;
const int mqPin = A0;

void setup() {
    Serial.begin(115200);  // Start Serial Communication
	mqSerial.begin(9600); // SoftwareSerial for esp32
	
    while (!Serial) {
        ; // Wait for serial port to connect
    }
    
    pca9685.begin();
    pca9685.setPWMFreq(SERVO_FREQ); // Set frequency for servos

    Serial.println("Robot Control System Initialized");
}

void loop() {
	//First reading the air quality data, then sending it to the esp32 through serial
	int reading = analogRead(A0); 
	mqSerial.println(reading);
	
    if (Serial.available() > 0) {  // If a command is received
        String command = Serial.readStringUntil('\n');  // Read it
        command.trim(); // Remove whitespace
        processCommand(command);
    }
}

// Function to Move a Servo with Validation
void setServoAngle(int servoNum, int angle) {
    // Constrain angle between 0 and 180
    angle = constrain(angle, 0, 180);
    
    int pulse = map(angle, 0, 180, MIN_PULSE, MAX_PULSE);
    pca9685.setPWM(servoNum, 0, pulse);
    
    delay(90); // Small delay for servo movement
}

// Function to Process Movement Commands
void processCommand(String command) {
    command.toUpperCase(); // Convert to uppercase for case-insensitive comparison
    
    if (command == "FORWARD") moveForward();
    else if (command == "BACKWARD") moveBackward();
    else if (command == "LEFT") moveLeft();
    else if (command == "RIGHT") moveRight();
    else if (command == "ROTATE_RIGHT") rotateRight();
    else if (command == "ROTATE_LEFT") rotateLeft();
    else if (command == "HEIGHT_UP") adjustHeight(1);
    else if (command == "HEIGHT_DOWN") adjustHeight(-1);
    else {
        Serial.println("Error: Unknown command - " + command);
    }
}

// Movements with Added Delays
void moveForward() {
    Serial.println("Moving Forward");
    setServoAngle(0, 120); setServoAngle(3, 120); // Leg 1 & 4 Forward
    delay(MOVEMENT_DELAY);
    setServoAngle(1, 60);  setServoAngle(2, 60);  // Leg 2 & 3 Backward
}

void moveBackward() {
    Serial.println("Moving Backward");
    setServoAngle(0, 60); setServoAngle(3, 60);
    delay(MOVEMENT_DELAY);
    setServoAngle(1, 120); setServoAngle(2, 120);
}

void moveLeft() {
    Serial.println("Moving Left");
    setServoAngle(0, 120); setServoAngle(2, 120);
    delay(MOVEMENT_DELAY);
    setServoAngle(1, 60);  setServoAngle(3, 60);
}

void moveRight() {
    Serial.println("Moving Right");
    setServoAngle(0, 60); setServoAngle(2, 60);
    delay(MOVEMENT_DELAY);
    setServoAngle(1, 120); setServoAngle(3, 120);
}

void rotateRight() {
    Serial.println("Rotating Right");
    setServoAngle(0, 120); setServoAngle(3, 120);
    delay(MOVEMENT_DELAY);
    setServoAngle(1, 120); setServoAngle(2, 120);
}

void rotateLeft() {
    Serial.println("Rotating Left");
    setServoAngle(0, 60); setServoAngle(3, 60);
    delay(MOVEMENT_DELAY);
    setServoAngle(1, 60); setServoAngle(2, 60);
}

void adjustHeight(int direction) {
    String heightMessage = (direction == 1) ? "Raising Height" : "Lowering Height";
    Serial.println(heightMessage);
    
    int angle = (direction == 1) ? 120 : 60;
    for (int i = 4; i < 8; i++) {
        setServoAngle(i, angle);
        delay(50); // Small delay between servos
    }
}
