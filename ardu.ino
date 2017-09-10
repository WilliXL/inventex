/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/
#include <Adafruit_NeoPixel.h>
#include <Adafruit_MotorShield.h>

#include <Servo.h>
#include <Wire.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_StepperMotor *stepperBin = AFMS.getStepper(200,1);
Adafruit_StepperMotor *stepperPush = AFMS.getStepper(200,2);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(14, 34, NEO_GRB + NEO_KHZ800);
int pinBin = 30;
int pinPush = 26;
int binServoFlat = 950;
int pushServoZero = 1440;
Servo binServo;
Servo pushServo;
int stepsPerBin = 277;
int stepsPerBinPush = 266;

int depositBin = -1;
int withdrawalBin = -1;

void binReturnToZero() {
  binServo.writeMicroseconds(binServoFlat);
  while (digitalRead(pinBin) != LOW){
    stepperBin->step(1, BACKWARD, SINGLE);
  }
}

void pushReturnToZero() {
  pushServo.writeMicroseconds(pushServoZero);
  while (digitalRead(pinPush) != LOW){
    stepperPush->step(1, BACKWARD, SINGLE);
  }
}

void resetLights(){
  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, 255, 255, 255, 35);
  }
  strip.setBrightness(75);
}

void turnOffLights(){
  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, 0, 0, 0, 0);
  }
}

void makeLightsGreen(){
  for(int i = 0; i < strip.numPixels(); i++){
    strip.setPixelColor(i, strip.Color(0,255,0));
  }
}

void setup() {
  strip.begin();
  resetLights();
  strip.show();
  
  
  Wire.begin(42);
  Wire.onReceive(transmissionTest);
  pinMode(pinBin, INPUT_PULLUP);
  pinMode(pinPush, INPUT_PULLUP);
  AFMS.begin();
  stepperBin->setSpeed(100);
  stepperPush->setSpeed(100);
  binServo.attach(10);
  pushServo.attach(9);
  pushServo.writeMicroseconds(pushServoZero);
  binReturnToZero();
  pushReturnToZero();
  delay(3000);
}

void shaky(){
  for (int pos = 950; pos <= 2350; pos++){
      binServo.writeMicroseconds(pos);
      delay(1);
    }
    delay(1000);
    binServo.writeMicroseconds(1800);
    delay(200);
    binServo.writeMicroseconds(2250);
    delay(200);
    binServo.writeMicroseconds(1800);
    delay(200);
    binServo.writeMicroseconds(2250);
    delay(200);
    binServo.writeMicroseconds(1800);
    delay(200);
    binServo.writeMicroseconds(2250);
    delay(200);
    delay(1000);
    for (int pos = 2350; pos >= 950; pos--){
      binServo.writeMicroseconds(pos);
      delay(1);
    }
}

void depositInBin(int bin){
  int blinkDelay = 700;
  makeLightsGreen();
  strip.show();
  binReturnToZero();
  for(int i = 0; i < 5; i++){
    makeLightsGreen();
    strip.show();
    delay(blinkDelay);
    turnOffLights();
    strip.show();
    delay(blinkDelay);
  }
  
  resetLights(); 
  if(depositBin == 0){
    strip.setPixelColor(13, strip.Color(0,255,0));
    strip.setPixelColor(12, strip.Color(0,255,0));
  }
  else if(depositBin == 1){
    strip.setPixelColor(11, strip.Color(0,255,0));
    strip.setPixelColor(10, strip.Color(0,255,0));
  }
  else if(depositBin == 2){
    strip.setPixelColor(9, strip.Color(0,255,0));
    strip.setPixelColor(8, strip.Color(0,255,0));
  }
  else if(depositBin == 3){
    strip.setPixelColor(7, strip.Color(0,255,0));
  }
  else if(depositBin == 4){
    strip.setPixelColor(6, strip.Color(0,255,0));
    strip.setPixelColor(5, strip.Color(0,255,0));
  }
  else if(depositBin == 5){
    strip.setPixelColor(4, strip.Color(0,255,0));
    strip.setPixelColor(3, strip.Color(0,255,0));
  }
  strip.show();
  stepperBin->step(stepsPerBin * bin, FORWARD, SINGLE);
  shaky();
  binReturnToZero();
  depositBin = -1;
  resetLights();
  strip.show();
}

void pushy(){
  int max = pushServoZero + 500;
  for (int pos = pushServoZero; pos < max; pos++){
    pushServo.writeMicroseconds(pos);
    delay(1);
  }
}

void pully(){
  int max = pushServoZero + 500;
  for (int pos = max; pos >= pushServoZero; pos--){
    pushServo.writeMicroseconds(pos);
    delay(1);
  }
}

void pushOutBin(int bin){

  resetLights();
  if(withdrawalBin == 0){
    strip.setPixelColor(13, strip.Color(255,0,0));
    strip.setPixelColor(12, strip.Color(255,0,0));
  }
  else if(withdrawalBin == 1){
    strip.setPixelColor(11, strip.Color(255,0,0));
    strip.setPixelColor(10, strip.Color(255,0,0));
  }
  else if(withdrawalBin == 2){
    strip.setPixelColor(9, strip.Color(255,0,0));
    strip.setPixelColor(8, strip.Color(255,0,0));
  }
  else if(withdrawalBin == 3){
    strip.setPixelColor(7, strip.Color(255,0,0));
  }
  else if(withdrawalBin == 4){
    strip.setPixelColor(6, strip.Color(255,0,0));
    strip.setPixelColor(5, strip.Color(255,0,0));
  }
  else if(withdrawalBin == 5){
    strip.setPixelColor(4, strip.Color(255,0,0));
    strip.setPixelColor(3, strip.Color(255,0,0));
  }
  strip.show();
  pushReturnToZero();
  stepperPush->step(stepsPerBinPush * bin, FORWARD, SINGLE);
  pushy();
  delay(8000);
  pully();
  delay(500);
  pushReturnToZero();
  withdrawalBin = -1;
  resetLights();
  strip.show();
} 

void loop() {
  if(depositBin != -1){
    depositInBin(depositBin);
  }
  if(withdrawalBin != -1){
    pushOutBin(withdrawalBin);
  }
}


void transmissionTest(int numBytes){
  while(Wire.available()){
    int command = Wire.read();
    int bin = Wire.read();
    Serial.println(command);
    Serial.println(bin);
    if(command == 20){
      withdrawalBin = bin;
    }
    else{
      depositBin = bin;
    }
  }
  
}
void onTransmission(int numBytes){


}

  
