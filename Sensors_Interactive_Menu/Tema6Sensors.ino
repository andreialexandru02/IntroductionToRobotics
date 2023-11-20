#include <EEPROM.h>
const int trigPin = 9;
const int echoPin = 10;
long duration = 0;
int distance = 0;
int photocellPin = 0;
int photocellValue;
int choice = 0;
int samplingRateMemorySlot = 0;
int ultrasoundThresholdMemorySlot = 2;
int LRDThresholdMemorySlot = 4;

int ultrasoundLogMemorySlotStart = 6;
int ultrasoundLogMemorySlotCurrent = 6;  //6 8 10 ...24
int ultrasoundLogMemorySlotMAX = 24;
int LRDLogMemorySlotStart = 26;
int LRDLogMemorySlotCurrent = 26;  //26; 26 28 ...44
int LRDLogMemorySlotMAX = 44;

const int bluePin = 4;
const int redPin = 2;
const int greenPin = 3;
const int automaticModeMemorySlot = 46;
bool automaticMode = EEPROM.get(46, automaticMode);

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);

  printMainMenu();
}
void loop() {

  MainMenu();
}

void MainMenu() {
  int choice = 0;

  while (!Serial.available()) {
    // Wait for user input
  }

  choice = Serial.parseInt();

  switch (choice) {
    case 1:
      sensorSettingsMenu();
      break;
    case 2:
      resetLoggerDataMenu();
      break;
    case 3:
      systemStatusMenu();
      break;
    case 4:
      rgbLedControlMenu();
      break;
    default:
      Serial.println("Invalid choice. Please try again.");
  }
}
void printMainMenu() {
  Serial.println("Main Menu:");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control");
}
void sensorSettingsMenu() {
  int subChoice = 0;

  while (subChoice != 4) {
    Serial.println("1. Sensor Settings // Go to submenu");
    Serial.println("    1.1 Sensors Sampling Interval");
    Serial.println("    1.2 Ultrasonic Alert Threshold");
    Serial.println("    1.3 LDR Alert Threshold");
    Serial.println("    1.4 Back // Return to main menu");

    while (!Serial.available()) {
      // Wait for user input
    }

    subChoice = Serial.parseInt();

    switch (subChoice) {
      case 1:
        Serial.println("1.1");
        Serial.println();
        chooseSamplingRate();
        break;
      case 2:
        Serial.println("1.2");
        Serial.println();
        chooseUltraSonicAlertThreshold();
        break;
      case 3:
        Serial.println("1.3");
        Serial.println();
        chooseLRDThreshold();
        break;
      case 4:
        printMainMenu();
        MainMenu();
        break;
      default:
        Serial.println("Invalid choice. Please try again.");
    }
  }
}

void resetLoggerDataMenu() {
  int subChoice = 0;

  while (subChoice != 2) {
    Serial.println("2. Reset Logger Data. Are you sure?");
    Serial.println("    2.1 Yes");
    Serial.println("    2.2 No");

    while (!Serial.available()) {
      // Wait for user input
    }

    subChoice = Serial.parseInt();

    switch (subChoice) {
      case 1:
        EEPROM.put(samplingRateMemorySlot, 0);
        EEPROM.put(ultrasoundThresholdMemorySlot, 0);
        EEPROM.put(LRDThresholdMemorySlot, 0);
        printMainMenu();
        MainMenu();
        break;
      case 2:
        printMainMenu();
        MainMenu();
        break;
      default:
        Serial.println("Invalid choice. Please try again.");
    }
  }
}

void systemStatusMenu() {
  int subChoice = 0;

  while (subChoice != 4) {
    Serial.println("3. System Status // Check current status and health");
    Serial.println("    3.1 Current ultraSonic sensor Readings");
    Serial.println("    3.2 Current LRD sensor Readings");
    Serial.println("    3.3 Current Sensor Settings");
    Serial.println("    3.4 Display Logged Data");
    Serial.println("    3.5 Back // Return to main menu");

    while (!Serial.available()) {
      // Wait for user input
    }

    subChoice = Serial.parseInt();

    switch (subChoice) {
      case 1:

        Serial.println("Press any key to stop continuous reading");

        while (!Serial.available()) {
          readUltrasonicSensor();
         
        }
        break;
      case 2:

        Serial.println("Press any key to stop continuous reading");

        while (!Serial.available()) {
          
          readLDR();
        }
        break;
      case 3:
        printSensorSettings();
        break;
      case 4:
        printUltrasoundLog();
        printLRDLog();
        break;
      case 5:
        printMainMenu();
        MainMenu();
        break;
      default:
        Serial.println("Invalid choice. Please try again.");
    }
  }
}

void rgbLedControlMenu() {
  int subChoice = 0;

  while (subChoice != 3) {
    Serial.println("4. RGB LED Control // Go to submenu");
    Serial.println("    4.1 Manual Color Control");
    Serial.println("    4.2 LED: Toggle Automatic ON/OFF");
    Serial.println("    4.3 Back // Return to main menu");

    while (!Serial.available()) {
      // Wait for user input
    }

    subChoice = Serial.parseInt();

    switch (subChoice) {
      case 1:
        turnOnRGBMenu();
        break;
      case 2:
        toggleAutomaticMode();
        break;
      case 3:
        printMainMenu();
        MainMenu();
        break;
      default:
        Serial.println("Invalid choice. Please try again.");
    }
  }
}
void chooseSamplingRate() {
  int samplingRate;
  Serial.println("Choose a sampling rate between 1 and 10 (natural numbers):");
  while (!Serial.available()) {
  }
  choice = Serial.parseInt();
  switch (choice) {
    case 1:
      samplingRate = 1000;  // 1 sample per second
      Serial.print("Sampling rate set at ");
      Serial.println(samplingRate);
      Serial.println();
      break;
    case 2:
      samplingRate = 500;  // 2 samples per second
      Serial.print("Sampling rate set at ");
      Serial.println(samplingRate);
      Serial.println();
      break;
    case 3:
      samplingRate = 333;  // 3 samples per second
      Serial.print("Sampling rate set at ");
      Serial.println(samplingRate);
      Serial.println();
      break;
    case 4:
      samplingRate = 250;  // 4 samples per second
      Serial.print("Sampling rate set at ");
      Serial.println(samplingRate);
      Serial.println();
      break;
    case 5:
      samplingRate = 200;  // 5 samples per second
      Serial.print("Sampling rate set at ");
      Serial.println(samplingRate);
      Serial.println();
      break;
    case 6:
      samplingRate = 166;  // 6 samples per second
      Serial.print("Sampling rate set at ");
      Serial.println(samplingRate);
      Serial.println();
      break;
    case 7:
      samplingRate = 142;  // 7 samples per second
      Serial.print("Sampling rate set at ");
      Serial.println(samplingRate);
      Serial.println();
      break;
    case 8:
      samplingRate = 125;  // 8 samples per second
      Serial.print("Sampling rate set at ");
      Serial.println(samplingRate);
      Serial.println();
      break;
    case 9:
      samplingRate = 111;  // 9 samples per second
      Serial.print("Sampling rate set at ");
      Serial.println(samplingRate);
      Serial.println();
      break;
    case 10:
      samplingRate = 100;  // 10 samples per second
      Serial.print("Sampling rate set at ");
      Serial.println(samplingRate);
      Serial.println();
      break;
    default:
      Serial.println("Invalid choice. Please enter a number between 1 and 10.");
      break;
  }
    EEPROM.put(samplingRateMemorySlot, samplingRate);  //samplingRate is on address 0
}
void chooseUltraSonicAlertThreshold() {
  Serial.println("Choose a min threshold for the ultrasonic sensor");


  while (!Serial.available()) {
  }
  int threshold = Serial.parseInt();

  EEPROM.put(ultrasoundThresholdMemorySlot, threshold);  //ltrasonic sensor threshold is on addres 1;
  Serial.print("Ultrasonic sensor threshold set at ");
  Serial.println(threshold);
  Serial.println();
}
void chooseLRDThreshold() {
  Serial.println("Choose a min threshold for the LRD sensor");


  while (!Serial.available()) {
  }
  int threshold = Serial.parseInt();

  EEPROM.put(LRDThresholdMemorySlot, threshold);  //LRD threshold is on addres 1;
  Serial.print("LRD threshold set at ");
  Serial.println(threshold);
  Serial.println();
}
void readUltrasonicSensor() {

  int samplingRate;
  EEPROM.get(samplingRateMemorySlot, samplingRate);
  int threshold;
  EEPROM.get(ultrasoundThresholdMemorySlot, threshold);

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  if (distance < threshold) {
    Serial.println("Too close");
    if (automaticMode) {
      turnOnRed();
    }
  } else {
    if (automaticMode) {
      turnOnGreen();
    }
    Serial.print("Distance: ");
    Serial.println(distance);
    if (ultrasoundLogMemorySlotCurrent > ultrasoundLogMemorySlotMAX) {
      ultrasoundLogMemorySlotCurrent = ultrasoundLogMemorySlotStart;
    }
    Serial.print(ultrasoundLogMemorySlotCurrent);
    Serial.println();
    EEPROM.put(ultrasoundLogMemorySlotCurrent, distance);
    ultrasoundLogMemorySlotCurrent += 2;
  }
  delay(500);
}
void readLDR() {
  int samplingRate;
  EEPROM.get(samplingRateMemorySlot, samplingRate);
  photocellValue = analogRead(photocellPin);
  int threshold;
  EEPROM.get(LRDThresholdMemorySlot, threshold);


  if (photocellValue < threshold) {
    Serial.println("Too dim");
    if (automaticMode) {
      turnOnRed();
    }
  } else {
    if (automaticMode) {
      turnOnGreen();
    }
    Serial.print("LDR = ");
    Serial.println(photocellValue);  // the raw analog reading
    if (LRDLogMemorySlotCurrent > LRDLogMemorySlotMAX) {
      LRDLogMemorySlotCurrent = LRDLogMemorySlotStart;
    }
    EEPROM.put(LRDLogMemorySlotCurrent, photocellValue);
    LRDLogMemorySlotCurrent += 2;
  }
  delay(500);
}
void printUltrasoundLog() {

  Serial.println("Ultrasound:");
  for (int i = ultrasoundLogMemorySlotStart; i <= ultrasoundLogMemorySlotMAX; i += 2) {

    EEPROM.get(i, distance);
    Serial.println(distance);
  }
}
void printLRDLog() {

  Serial.println("LRD:");
  for (int i = LRDLogMemorySlotStart; i <= LRDLogMemorySlotMAX; i += 2) {

    EEPROM.get(i, photocellValue);

    Serial.println(photocellValue);
  }
}
void printSensorSettings() {
  int samplingRate = EEPROM.get(samplingRateMemorySlot, samplingRate);
  int ultrasoundThreshold = EEPROM.get(ultrasoundThresholdMemorySlot, ultrasoundThreshold);
  int LRDThreshold = EEPROM.get(LRDThresholdMemorySlot, LRDThreshold);
  Serial.print("Sampling rate: ");
  Serial.println(samplingRate);
  Serial.print("Ultrasound threshold:  ");
  Serial.println(ultrasoundThreshold);
  Serial.print("LRD threshold:  ");
  Serial.println(LRDThreshold);
}
//LED
void turnOnRed() {
  digitalWrite(redPin, HIGH);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}

void turnOnGreen() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH);
  digitalWrite(bluePin, LOW);
}

void turnOnBlue() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, HIGH);
}

void turnOffAll() {
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}
void turnOnRGBMenu() {
  Serial.println("        4.1.1 Turn on Red");
  Serial.println("        4.1.2 Turn on Green");
  Serial.println("        4.1.3 Turn on Blue");
  Serial.println("        4.1.4 Turn off All");
  Serial.println("        4.1.5 Back // Return to main menu");
  int subChoice;
  while (subChoice != 5) {



    while (!Serial.available()) {
      // Wait for user input
    }

    int subChoice = Serial.parseInt();

    switch (subChoice) {
      case 1:
        turnOnRed();
        break;
      case 2:
        turnOnGreen();
        break;
      case 3:
        turnOnBlue();
        break;
      case 4:
        turnOffAll();
        break;
      case 5:
        rgbLedControlMenu();
        break;
      default:
        Serial.println("Invalid choice. Please try again.");
    }
  }
}
void toggleAutomaticMode() {
  automaticMode = !automaticMode;
  EEPROM.put(automaticModeMemorySlot, automaticMode);
  if (automaticMode) {
    Serial.println("Automatic Mode ON");
  } else {
    Serial.println("Automatic Mode OFF");
  }
}