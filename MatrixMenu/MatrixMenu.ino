
#include <EEPROM.h>
#include <LiquidCrystal.h>
#include "LedControl.h"
const byte dinPin = 12;
const byte clockPin = 11;
const byte loadPin = 10;
const byte matrixSize = 8;
byte isTextScrolling = false;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
byte matrix[matrixSize][matrixSize] = {
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0 }
};
byte isGamePlaying = false;
const int rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char* menuOptions[] = { "Start Game", "Settings" };
char* settingsMenuOptions[] = { "LCD Brightness", "Mtrx Brightness", "About", "Back" };
char* aboutOptions[] = { "Andrei Alexandru", "Crossy Roads", "https://github.com/andreialexandru02/IntroductionToRobotics " };
byte isInSettings = false;
byte isInLCDBrightness = false;
byte isInMatrixBrightness = false;
byte isGameEnded = false;
byte isInAboutSection = false;
int numMenuOptions = 2;
int selectedMenuOption = 0;

//JoyStick
int xValueJoy = 0;
int yValueJoy = 0;
bool joyMoved = false;
int minThresholdJoy = 400;
int maxThresholdJoy = 600;
const int pinXJoy = A0;
const int pinYJoy = A1;
//Button
const int buttonPin = 2;
byte buttonState = LOW;
byte reading = HIGH;
byte lastReading = HIGH;
unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;
int LCD_Backlight = 3;
int LCDBrightnessMermorySlot = 0;
int MatrixBrightnessMermorySlot = 2;
unsigned long timerStartTime = 0;
unsigned long startingScollingTime = 0;
int scrollingDelay  = 200;
int timer;
int cursor;
byte fullBlock[8] = {
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111
};

void setup() {
  lc.shutdown(0, false);
  int matrixBrightness = EEPROM.get(MatrixBrightnessMermorySlot, matrixBrightness);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(LCD_Backlight, OUTPUT);
  int brightnessLvl = EEPROM.get(LCDBrightnessMermorySlot, brightnessLvl);
  analogWrite(LCD_Backlight, brightnessLvl);
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO");
  lcd.setCursor(0, 1);
  lcd.print("CROSSY ROADS");
  delay(1000);
  lcd.clear();
  lcd.print(menuOptions[selectedMenuOption]);
  lcd.createChar(0, fullBlock);
}
void loop() {
  if (!isGamePlaying) {
    moveJoyStick();
    if (isTextScrolling) {
      scrollText();
    }
  } else {
    displayTimer();
  }
  buttonDebounce();
}

void moveJoyStick() {
  xValueJoy = analogRead(pinXJoy);
  yValueJoy = analogRead(pinYJoy);
  if (yValueJoy > maxThresholdJoy && joyMoved == false) {  //JOS
    Serial.println("JOS");
    Serial.println(numMenuOptions);
    Serial.println(selectedMenuOption);
    selectedMenuOption = (selectedMenuOption + 1) % numMenuOptions;
    displayMenu();
    joyMoved = true;
  }
  if (yValueJoy < minThresholdJoy && joyMoved == false) {  //SUS
    Serial.println("SUS");
    Serial.println(numMenuOptions);
    Serial.println(selectedMenuOption);
    selectedMenuOption = (selectedMenuOption - 1 + numMenuOptions) % numMenuOptions;
    displayMenu();
    joyMoved = true;
  }
  if (xValueJoy > maxThresholdJoy && joyMoved == false) {  //DREAPTA
    Serial.println("DREAPTA");
    if (isInLCDBrightness && selectedMenuOption < 4) {
      selectedMenuOption = selectedMenuOption + 1;
      analogWrite(LCD_Backlight, 110 - selectedMenuOption * 10);
      Serial.println(110 - selectedMenuOption * 10);
    } else if (isInMatrixBrightness && selectedMenuOption < 4) {
      selectedMenuOption = selectedMenuOption + 1;
      lc.setIntensity(0, (selectedMenuOption + 1) * 3);
    }
    displayMenu();
    joyMoved = true;
  }
  if (xValueJoy < minThresholdJoy && joyMoved == false) {  //STANGA
    Serial.println("STANGA");
    if (isInLCDBrightness && selectedMenuOption > 0) {
      selectedMenuOption = selectedMenuOption - 1;
      analogWrite(LCD_Backlight, 110 - selectedMenuOption * 10);
      Serial.println(110 - selectedMenuOption * 10);
    } else if (isInMatrixBrightness && selectedMenuOption > 0) {
      selectedMenuOption = selectedMenuOption - 1;
      lc.setIntensity(0, (selectedMenuOption + 1) * 3);
    }
    displayMenu();
    joyMoved = true;
  }

  if (xValueJoy >= minThresholdJoy && xValueJoy <= maxThresholdJoy && yValueJoy >= minThresholdJoy && yValueJoy <= maxThresholdJoy) {
    joyMoved = false;
  }
}
void buttonDebounce() {
  reading = digitalRead(buttonPin);
  if (reading != lastReading) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        if (!isInAboutSection && !isGameEnded && !isGamePlaying && !isInSettings && !isInLCDBrightness && !isInMatrixBrightness && selectedMenuOption == 0) {
          isGamePlaying = true;
          timerStartTime = millis();
          timer = 0;
        } else if (isGamePlaying) {
          isGamePlaying = false;
          isGameEnded = true;
          displayMenu();
        } else if (isGameEnded) {
          isGameEnded = false;
          displayMenu();
        } else if (!isInSettings && selectedMenuOption == 1) {
          isInSettings = true;
          selectedMenuOption = 0;
          numMenuOptions = 4;
          displayMenu();
        } else if (isInSettings && selectedMenuOption == 3) {
          isInSettings = false;
          selectedMenuOption = 0;
          numMenuOptions = 2;
          displayMenu();
        } else if (isInSettings && !isInLCDBrightness && selectedMenuOption == 0) {
          isInLCDBrightness = true;
          isInSettings = false;
          int brightnessLvl = EEPROM.get(LCDBrightnessMermorySlot, brightnessLvl);
          selectedMenuOption = (110 - brightnessLvl) / 10;
          displayMenu();
        } else if (isInSettings && !isInAboutSection && selectedMenuOption == 2) {
          isInAboutSection = true;
          isInSettings = false;
          numMenuOptions = 3;
          selectedMenuOption = 0;
          displayMenu();
        } else if (isInAboutSection) {
          isInAboutSection = false;
          isTextScrolling = false;
          isInSettings = true;
          selectedMenuOption = 0;
          numMenuOptions = 4;
          displayMenu();

        } else if (isInLCDBrightness) {
          isInLCDBrightness = false;
          isInSettings = true;
          EEPROM.put(LCDBrightnessMermorySlot, 110 - selectedMenuOption * 10);
          selectedMenuOption = 0;
          displayMenu();
        } else if (isInSettings && !isInMatrixBrightness && selectedMenuOption == 1) {
          isInMatrixBrightness = true;
          isInSettings = false;
          selectedMenuOption = 0;
          turnOnMatrix();
          displayMenu();
        } else if (isInMatrixBrightness) {
          isInMatrixBrightness = false;
          isInSettings = true;
          EEPROM.put(MatrixBrightnessMermorySlot, (selectedMenuOption + 1) * 3);
          selectedMenuOption = 0;
          displayMenu();
        }
      }
    }
  }
  lastReading = reading;
}
void displayMenu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  if (isInSettings) {
    lcd.print(settingsMenuOptions[selectedMenuOption]);
  } else if (isInLCDBrightness || isInMatrixBrightness) {
    lcd.clear();
    showBrightnessLvl(selectedMenuOption + 1);
  } else if (isGameEnded) {
    lcd.print("GameOver!");
  } else if (isInAboutSection) {
    if (strlen(aboutOptions[selectedMenuOption]) > 16) {
      cursor = 0;
      startingScollingTime = millis();
      isTextScrolling = true;
    } else {
      isTextScrolling = false;
      lcd.clear();
      lcd.print(aboutOptions[selectedMenuOption]);
    }
  } else {
    lcd.print(menuOptions[selectedMenuOption]);
  }
}
void showBrightnessLvl(int level) {
  lcd.clear();
  for (int i = 0; i < level; i++) {
    lcd.setCursor(i, 0);
    lcd.write(byte(0));
  }
}
void turnOnMatrix() {
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      lc.setLed(0, i, j, true);
}
void displayTimer() {
  if (millis() - timerStartTime > 1000) {
    lcd.clear();
    lcd.setCursor(0, 0);
    timer++;
    lcd.print(timer);
    timerStartTime = millis();
  }
}
void scrollText() {
  int textLength = strlen(aboutOptions[selectedMenuOption]);

  if (millis() - startingScollingTime > scrollingDelay) {


    if (cursor == (textLength - 1)) {
      cursor = 0;
    }
    lcd.setCursor(0, 0);
    if (cursor < textLength - 16) {
      for (int i = cursor; i < cursor + 16; i++) {
        lcd.print(aboutOptions[selectedMenuOption][i]);
      }
    } else {
      for (int i = cursor; i < (textLength); i++) {
        lcd.print(aboutOptions[selectedMenuOption][i]);
      }
      for (int i = 0; i <= 16 - (textLength - cursor); i++) {
        lcd.print(aboutOptions[selectedMenuOption][i]);
      }
    }
    cursor++;
    startingScollingTime = millis();
  }
}
