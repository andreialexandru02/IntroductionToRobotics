// declare all the joystick pins

const int pinSW = 2;  // digital pin connected to switch output
const int pinX = A0;  // A0 - analog pin connected to X output
const int pinY = A1;  // A1 - analog pin connected to Y output
// declare all the segments pins
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;
const int segSize = 8;
int index = 0;
// modify if you have common anode
bool commonAnode = false;
const int noOfDigits = 10;
byte state = HIGH;
byte dpState = LOW;
byte swState = LOW;
byte lastSwState = LOW;
int xValue = 0;
int yValue = 0;
bool joyMoved = false;
int digit = 0;
int minThreshold = 400;
int maxThreshold = 600;
int currentLed;
bool currentLedState;
int currentLedIndex;
unsigned long swPressedTime;
int swHoldTime = 2000;
bool swHoldState = false;
bool swPressState = false;
unsigned long previousBlinkingTime;
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
int segmentsStates[segSize] = {
  LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};
void setup() {
  Serial.begin(9600);

  // initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }
  pinMode(pinSW, INPUT_PULLUP);
  if (commonAnode == true) {
    state = !state;
  }
  currentLed = 4;
  currentLedState = LOW;
  previousBlinkingTime = 0;
}
void loop() {

  for (int i = 0; i < segSize; i++) {
    if (segments[i] != currentLed) {
      digitalWrite(segments[i], segmentsStates[i]);
    }
  }
  currentLedIndex = findCurrentLedIndex(currentLed);
  blinkCurrentLed();
  swMoveMent();
  swPress();
}
void swPress(){
  swState = digitalRead(pinSW) ^ state;
  if (swState != lastSwState) {
    lastSwState = swState;
    swPressedTime = millis();
    if (swState == HIGH) {
      swPressState = true;
    }
  }
  swHoldState = false;
  if (swState == HIGH && millis() - swPressedTime >= swHoldTime) {
    swHoldState = true;
    Serial.println("HELD");
  }
  if (swPressState) {
    segmentsStates[currentLedIndex] = !segmentsStates[currentLedIndex];
    Serial.println("Pressed");
    swPressState = false;
  }
  if (swHoldState) {
    resetSegments();
    swHoldState = false;
  }
}
void swMoveMent(){
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  if (yValue > maxThreshold && joyMoved == false) {  //JOS
    Serial.println("JOS");
    digitalWrite(currentLed, LOW);
    if (currentLed == pinA) {
      currentLed = pinG;
    }
    if (currentLed == pinB) {
      currentLed = pinG;
    }
    if (currentLed == pinC) {
      currentLed = pinD;
    }
    if (currentLed == pinE) {
      currentLed = pinD;
    }
    if (currentLed == pinF) {
      currentLed = pinG;
    }
    if (currentLed == pinG) {
      currentLed = pinD;
    }
    joyMoved = true;
  }
  if (yValue < minThreshold && joyMoved == false) {  //SUS
    Serial.println("SUS");
    digitalWrite(currentLed, LOW);
    if (currentLed == pinG) {
      currentLed = pinA;
    }
    if (currentLed == pinF) {
      currentLed = pinA;
    }
    if (currentLed == pinE) {
      currentLed = pinG;
    }
    if (currentLed == pinD) {
      currentLed = pinG;
    }
    if (currentLed == pinC) {
      currentLed = pinG;
    }
    if (currentLed == pinB) {
      currentLed = pinA;
    }
    joyMoved = true;
  }
  if (xValue > maxThreshold && joyMoved == false) {  //DREAPTA
    Serial.println("DREAPTA");
    digitalWrite(currentLed, LOW);
    if (currentLed == pinG) {
      currentLed = pinB;
    }
    if (currentLed == pinC) {
      currentLed = pinDP;
    }
    if (currentLed == pinD) {
      currentLed = pinC;
    }
    if (currentLed == pinE) {
      currentLed = pinC;
    }
    if (currentLed == pinF) {
      currentLed = pinG;
    }
    joyMoved = true;
  }
  if (xValue < minThreshold && joyMoved == false) {  //STANGA
    Serial.println("STANGA");
    digitalWrite(currentLed, LOW);

    if (currentLed == pinA) {
      currentLed = pinF;
    }
    if (currentLed == pinB) {
      currentLed = pinF;
    }
    if (currentLed == pinC) {
      currentLed = pinE;
    }
    if (currentLed == pinE) {
      currentLed = pinE;
    }
    if (currentLed == pinDP) {
      currentLed = pinC;
    }
    joyMoved = true;
  }
  if (xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }
}

void blinkCurrentLed() {
  unsigned long currentBlinkingTime = millis();
  if (currentBlinkingTime - previousBlinkingTime >= 500) {
    previousBlinkingTime = currentBlinkingTime;
    currentLedState = digitalRead(currentLed);
    digitalWrite(currentLed, !currentLedState);
  }
}
int findCurrentLedIndex(int currentLed) {
  for (int i = 0; i < segSize; i++) {
    if (segments[i] == currentLed)
      return i;
  }
}
void resetSegments() {
  currentLed = pinDP;
  for (int i = 0; i < segSize; i++) {
    segmentsStates[i] = LOW;
  }
}
