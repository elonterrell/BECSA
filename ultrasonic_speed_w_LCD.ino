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



bool proximity1 = 0;
bool proximity2 = 0;
unsigned long time1;
unsigned long time2;
float diff1;
float diff2;
float deltat_microsec;
float deltat;
float speed;
float distance_in = 7.5; //Separation distance between sensors, in inches
float distance = distance_in*0.0254;  //Convert separation distance from inches to meters


void setup() {
  pinMode(2, OUTPUT);//define arduino pin for the trigger, or output signal
  pinMode(4, INPUT);//define arduino pin for the echo, or input signal

  pinMode(6, OUTPUT);//define arduino pin for the trigger, or output signal
  pinMode(8, INPUT);//define arduino pin for the echo, or input signal

  
  Serial.begin(9600);//enable serial monitor

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  ////////////////////////////////////////
 
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Waiting for object"); 
  display.display();


}
void loop() {
  ////////////////////////////////////////////////////////////////////////////////////////////
  //pulse output - sensor 1
  digitalWrite(2, LOW);
  delayMicroseconds(4);
  digitalWrite(2, HIGH);
  delayMicroseconds(10);
  digitalWrite(2, LOW);
  
  long t1 = pulseIn(4, HIGH);//record input pulse time and save it as a variable
  
  float inches1 = t1 / 74.0/ 2; //convert time into distance, in inches
  String inch1 = " inches1 t";

  ////////////////////////////////////////////////////////////////////////////////////////////
  //pulse output - sensor 2
  digitalWrite(6, LOW);
  delayMicroseconds(4);
  digitalWrite(6, HIGH);
  delayMicroseconds(10);
  digitalWrite(6, LOW);
  
  long t2 = pulseIn(8, HIGH);//record input pulse time and save it as a variable
  
  float inches2 = t2 / 74.0/ 2; //convert time into distance, in inches
  String inch2 = " inches2 t";
  ////////////////////////////////////////////////////////////////////////////////////////////


  // When Sensor1 is first tripped, record the time and trigger the proximity variable for Sensor1.
  if (inches1 < 10.0){
    if (proximity1 == 0){
      proximity1 = 1;
      time1 = micros();
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////////
  // When Sensor2 is first tripped, record the time and trigger the proximity variable for Sensor1.
  if (inches2 < 10.0){
    if (proximity2 == 0){
      proximity2 = 1;
      time2 = micros();
    }
  }
  ////////////////////////////////////////////////////////////////////////////////////////////
  

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
