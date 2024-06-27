#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// -----------------------

#define laser1 2
#define sensor1 3
#define laser2 4
#define sensor2 5

// Downstream distance between lasers, in meters
float distance = 0.1778;

unsigned long time1;
unsigned long time2;
float speed;
bool proximity1 = 0;
bool proximity2 = 0;
float deltat;

void setup() {

  Serial.begin(9600);
  pinMode(laser1, OUTPUT);
  pinMode(sensor1, INPUT);
  pinMode(laser2, OUTPUT);
  pinMode(sensor2, INPUT);

  digitalWrite(laser1, HIGH);
  digitalWrite(laser2,HIGH);

  // -----------------

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Speed sensor ");
  display.display();

  delay(1000);

  //--------------------------

  display.clearDisplay();
  display.setCursor(0,0);
  display.println("Waiting for Object");
  display.display();

}

void loop() {

  // Determine if either sensor is tripped
  bool sensor1value = digitalRead(sensor1);
  bool sensor2value = digitalRead(sensor2);

  // When Sensor 1 is tripped, record the time and trigger the proximity variable for Sensor 1.
  if (sensor1value == 1){
    if (proximity1==0){
      proximity1 = 1;
      time1 = micros();
      Serial.print("Sensor 1 tripped at ");
      Serial.print(time1);
      Serial.println(" microsec");
    }
  }

  // When Sensor 2 is tripped, record the time and trigger the proximity variable for Sensor 2.
  if (sensor2value == 1){
    if (proximity2==0){
      proximity2 = 1;
      time2 = micros();
      Serial.print("Sensor 2 tripped at ");
      Serial.print(time2);
      Serial.println(" microsec");
    }    
  }

   // Once both proximity variables are tripped, then calculate the elapsed time between triggers, calculate and 
  // display the speed of the object, and reset the proximity variables so that we can measure again.
  if (proximity1 == 1){
    if (proximity2 == 1){

      deltat = (time1 - time2)/1000000.0;  //Convert microseconds to seconds

      speed = distance/deltat; //Calculate speed, in meters per second
      
      proximity1 = 0;
      proximity2 = 0;

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print("V=");
      display.print(speed);
      display.println(" m/s");
  
      display.display();

      delay(5000);//delay
      display.clearDisplay();


      ////////////////////////////////////////
     
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.println("Waiting for object"); 
      display.display();
      
    }
  }



}