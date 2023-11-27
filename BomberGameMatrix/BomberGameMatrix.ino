#include "LedControl.h"                                    // need the library
const byte dinPin = 12;                                    // pin 12 is connected to the MAX7219 pin 1
const byte clockPin = 11;                                  // pin 11 is connected to the CLK pin 13
const byte loadPin = 10;                                   // pin 10 is connected to LOAD pin 12
const byte matrixSize = 8;                                 // 1 as we are only using 1 MAX7219
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);  //DIN, CLK, LOAD, No. DRIVER
byte matrixBrightness = 2;
int player_x, player_y, bomb_x = -1, bomb_y = -1;
byte playerAlive;
int numberOfWalls =15;
unsigned long playerBlinkTime;
unsigned long bombBlinkTime;
byte playerLedState = false;
byte bombLedState = false;
int playerBlinkDelay = 300;
int bombBlinkDelay = 100;

byte playerMoving = true;
byte placeBomb = false;
byte bombPlaced = false;
byte detonateBomb = false;
unsigned long detonatingBombTime;
int detonatingBombDelay = 3000;
int explosionDelay = 1000;
unsigned long explosionTime = 0;
byte bombExploaded = false;


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

int xValueJoy = 0;
int yValueJoy = 0;
bool joyMoved = false;
int minThresholdJoy = 400;
int maxThresholdJoy = 600;
const int pinSW = 2;
const int pinXJoy = A0;
const int pinYJoy = A1;

const int buttonPin = 2;
byte buttonState = LOW;
byte ledState = HIGH;
byte reading = HIGH;
byte lastReading = HIGH;
unsigned int lastDebounceTime = 0;
unsigned int debounceDelay = 50;

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false);                 // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness);  // sets brightness (0~15 possible values)
  lc.clearDisplay(0);                    // clear screen
  Serial.begin(9600);
  buildWalls();
  setPlayer();
}
void loop() {
  if (playerAlive) {
    blinkPlayer();
    if (placeBomb || bombPlaced) {
      blinkBomb();
    }
    if (bombPlaced) {
      placeBomb = false;
      explodeBomb();
    }
    moveJoyStick();
    buttonDebounce();
    checkIfWinGame();
  } else {
    gameOver();
  }
}
void gameOver() {
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      lc.setLed(0, i, j, false);
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      if (i == j || i + j == 7) {
        lc.setLed(0, i, j, true);
      }
}
void checkIfWinGame() {
  int ok = false;
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
      if (matrix[i][j] == 1)
        ok = true;
  if (ok == false) {
    for (int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++)
        lc.setLed(0, i, j, true);
  }
  return true;
}
void buildWalls() {
  int x, y;
  randomSeed(analogRead(0));
  for (int i = 0; i < numberOfWalls; i++) {
    x = random(8);
    y = random(8);

    lc.setLed(0, x, y, true);  // turns on LED at col, row
    matrix[x][y] = 1;
  }
}
void setPlayer() {
  int x, y;
  randomSeed(analogRead(0));
  x = random(8);
  y = random(8);
  while (matrix[x][y] == 1) {
    randomSeed(analogRead(0));
    x = random(8);
    y = random(8);
  }
  player_x = x;
  player_y = y;
  playerAlive = true;
  playerBlinkTime = millis();
  playerLedState = true;
}
void blinkPlayer() {
  lc.setLed(0, player_x, player_y, playerLedState);
  if (millis() - playerBlinkTime > playerBlinkDelay) {
    playerLedState = !playerLedState;
    playerBlinkTime = millis();
  }
}
void blinkBomb() {
  lc.setLed(0, bomb_x, bomb_y, bombLedState);
  if (millis() - bombBlinkTime > bombBlinkDelay) {
    bombLedState = !bombLedState;
    bombBlinkTime = millis();
  }
}
void moveJoyStick() {
  xValueJoy = analogRead(pinXJoy);
  yValueJoy = analogRead(pinYJoy);
  if (yValueJoy > maxThresholdJoy && joyMoved == false) {  //JOS
    Serial.println("JOS");
    if (playerMoving) {
      if (player_x != 7 && matrix[player_x + 1][player_y] == 0) {
        lc.setLed(0, player_x, player_y, false);
        playerBlinkTime = millis();
        player_x += 1;
      }
    } else if (placeBomb) {

      lc.setLed(0, bomb_x, bomb_y, false);

      if (player_x != 7 && matrix[player_x + 1][player_y] == 0) {
        bomb_x = player_x + 1;
        bomb_y = player_y;
        bombBlinkTime = millis();
      }
    }
    joyMoved = true;
  }
  if (yValueJoy < minThresholdJoy && joyMoved == false) {  //SUS
    Serial.println("SUS");
    if (playerMoving) {
      if (player_x != 0 && matrix[player_x - 1][player_y] == 0) {
        lc.setLed(0, player_x, player_y, false);
        playerBlinkTime = millis();
        player_x -= 1;
      }
    } else if (placeBomb) {

      lc.setLed(0, bomb_x, bomb_y, false);

      if (player_x != 0 && matrix[player_x - 1][player_y] == 0) {
        bomb_x = player_x - 1;
        bomb_y = player_y;
        bombBlinkTime = millis();
      }
    }
    joyMoved = true;
  }
  if (xValueJoy > maxThresholdJoy && joyMoved == false) {  //DREAPTA
    Serial.println("DREAPTA");
    if (playerMoving) {
      if (player_y != 7 && matrix[player_x][player_y + 1] == 0) {
        lc.setLed(0, player_x, player_y, false);
        playerBlinkTime = millis();
        player_y += 1;
      }
    } else if (placeBomb) {

      lc.setLed(0, bomb_x, bomb_y, false);

      if (player_y != 7 && matrix[player_x][player_y + 1] == 0) {
        bomb_x = player_x;
        bomb_y = player_y + 1;
        bombBlinkTime = millis();
      }
    }
    joyMoved = true;
  }
  if (xValueJoy < minThresholdJoy && joyMoved == false) {  //STANGA
    Serial.println("STANGA");
    if (playerMoving) {
      if (player_y != 0 && matrix[player_x][player_y - 1] == 0) {
        lc.setLed(0, player_x, player_y, false);
        playerBlinkTime = millis();
        player_y -= 1;
      }
    } else if (placeBomb) {

      lc.setLed(0, bomb_x, bomb_y, false);

      if (bomb_x != 0 && matrix[bomb_x][bomb_y - 1] == 0) {
        bomb_x = player_x;
        bomb_y = player_y - 1;
        bombBlinkTime = millis();
      }
    }
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
        if (playerMoving) {
          Serial.print("pushed");
          playerMoving = false;
          placeBomb = true;
        } else if (placeBomb) {
          Serial.print("pushedAgain");
          placeBomb = false;
          detonatingBombTime = millis();
          bombPlaced = true;
          playerMoving = true;
        }
      }
    }
  }
  lastReading = reading;
}
void explodeBomb() {
  if (explosionTime != 0) {
    Serial.println(millis() - explosionTime);
  }
  if (!bombExploaded && millis() - detonatingBombTime > detonatingBombDelay) {
    for (int i = 0; i < 8; i++) {
      lc.setLed(0, bomb_x, i, true);
      matrix[bomb_x][i] = 0;
    }
    for (int i = 0; i < 8; i++) {
      lc.setLed(0, i, bomb_y, true);
      matrix[i][bomb_y] = 0;
    }
    explosionTime = millis();
    bombExploaded = true;
    if (bomb_x == player_x || bomb_y == player_y) {
      playerAlive = false;
    }
  } else if (bombExploaded && millis() - explosionTime > explosionDelay) {
    Serial.println(explosionTime);
    for (int i = 0; i < 8; i++) {
      lc.setLed(0, bomb_x, i, false);
    }
    for (int i = 0; i < 8; i++) {
      lc.setLed(0, i, bomb_y, false);
    }
    bombPlaced = false;
    bombExploaded = false;
    bomb_x = -1;
    bomb_y = -1;
  }
}