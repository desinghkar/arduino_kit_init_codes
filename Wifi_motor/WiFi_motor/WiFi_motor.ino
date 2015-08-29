/* 
  This a simple example of the aREST Library for Arduino (Uno/Mega/Due/Teensy)
  using the CC3000 WiFi chip. See the README file for more details.
 
  Written in 2014 by Marco Schwartz under a GPL license. 
*/

// Import required libraries
#include <Adafruit_CC3000.h>
#include <SPI.h>
#include <CC3000_MDNS.h>
#include <aREST.h>
#include <avr/wdt.h>

/*
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"
*/
// These are the pins for the CC3000 chip if you are using a breakout board
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// Create CC3000 instance
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2);
// Create aREST instance
aREST rest = aREST();

// Your WiFi SSID and password                                         
#define WLAN_SSID       "RLAB"
#define WLAN_PASS       "metropolis"
#define WLAN_SECURITY   WLAN_SEC_WPA2

// The port to listen for incoming TCP connections 
#define LISTEN_PORT           80

// Server instance
Adafruit_CC3000_Server restServer(LISTEN_PORT);

// DNS responder instance
MDNSResponder mdns;

// Create the motor shield object with the default I2C address
//Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
//Adafruit_DCMotor *myMotor1 = AFMS.getMotor(1);
//Adafruit_DCMotor *myMotor2 = AFMS.getMotor(2);
// You can also make another motor on port M2

void setup(void)
{  
  // Start Serial
  Serial.begin(115200);
  Serial.println(F("Starting!!..."));
  // Init variables and expose them to REST API
   // Function to be exposed
  rest.function("motor1",motorControl1);
  rest.function("motor2",motorControl2);
  
  // Give name and ID to device
  rest.set_id("008");
  rest.set_name("mighty_cat");
  
  // Set up CC3000 and get connected to the wireless network.
  if (!cc3000.begin())
  {
    while(1);
  }
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    while(1);
  }
  while (!cc3000.checkDHCP())
  {
    delay(100);
  }
  Serial.println();
  
  // Print CC3000 IP address. Enable if mDNS doesn't work
  while (! displayConnectionDetails()) {
    delay(1000);
  }
  
  // Start multicast DNS responder
  if (!mdns.begin("arduino", cc3000)) {
    while(1); 
  }
   
  // Start server
  restServer.begin();
  Serial.println(F("Listening for connections..."));

  // Enable watchdog
  wdt_enable(WDTO_4S);

  /*
  //Serial.begin(9600);           // set up Serial library at 9600 bps
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
  */
}

void loop() {
  
  // Handle any multicast DNS requests
  mdns.update();
  
  // Handle REST calls
  Adafruit_CC3000_ClientRef client = restServer.available();
  rest.handle(client);
  wdt_reset();

  // Check connection, reset if connection is lost
  if(!cc3000.checkConnected()){while(1){}}
  wdt_reset();
  
  motor_effect();
 
}

void motor_effect()
{
    uint8_t i;
  int sensorVal1 = digitalRead(6);
  int sensorVal2 = digitalRead(7);
  /*
  Serial.print("tick");
  if(sensorVal1 == HIGH && sensorVal2 == HIGH){
    Serial.print("input high");
    myMotor1->run(FORWARD);
    myMotor2->run(FORWARD);
    myMotor1->setSpeed(150);  
    myMotor2->setSpeed(150);  
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
  */
}

// Print connection details of the CC3000 chip
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

// Custom function accessible by the API
int motorControl1(String command) {
  int state = command.toInt();
  digitalWrite(3,state);
  return 1;
}
int motorControl2(String command) {
  
  // Get state from command
  int state = command.toInt();
  digitalWrite(4,state);
  return 1;
}
