#include <SoftwareSerial.h>

// Define SoftwareSerial Pins (RX, TX)
SoftwareSerial BTSerial(12, 13); // RX, TX for Bluetooth module

// Motor Control Pins
const int IN1 = 2;
const int IN2 = 3;
const int IN3 = 4;
const int IN4 = 5;

void setup() {
  // Set motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Initialize Serial communication for debugging (Serial Monitor)
  Serial.begin(9600);
  Serial.println("Bluetooth and Serial Robot Control Initialized.");
  
  // Initialize SoftwareSerial for Bluetooth communication
  BTSerial.begin(9600);
  Serial.println("Send commands via Serial Monitor or Bluetooth terminal.");
  Serial.println("Commands: F (Forward), B (Backward), L (Left), R (Right), S (Stop).");
}

void stopMotors() {
  // Stop both motors
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  
  // Print motor state info
  Serial.println("Motors stopped.");
  printMotorState();
}

void moveForward() {
  // Move robot forward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
  // Print motor state info
  Serial.println("Moving forward.");
  printMotorState();
}

void moveBackward() {
  // Move robot backward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
  // Print motor state info
  Serial.println("Moving backward.");
  printMotorState();
}

void turnLeft() {
  // Turn robot left
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  
  // Print motor state info
  Serial.println("Turning left.");
  printMotorState();
}

void turnRight() {
  // Turn robot right
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  
  // Print motor state info
  Serial.println("Turning right.");
  printMotorState();
}

void printMotorState() {
  // Print the state of each motor to Serial Monitor
  Serial.print("Motor 1 (IN1, IN2): ");
  if (digitalRead(IN1) == HIGH) {
    Serial.print("ON");
  } else {
    Serial.print("OFF");
  }
  Serial.print(" / ");
  if (digitalRead(IN2) == HIGH) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }

  Serial.print("Motor 2 (IN3, IN4): ");
  if (digitalRead(IN3) == HIGH) {
    Serial.print("ON");
  } else {
    Serial.print("OFF");
  }
  Serial.print(" / ");
  if (digitalRead(IN4) == HIGH) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
}

void loop() {
  // Check if a command is available from either Serial Monitor or Bluetooth
  if (Serial.available()) {
    char command = Serial.read();  // Read the command from Serial Monitor

    // Print received command to Serial Monitor for debugging
    Serial.print("Received from Serial Monitor: ");
    Serial.println(command);

    // Execute corresponding action based on the command
    executeCommand(command);
  }

  if (BTSerial.available()) {
    char command = BTSerial.read();  // Read the command from Bluetooth

    // Print received command to Serial Monitor for debugging
    Serial.print("Received from Bluetooth: ");
    Serial.println(command);

    // Execute corresponding action based on the command
    executeCommand(command);
  }
}

void executeCommand(char command) {
  // Execute corresponding action based on the received command
  switch (command) {
    case 'F':  // Move forward
      moveForward();
      break;
    case 'B':  // Move backward
      moveBackward();
      break;
    case 'L':  // Turn left
      turnLeft();
      break;
    case 'R':  // Turn right
      turnRight();
      break;
    case 'S':  // Stop motors
      stopMotors();
      break;
    default:
      // If an invalid command is received, stop the motors
      Serial.println("Invalid command. Stopping motors.");
      stopMotors();
      break;
  }
}
