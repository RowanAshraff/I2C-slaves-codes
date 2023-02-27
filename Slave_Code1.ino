#include <Wire.h>                            
#define SlaveAdress 1  // Adress of this slave

int outVal[] = {0, 0, 0, 0, 0, 0, 0, 0};   // An array for the values of the PWM pins 
int outPin[] = {2, 3, 4, 5, 6, 7, 8, 9};   // An array for the PWM pins of the microcontroller
int counter = 0, counterAnalog = 0, val = 0;
  

int analogVal[] = {200, 150, 100, 90, 80, 70, 50, 10};    // An array for the values of the analog pins
int analogPin[] = {A0, A1, A2, A3, A4, A5, A6, A7};       // An array for the PWM pins

void setup() 
{
  // This for loop is for initiating 8 PWM pins and 8 analog pins
  for(int i = 0; i < 8; i++){
    pinMode(outPin[i], OUTPUT);
    pinMode(analogPin[i], INPUT);         
  }
  
  Wire.begin(SlaveAdress);                                
  Wire.onReceive(RecieveEvent); 
  Wire.onRequest(requestEvent);  
  Serial.begin(9600);
  Serial.println("slave");
                      
}

void loop() 
{
  delay(100);
}

void RecieveEvent(int Press)    // A function that runs each time a master sends a number
{
  while(Wire.available()){ 
    outVal[counter] =  Wire.read();
//    Serial.println(outVal[counter]);                   
    counter += 1;
    if (counter == 8){    // Get 8 PWM readings from the master then resetting it
      counter = 0;   
    }
  }
}


void requestEvent(){ // A function that runs each time a master requests a byte
  analogVal[counterAnalog] = analogRead(analogPin[counterAnalog]);                // Reading analog pins values
  analogVal[counterAnalog] = mapping(analogVal[counterAnalog], 0,1023,0,255);     // Converting it to the specified range

  Wire.write(analogVal[counterAnalog]);              // Sending the value to the master
  counterAnalog += 1;
  if (counterAnalog==8){ //Resetting the counter once 8 pins have been read and their values sent
    counterAnalog = 0;
  }
}


int mapping(int value, int startPoint1, int endPoint1, int startPoint2, int endPoint2){  // It acts exactly like the arduino "map" function
  return(value*(float(endPoint2-startPoint2)/float(endPoint1-startPoint1)));
}
