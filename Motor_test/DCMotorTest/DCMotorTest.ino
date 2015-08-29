/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);
Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");
  //Input
  pinMode(6,INPUT);
  pinMode(7,INPUT);

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  myMotor1->setSpeed(150);
  myMotor1->run(FORWARD);
  // turn on motor
  myMotor1->run(RELEASE);
  
  //Motor2
  myMotor2->setSpeed(150);
  myMotor2->run(FORWARD);
  // turn on motor
  myMotor2->run(RELEASE);
}

void loop() {
  uint8_t i;
  int sensorVal1 = digitalRead(6);
  int sensorVal2 = digitalRead(7);
  
  Serial.print("tick");
  if(sensorVal1 == HIGH && sensorVal2 == HIGH){
    Serial.print("input high");
    myMotor1->run(FORWARD);
    myMotor2->run(FORWARD);
    myMotor1->setSpeed(50);  
    myMotor2->setSpeed(50);  
    delay(10);   
    myMotor1->run(RELEASE); 
    myMotor2->run(RELEASE); 
    delay(10);   
  }
  else
  {
    Serial.print("no input");
    myMotor1->run(FORWARD);
    myMotor2->run(FORWARD);
    myMotor1->setSpeed(0);  
    myMotor2->setSpeed(0);
    delay(10);   
    myMotor1->run(RELEASE); 
    myMotor2->run(RELEASE); 
    delay(10);
  }

    
}
