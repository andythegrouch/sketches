// This an arduino sketch that gives 15m pulse from a RC ON/OFF switch that also resets if the battery voltage
// drops below 10.0V
//#define DEBUG 1

#define STANDBY 1
#define POWER   2
#define TIMEOUT 3
//change for commit test
//another change for commit

unsigned int state = STANDBY;
unsigned int ledPin = 13;
unsigned int duration = 900000; //ms
unsigned int buttonPin = 2;
unsigned int rxSignal = 0;
unsigned int powerLevel =0;
unsigned long startTime =millis();
float batteryVoltage;
float batteryVoltageThreshold = 11.5;
 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {


  rxSignal=!digitalRead(buttonPin);
  batteryVoltage = 12.0*analogRead(A0)/1024.0;
  digitalWrite(ledPin, powerLevel);

#ifdef DEBUG  
  Serial.print("State: ");
  Serial.println(state);
  Serial.print("Rx signal: ");
  Serial.println(rxSignal);
  Serial.print("Power level: ");
  Serial.println(powerLevel);
  Serial.print("Battery Voltage: ");
  Serial.println(batteryVoltage);
  Serial.print("On time: ");
  Serial.println(millis() - startTime);
  Serial.println();
#endif



  delay(500);
  switch (state) {
  case STANDBY:
    //Serial.println("STANDBY");
    if (rxSignal == HIGH){
      state = POWER;
      powerLevel = HIGH;
      startTime=millis();
    }
    break;

  case POWER:
    //Serial.println("POWER");
    if (((millis() - startTime) > duration) || (rxSignal == LOW) || (batteryVoltage < batteryVoltageThreshold)) {
      state = TIMEOUT;
      powerLevel = LOW;
    }
    break;

  case TIMEOUT:
    powerLevel = LOW;
    //Serial.println("TIMEOUT");
    if (rxSignal == HIGH) {
      state = TIMEOUT;
    }
    else {
      state = STANDBY;  
    };
    break;   

  };
};

