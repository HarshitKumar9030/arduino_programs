// Pins for ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;

// Pins for relay control
const int relay1IN1 = 2; // Relay 1, Input 1 (Coca-Cola)
const int relay1IN2 = 3; // Relay 1, Input 2 (Sprite)
const int relay2IN1 = 4; // Relay 2, Input 1 (Mirinda)
const int relay2IN2 = 5; // Relay 2, Input 2 (Unused)

// Pins for TCS3200 color sensor
const int S0 = 6;
const int S1 = 7;
const int S2 = 8;
const int S3 = 11;
const int outPin = 12;

// Calibration values for each color
int blackValues[3];   // For Coca-Cola
int whiteValues[3];   // For Sprite
int orangeValues[3];  // For Mirinda

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set up the ultrasonic sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Set up the relays
  pinMode(relay1IN1, OUTPUT);
  pinMode(relay1IN2, OUTPUT);
  pinMode(relay2IN1, OUTPUT);
  pinMode(relay2IN2, OUTPUT);

  // Set up the color sensor
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(outPin, INPUT);

  // Set frequency scaling to 20%
  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);

  // Turn off all relays at the start
  // HIGH means relay off (NO contact is open)
  digitalWrite(relay1IN1, HIGH);
  digitalWrite(relay1IN2, HIGH);
  digitalWrite(relay2IN1, HIGH);
  digitalWrite(relay2IN2, HIGH);

  // Calibrate the sensor
  calibrateSensor();
}

void loop() {
  // Measure distance using the ultrasonic sensor
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) / 29.1;

  // Print ultrasonic sensor distance reading
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If the distance is less than a threshold, detect the color and dispense drink
  if (distance < 10) { // Distance threshold in cm
    Serial.println("Glass Detected");

    // Get raw color frequency data from the sensor
    int red = getColorFrequency(S2, S3, LOW, LOW);
    int green = getColorFrequency(S2, S3, HIGH, HIGH);
    int blue = getColorFrequency(S2, S3, LOW, HIGH);

    // Print raw color sensor readings
    Serial.print("Raw Data - Red: ");
    Serial.print(red);
    Serial.print(", Green: ");
    Serial.print(green);
    Serial.print(", Blue: ");
    Serial.println(blue);

    // Determine which drink to dispense based on the color
    if (isClosestColor(red, green, blue, blackValues)) {
      Serial.println("Detected: Black (Coca-Cola)");
      dispenseDrink(1);
    } else if (isClosestColor(red, green, blue, whiteValues)) {
      Serial.println("Detected: White (Sprite)");
      dispenseDrink(2);
    } else if (isClosestColor(red, green, blue, orangeValues)) {
      Serial.println("Detected: Orange (Mirinda)");
      dispenseDrink(3);
    } else {
      Serial.println("Unknown color");
    }
  } else {
    Serial.println("No Glass Detected");
  }

  delay(100);
}

void dispenseDrink(int drinkNumber) {
  switch (drinkNumber) {
    case 1: // Coca-Cola
      digitalWrite(relay1IN1, LOW);  // Turn on pump 1 (NO contact closes)
      delay(3000); // Dispense for 3 seconds
      digitalWrite(relay1IN1, HIGH); // Turn off pump 1 (NO contact opens)
      break;
    case 2: // Sprite
      digitalWrite(relay1IN2, LOW);  // Turn on pump 2 (NO contact closes)
      delay(3000); // Dispense for 3 seconds
      digitalWrite(relay1IN2, HIGH); // Turn off pump 2 (NO contact opens)
      break;
    case 3: // Mirinda
      digitalWrite(relay2IN1, LOW);  // Turn on pump 3 (NO contact closes)
      delay(3000); // Dispense for 3 seconds
      digitalWrite(relay2IN1, HIGH); // Turn off pump 3 (NO contact opens)
      break;
  }
}

int getColorFrequency(int S2, int S3, int S2State, int S3State) {
  digitalWrite(S2, S2State);
  digitalWrite(S3, S3State);
  return pulseIn(outPin, LOW);
}

void calibrateSensor() {
  Serial.println("Starting calibration...");
  
  Serial.println("Place black object (Coca-Cola) and press any key...");
  while (!Serial.available());
  Serial.read();
  readColor(blackValues);
  Serial.println("Black (Coca-Cola) calibrated.");

  Serial.println("Place white object (Sprite) and press any key...");
  while (!Serial.available());
  Serial.read();
  readColor(whiteValues);
  Serial.println("White (Sprite) calibrated.");

  Serial.println("Place orange object (Mirinda) and press any key...");
  while (!Serial.available());
  Serial.read();
  readColor(orangeValues);
  Serial.println("Orange (Mirinda) calibrated.");

  Serial.println("Calibration complete!");
  printCalibrationValues();
}

void readColor(int* colorValues) {
  colorValues[0] = getColorFrequency(S2, S3, LOW, LOW);   // Red
  colorValues[1] = getColorFrequency(S2, S3, HIGH, HIGH); // Green
  colorValues[2] = getColorFrequency(S2, S3, LOW, HIGH);  // Blue
}

void printCalibrationValues() {
  Serial.println("Calibration Values:");
  Serial.print("Black (Coca-Cola): R=");
  Serial.print(blackValues[0]);
  Serial.print(", G=");
  Serial.print(blackValues[1]);
  Serial.print(", B=");
  Serial.println(blackValues[2]);

  Serial.print("White (Sprite): R=");
  Serial.print(whiteValues[0]);
  Serial.print(", G=");
  Serial.print(whiteValues[1]);
  Serial.print(", B=");
  Serial.println(whiteValues[2]);

  Serial.print("Orange (Mirinda): R=");
  Serial.print(orangeValues[0]);
  Serial.print(", G=");
  Serial.print(orangeValues[1]);
  Serial.print(", B=");
  Serial.println(orangeValues[2]);
}

bool isClosestColor(int r, int g, int b, int* targetColor) {
  int blackDiff = colorDifference(r, g, b, blackValues);
  int whiteDiff = colorDifference(r, g, b, whiteValues);
  int orangeDiff = colorDifference(r, g, b, orangeValues);

  int minDiff = min(blackDiff, min(whiteDiff, orangeDiff));

  return (minDiff == colorDifference(r, g, b, targetColor));
}

int colorDifference(int r, int g, int b, int* targetColor) {
  return abs(r - targetColor[0]) + abs(g - targetColor[1]) + abs(b - targetColor[2]);
}
