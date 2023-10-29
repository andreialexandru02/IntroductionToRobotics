unsigned long startTime = 0;
bool isElevatorWorking =  false;
unsigned long previousFlickeringTime;
unsigned long forDelayTime;
class FloorLed{

  public:
    int LedPin;
    bool ElevatorAtThisFloor = false;
    byte LedState =  HIGH;
    FloorLed(int ledPin){
      this->LedPin =  ledPin;
    }

    SetupInitialization(){
      pinMode(this->LedPin, OUTPUT);
    }
    MoveElevator(){
      
      unsigned long currentTime = millis();

     Serial.println(currentTime);
      if(currentTime - startTime > 6000){
        digitalWrite(this->LedPin,LOW);
        this->ElevatorAtThisFloor =  false;
      }
      else if(currentTime - startTime > 4000){
  
        digitalWrite(this->LedPin,HIGH);
      }
      else if(currentTime - startTime > 2000 ){
  
        digitalWrite(this->LedPin,LOW);

      }
    }
    Flicker(){
      unsigned long currentFlickeringTime = millis();
      if (currentFlickeringTime - previousFlickeringTime >= 250) {
      previousFlickeringTime = currentFlickeringTime; 
      if (this->LedState == LOW) {
      this->LedState = HIGH;
      } else {
      this->LedState = LOW;
      }
      digitalWrite(this->LedPin, this->LedState);
      }
    }
    
};

FloorLed floors[] = {FloorLed(8),FloorLed(9),FloorLed(10)};
FloorLed workingElevator = FloorLed(11);

int currentFloor;
void Elevator(int startingFloor, int finishFloor){
  isElevatorWorking = true;
  Serial.print("Apasat Buton:");
  Serial.println(finishFloor);
  
  if(startingFloor < finishFloor){
    for(int i=startingFloor+1; i<=finishFloor; i+=1){
        floors[i].ElevatorAtThisFloor =  true;
    }
  }
  else if(startingFloor > finishFloor){
     for(int i=startingFloor-1; i>=finishFloor; i-=1){
        floors[i].ElevatorAtThisFloor =  true;
       
    }
  }
  currentFloor = finishFloor;
//  digitalWrite(floors[currentFloor].LedPin,HIGH);
  isElevatorWorking =  false;
};
class Button{
  public:
    int ButtonPin;
    int ButtonNumber;
    byte ButtonState = LOW;
    byte Reading = LOW;
    byte LastReading =  LOW;
    unsigned int LastDebounceTime = 0;
    unsigned int DebounceDelay = 50;
    
    Button(int buttonPin,int buttonNumber){
      this->ButtonPin =  buttonPin;
      this->ButtonNumber = buttonNumber;
    }
    SetupInitialization(){
      pinMode(this->ButtonPin, INPUT_PULLUP);
    }
    Debounce(){
     
      this->Reading = digitalRead(this->ButtonPin);
      if (this->Reading != this->LastReading) {
        this->LastDebounceTime = millis();
      }
      if ((millis() - this->LastDebounceTime) > this->DebounceDelay && LastDebounceTime != 0) {
        if (this->Reading != this->ButtonState) {
          this->ButtonState = this->Reading;
          if (this->ButtonState == HIGH) {            
            Elevator(currentFloor, this->ButtonNumber);
            startTime = millis();
            previousFlickeringTime = millis();
            forDelayTime = millis();
          }
        }
      }
      this->LastReading = this->Reading;
    }

};

Button buttons[] = {Button(2,0),Button(3,1),Button(4,2)};



//}
void setup() {
  
  Serial.begin(9600);
  currentFloor = 0;
  buttons[0].SetupInitialization();
  buttons[1].SetupInitialization();
  buttons[2].SetupInitialization();
  floors[0].SetupInitialization();
  floors[1].SetupInitialization();
  floors[2].SetupInitialization();
  //digitalWrite(floors[currentFloor].LedPin,HIGH);
}
void loop() {

   buttons[0].Debounce();
   buttons[1].Debounce();
   buttons[2].Debounce();
   Serial.println(currentFloor);
  if(floors[0].ElevatorAtThisFloor)  {floors[0].MoveElevator();}
  if(floors[1].ElevatorAtThisFloor) {floors[1].MoveElevator(); }
  if(floors[2].ElevatorAtThisFloor) {floors[2].MoveElevator();}
  if(isElevatorWorking) {workingElevator.Flicker();}

};
