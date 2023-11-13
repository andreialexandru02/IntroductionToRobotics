//BUTTONS
const int startButton = 2;
const int resetButton = 3;
const int lapButton = 8;
byte startButtonState = LOW;
byte resetButtonState = LOW;
byte lapButtonState = HIGH;
byte resetReading = LOW;
byte resetLastReading = LOW;
byte startReading = LOW;
byte startLastReading = LOW;
byte lapReading = LOW;
byte lapLastReading = LOW;
unsigned int resetLastDebounceTime = 0;
unsigned int startLastDebounceTime = 0;
unsigned int lapLastDebounceTime = 0;
unsigned int debounceDelay = 50;
//DISPLAY


// Define connections to the shift register
const int latchPin = 11;  // Connects to STCP (latch pin) on the shift register
const int clockPin = 10;  // Connects to SHCP (clock pin) on the shift register
const int dataPin = 12;   // Connects to DS (data pin) on the shift register
// Define connections to the digit control pins for a 4-digit display
const int segD1 = 4;
const int segD2 = 5;
const int segD3 = 6;
const int segD4 = 7;
// Store the digits in an array for easy access
int displayDigits[] = { segD1, segD2, segD3, segD4 };
const int displayCount = 4;  // Number of digits in the display
// Define the number of unique encodings (0-9, A-F for hexadecimal)
const int encodingsNumber = 10;
// Define byte encodings for the hexadecimal characters 0-F
byte byteEncodings[encodingsNumber] = {
  //A B C D E F G DP
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110,  // 9
};
unsigned long lastIncrement = 0;
unsigned long delayCount = 100;  // Delay between updates (milliseconds)
unsigned long number = 0;        // The number being displayed
String timerState = "reseted";
const int decimalPosition = 2;
int numberOfLaps = 4;
int savedLaps[4];
int currentSavingLap = 0;
int currentViewingLap = 0;
unsigned long lapButtonPressedTime;
int holdDuration = 2000;
byte lapButtonHolded = false;
unsigned long cycleTime = 0;
int cycleDelay = 500;
void setup() {


  pinMode(startButton, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(lapButton, INPUT_PULLUP);
  Serial.begin(9600);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  // Initialize digit control pins and set them to LOW (off)
  for (int i = 0; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }
  resetLaps();
}

void loop() {
 // Serial.println(timerState);
  debounceButtons();
  if (timerState == "counting") {
    if (millis() - lastIncrement > delayCount) {
      number++;
      number %= 10000;  // Wrap around after 9999
      lastIncrement = millis();
    }
  }
  writeNumber(number);
  if (timerState == "lap") {
    viewLaps();
  }
  if (lapButtonHolded) {
    cycleLaps();
  }
}
void cycleLaps() {
  
  viewLaps();
  
  if((millis() - cycleTime) > cycleDelay){
    currentViewingLap++;
    currentViewingLap %= 4;
    cycleTime = millis();
  }  
}
void resetLaps() {
  for (int i = 0; i < numberOfLaps; i++)
    savedLaps[i] = 0;
}
void debounceButtons() {
  startDebounce();
  resetDebounce();
  lapDebounce();
}
void resetDebounce() {
  resetReading = !digitalRead(resetButton);
  if (resetReading != resetLastReading) {
    resetLastDebounceTime = millis();
  }
  if ((millis() - resetLastDebounceTime) > debounceDelay && resetLastDebounceTime != 0) {
    if (resetReading != resetButtonState) {
      resetButtonState = resetReading;
      if (resetButtonState == HIGH) {
        if (timerState == "paused") {
          //reset timer
          Serial.println("reset");
          number = 0;
          timerState = "reseted";
        } else if (timerState == "lap") {
          // reset laps
          number = 0;
          resetLaps();
        }
      }
    }
  }
  resetLastReading = resetReading;
}

void startDebounce() {
  startReading = !digitalRead(startButton);


  if (startReading != startLastReading) {
    startLastDebounceTime = millis();
  }
  if ((millis() - startLastDebounceTime) > debounceDelay && startLastDebounceTime != 0) {
    if (startReading != startButtonState) {
      startButtonState = startReading;
      if (startButtonState == HIGH) {
        Serial.println("start");
        if (timerState == "counting") {
          timerState = "paused";
        } else {
          timerState = "counting";
        }
      }
    }
  }
  startLastReading = startReading;
}
void lapDebounce() {
  lapReading = !digitalRead(lapButton);
 
  if (lapReading != lapLastReading) {
    lapLastDebounceTime = millis();
  }
  if ((millis() - lapLastDebounceTime) > debounceDelay && lapLastDebounceTime != 0) {
    if (lapReading != lapButtonState) {
      lapButtonState = lapReading;
      if (lapButtonState == HIGH) {
        lapButtonPressedTime = millis();
        Serial.println("lap");
        if (timerState == "counting") {
          saveLap();
        } else if (timerState == "reseted") {
          timerState = "lap";
        } else if (timerState == "lap") {
          currentViewingLap++;
          currentViewingLap %= 4;
        }
      }
    }
  }
  // Serial.println(lapButtonPressedTime);
  if (lapButtonState == HIGH && timerState == "lap" && (millis() - lapButtonPressedTime) > holdDuration) {
    
    lapButtonHolded = true;
  }
  else{
    lapButtonHolded = false;
  }
  lapLastReading = lapReading;
}
void saveLap() {

  Serial.println("saved LAP");
  savedLaps[currentSavingLap] = number;
  currentSavingLap++;
  currentSavingLap %= 4;
}
void viewLaps() {
  number = savedLaps[currentViewingLap];
}
void writeReg(int digit) {
  // Prepare to shift data by setting the latch pin low
  digitalWrite(latchPin, LOW);
  // Shift out the byte representing the current digit to the shift register
  shiftOut(dataPin, clockPin, MSBFIRST, digit);
  // Latch the data onto the output pins by setting the latch pin high
  digitalWrite(latchPin, HIGH);
}
void activateDisplay(int displayNumber) {
  // Turn off all digit control pins to avoid ghosting
  for (int i = 0; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);
  }
  // Turn on the current digit control pin
  digitalWrite(displayDigits[displayNumber], LOW);
}

void createListFromNumber(int number, int list[]) {
  int currentDigit = 3;
  do {
    list[currentDigit] = number % 10;
    number /= 10;
    currentDigit--;
  } while (number != 0);
}
void writeNumber(int number) {
  // DONE: Initialize necessary variables for tracking the current number and digit position
  int currentNumberList[displayCount] = { 0, 0, 0, 0 };

  createListFromNumber(number, currentNumberList);
  int displayDigit = 3;  // Start with the least significant digit
  int lastDigit = 0;


  for (int i = displayDigit; i >= 0; i--) {
    activateDisplay(i);
    if (i == decimalPosition) {
      writeReg(byteEncodings[currentNumberList[i]] + 1);
    } else {
      writeReg(byteEncodings[currentNumberList[i]]);
    }
    writeReg(B00000000);
  }
}
