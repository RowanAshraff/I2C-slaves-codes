#include <Wire.h>                            
#define SlaveAdress 2  // Adress of this slave

int outVal[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};   // An array for the values of the PWM pins 
int outPin[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};   // An array for the PWM/digital pins of the microcontroller
                                                           //Pins 10-13 are Digital+ pins3
int counter = 0, val = 0;
  

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
  for(int k = 0; k < 8; k++){
    analogVal[k] = analogRead(analogPin[k]);
    analogVal[k] = mapping(analogVal[k], 0,1023,0,255);     // Converting it to the specified range
    }
}


void RecieveEvent(int Press)    // A function that runs each time a master sends a number
{
  while(Wire.available()){
    if(Wire.read() == 's'){
      Serial.println("recieving: ");
      for(int i = 0; i < 12; i++){
        outVal[i] =  Wire.read();
        Serial.println(String(outVal[i]));
      }
    }
  }
}


void requestEvent(){ // A function that runs each time a master requests a byte
  Serial.println(String(analogVal[0]));
  Wire.write(analogVal[0]);              // Sending the value to the master
  counter += 1;
  if (counter == 8){ //Resetting the counter once 8 pins have been read and their values sent
    counter = 0;
  }
}

int mapping(int value, int startPoint1, int endPoint1, int startPoint2, int endPoint2){  // It acts exactly like the arduino "map" function
  return(value*(float(endPoint2-startPoint2)/float(endPoint1-startPoint1)));
}
