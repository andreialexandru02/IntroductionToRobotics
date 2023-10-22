 const int RedInputPin = A0;
const int GreenInputPin = A1;
const int BlueInputPin = A2;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

int mappedRedValue;   
int mappedBlueValue;   
int mappedGreenValue;   



const int blueLedPin = 9;
const int redLedPin = 11;
const int greenLedPin = 10;

void setup() {
  Serial.begin(9600);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(blueLedPin, OUTPUT);

}

void loop() {
    redValue = analogRead(RedInputPin);
    greenValue = analogRead(GreenInputPin);
    blueValue = analogRead(BlueInputPin);

    mappedRedValue = map(redValue, 0, 1023, 0, 255);
    mappedBlueValue = map(blueValue, 0, 1023, 0, 255);
    mappedGreenValue = map(greenValue, 0, 1023, 0, 255);

    analogWrite(redLedPin,mappedRedValue);
    analogWrite(greenLedPin,mappedGreenValue);
    analogWrite(blueLedPin,mappedBlueValue);
    delay(100);
    
}
