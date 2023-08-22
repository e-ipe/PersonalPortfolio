
#include <Enes100.h>
#include "Adafruit_TCS34725.h"
#include <Servo.h>
#include <Wire.h>
#define trigPinLeft 8 // attach pin D2 Arduino to pin Trig of HC-SR04
#define echoPinLeft 9 // attach pin D3 Arduino to pin Echo of HC-SR04
#define trigPinRight 4 //attach pin D4 Arduino to pin Trig of HC-SR04
#define echoPinRight 5 // attach pin D5 Arduino to pin Echo of HC-SR04
float x;
float y;
float theta;
int row;
int col;
int runCount = 0;
float mytheta = theta + (2*3.14);
// defines variables for distance measurement
long durationLeft = 0; // variable for the duration of sound wave travel
long durationRight = 0;
int distanceLeft = 0; // variable for the distance measurement
int distanceRight = 0;
int motorleftpin1 = 6;
int motorleftpin2 = 7;
int motorrightpin1 = 2;
int motorrightpin2 = 3;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
Servo myservo;
int progress = 0;
int colCount = 0;




void setup() {
Enes100.begin("Family", CRASH_SITE, 19, 10, 11);
pinMode(trigPinLeft, OUTPUT); // Sets the trigPin4 as an OUTPUT
pinMode(echoPinLeft, INPUT); // Sets the echoPin3 as an INPUT
pinMode(trigPinRight, OUTPUT); // Sets the echoPin4 as an OUTPUT
pinMode(echoPinRight, INPUT); // Sets the echoPin5 as an INPUT
Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate spe
pinMode(motorleftpin1, OUTPUT);
pinMode(motorleftpin2, OUTPUT);
pinMode(motorrightpin1, OUTPUT);
pinMode(motorrightpin2, OUTPUT);
Serial.println("hello!");
/*
if (tcs.begin()) {
  Serial.println("Found sensor");
} else {
  Serial.println("No TCS34725 found ... check your connections");
  while (1);
}
*/
Serial.println("Bye!");
 myservo.attach(13); // uses pin 9
 //myservo.write(30); // initial pos is 30
}
void loop() {
//finding location of aruco marker
Enes100.updateLocation();
x = Enes100.location.x;
y = Enes100.location.y;
theta = Enes100.location.theta;
row = findRow(x);
col = findCol(y);




if (row == 0 && progress == 0){
 Enes100.updateLocation();
 if (progress == 0){
  inRowOne();
 }


 Enes100.println("Reading red or silver?");
 for (int i = 0; i < 3; i++){
   uint16_t r, g, b, c, colorTemp, lux;
   tcs.getRawData(&r, &g, &b, &c);
   // colorTemp = tcs.calculateColorTemperature(r, g, b);
   colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
   lux = tcs.calculateLux(r, g, b);
   Enes100.print("Color Temp: "); Enes100.print(colorTemp); Enes100.print(" K - ");
    if ((colorTemp) < (3850)) {
     Enes100.println("Red Side ");
     Enes100.mission(DIRECTION, NEG_Y);


   }
   else {
     Enes100.println("Silver Side ");
     Enes100.mission(DIRECTION, NEG_X);


   }
 }
 moveBack();
 delay(2000);
 theta = Enes100.location.theta;
 Enes100.updateLocation();
 rightTurn90();
 myservo.write(40);
 //adjustOrientation(theta, row, col);
 moveForward();
 delay(8000);
 colCount = 1;
 Enes100.println("ColCount: "); Enes100.print(colCount);
 Enes100.println("Progress: "); Enes100.print(progress);
 row = 1;


}


else if (row == 0){
 moveForward();
 delay(1000);
}


else{
 //navigation
  adjustOrientation(theta, row, col);
  Enes100.updateLocation();
   //general navigation
  if (!distanceSense()){
    Enes100.println("Moving forward");
    moveForward();
  }
  if (distanceSense()) {
    Enes100.println("STOP!");
    obstacleMove(row, col, theta);
  }
  if (row == 3 && y < 1.5){
   //to change it to column 1, where the limbo is
   moveBack();
   delay(2000/1.7);
   leftTurn90();
   //moveHalt();
   //delay(1000);
   moveForward();
   delay(3000);
   rightTurn();
   delay(1000);
   adjustOrientation(theta, row, col);
  }
}
}




//determines row of OSV
int findRow (float x) {
 int row = 0;
 //grid system, finding x
 if (x <= 1.00){
  // Enes100.println("Row 0");
   row = 0;
 }
 else if (x > 1.00 && x <= 1.99){
   //Enes100.println("Row 1");
   row = 1;
 }
 else if (x >= 2.00 && x <= 2.99){
   //Enes100.println("Row 2");
   row = 2;
 }
 else if (x >= 3.00){
   //Enes100.println("Row 3");
   row = 3;
 }
 return row;
}


//determines column of OSV
int findCol(float y){
 int col = 0;
 //grid system, finding y
 if (y <= 1.00){
   //Enes100.println("Column 0");
   col = 0;
 }
 else if (y > 1.00){
  // Enes100.println("Column 1");
   col = 1;
 }
 return col;
}
//adjusts angle to forward
void adjustOrientation (float theta, int row, int col){
 //past the mission row, adjusts to theta = 0
   if (theta > 0.05 || theta < -0.05){
    theta = Enes100.location.theta;
    Enes100.updateLocation();
    if (theta > 0){
     rightTurn();
    }
    else if (theta < 0){
      leftTurn();
    }
   }
   Enes100.println("Theta = 0");
}


//returns true if obstacle is too close
bool distanceSense (){
 //Serial.println(“Sensing distance forward”)
 // SENSOR BLOCK LEFT
 // Clears the trigPin condition
 digitalWrite(trigPinLeft, LOW);
 digitalWrite(trigPinRight, LOW);
 delay(2);
 // Sets the trigPin HIGH (ACTIVE) for 500 ms
 digitalWrite(trigPinLeft, HIGH);
 // Equation for returning value per cm of movement: (1000 ms / speed of vehicle in cm)
 //delay(1000/1.7);
 digitalWrite(trigPinLeft, LOW);
 // Reads the echoPin, returns the sound wave travel time in microseconds
 durationLeft = pulseIn(echoPinLeft, HIGH);
 // Calculating the distance
 distanceLeft = durationLeft * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
 // Displays the distance on the Serial Monitor
 Serial.print("Distance Left: ");
 Serial.print(distanceLeft);
 Serial.println(" cm");
 // SENSOR BLOCK RIGHT
 digitalWrite(trigPinRight, LOW);
 delayMicroseconds(2);
 digitalWrite(trigPinRight, HIGH);
 digitalWrite(trigPinRight, LOW);
 durationRight = pulseIn(echoPinRight, HIGH);
 distanceRight = durationRight * 0.034 / 2;
 Serial.print("                         Distance right: ");
 Serial.print(distanceRight);
 Serial.println(" cm");
 // END OF SENSOR READINGS
 float avg = (distanceLeft + distanceRight)/2;
 float avg2 = (distanceLeft - distanceRight);
 if ( distanceLeft <= 7 || distanceRight<=7){
   Serial.print("stop");
   return true;
 }
 return false;
}
//90 degree turns if an obstacle is sensed
void obstacleMove (int row, int col, float theta){
  //if it's on the right side
  if (col == 0){
    moveBack();
     delay(2000/1.7);
    leftTurn90();
     moveForward();
     delay(4000);
    //rightTurn();
    //delay(1000);
    adjustOrientation(theta, row, col);
  }
  //left side
  else {
    moveBack();
    delay(2000/1.7);
    rightTurn90();
    moveForward();
    delay(4000);
    //leftTurn();
   // delay(1000);
    adjustOrientation(theta,row, col);
  }
}
//information block for rotation
//one revolution: 19.45 seconds
//half revolution: 9.40 seconds
void moveHalt (){
 //RIGHT WHEELS
 digitalWrite(motorrightpin1, LOW);
 digitalWrite(motorrightpin2, LOW);
 //LEFT WHEELS
 digitalWrite(motorleftpin1, LOW);
 digitalWrite(motorleftpin2, LOW);
}
void moveForward (){
 //RIGHT WHEELS
 digitalWrite(motorrightpin1, HIGH);
 digitalWrite(motorrightpin2, LOW);
 //LEFT WHEELS
 digitalWrite(motorleftpin1, LOW);
 digitalWrite(motorleftpin2, HIGH);
}




void moveBack (){
 //RIGHT WHEELS
 digitalWrite(motorrightpin1, LOW);
 digitalWrite(motorrightpin2, HIGH);
 //LEFT WHEELS
 digitalWrite(motorleftpin1, HIGH);
 digitalWrite(motorleftpin2, LOW);
}
//continuous right turn
void rightTurn (){
 //RIGHT WHEELS
 digitalWrite(motorrightpin1, LOW);
 digitalWrite(motorrightpin2, HIGH);
 //LEFT WHEELS
 digitalWrite(motorleftpin1, LOW);
 digitalWrite(motorleftpin2, HIGH);
}
//continuous left turn
void leftTurn(){
 //RIGHT WHEELS
 digitalWrite(motorrightpin1, HIGH);
 digitalWrite(motorrightpin2, LOW);
 //LEFT WHEELS
 digitalWrite(motorleftpin1, HIGH);
 digitalWrite(motorleftpin2, LOW);
}
void rightTurn90 (){
 //right turn
 //RIGHT WHEELS
 digitalWrite(motorrightpin1, LOW);
 digitalWrite(motorrightpin2, HIGH);
 //LEFT WHEELS
 digitalWrite(motorleftpin1, LOW);
 digitalWrite(motorleftpin2, HIGH);
 delay(4200);
}
void leftTurn90 (){
 //one revolution: 19.45
 //half revolution: 9.40
 //RIGHT WHEELS
 digitalWrite(motorrightpin1, HIGH);
 digitalWrite(motorrightpin2, LOW);
 //LEFT WHEELS
 digitalWrite(motorleftpin1, HIGH);
 digitalWrite(motorleftpin2, LOW);
 delay(4200);
}
// For col == 0, left turn
void inCol0 (){
while (mytheta > 1.60 || mytheta < 1.54){
   rightTurn();
   mytheta = Enes100.location.theta;
   Enes100.updateLocation();
   //Enes100.println(mytheta);
}


 moveForward();
 delay(9000);
 moveHalt();
 delay(5000);
}
// For col == 1, left turn
void inCol1(){
//RIGHT WHEELS
digitalWrite(motorrightpin1, HIGH);
digitalWrite(motorrightpin2, LOW);
//LEFT WHEELS
digitalWrite(motorleftpin1, HIGH);
digitalWrite(motorleftpin2, LOW);
while (mytheta < -1.63 || mytheta > -1.57){
  mytheta = Enes100.location.theta;
  Enes100.updateLocation();
}
moveForward();
delay(9000);
moveHalt();
delay(10000);




}
void redSide (){
bool red = false;
int count = 1;
int turnCount = 0;
while (count < 6 && !red){
  uint16_t r, g, b, c, colorTemp, lux;
   tcs.getRawData(&r, &g, &b, &c);
  // colorTemp = tcs.calculateColorTemperature(r, g, b);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);
   Enes100.print("Color Temp: "); Enes100.print(colorTemp); Enes100.print(" K - ");
  if ((colorTemp) < (3850)) {
    Enes100.println("Red Side ");
    red = true;
  
  }
  else {
    Enes100.println("Silver Side ");
    //switching to next side
    if (col==1){
       moveBack();
       delay(2000/1.7);
       rightTurn90();
       moveForward();
       delay(2000);
       leftTurn90();
       moveForward();
       delay(2000);
       leftTurn90();
       moveHalt();
    }
    if (col==0){
       moveBack();
       delay(2000/1.7);
       leftTurn90();
       moveForward();
       delay(2000);
       leftTurn90();
       moveForward();
       delay(2000);
       leftTurn90();
       moveHalt();
    }
  }
  count++;
  turnCount++;
 }
 if (count!=1 || count!=2){
   count = 3;
 }
  Enes100.println("Count: ");
  Enes100.print(count);
  //to print which side
  if (col==0){
      if (count==1){
        Enes100.mission(DIRECTION, POS_Y);
      }
      else if (count==2){
        Enes100.mission(DIRECTION, NEG_X);
      }
      else if (count==3){
        Enes100.mission(DIRECTION, NEG_Y);
      }
      else {
        Enes100.mission(DIRECTION, POS_X);
      }
    }
    else if (col==1){
      if (count==1){
        Enes100.mission(DIRECTION, NEG_Y);
      }
      else if (count==2){
        Enes100.mission(DIRECTION, POS_X);
      }
      else if (count==3){
        Enes100.mission(DIRECTION, POS_Y);
      }
      else {
        Enes100.mission(DIRECTION, NEG_X);
      }
    }
 //to adjust towards the entire navigation
  adjustOrientation(theta, row, col);
  if (!distanceSense()){
    moveForward();
    delay(2000);
  }
  if (distanceSense()) {
    obstacleMove(row, col, theta);
  }
  Serial.println(" ");
}


void readRed(){
 uint16_t r, g, b, c, colorTemp, lux;
 tcs.getRawData(&r, &g, &b, &c);
 // colorTemp = tcs.calculateColorTemperature(r, g, b);
 colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
 lux = tcs.calculateLux(r, g, b);
 Enes100.print("Color Temp: "); Enes100.print(colorTemp); Enes100.print(" K - ");
  if ((colorTemp) < (3850)) {
    Enes100.println("Red Side ");
    myservo.write(40);


  }
  else {
    Enes100.println("Silver Side ");


  }
}


void rowOne (){
 if (col==0){
   /*while (mytheta > 1.59 || mytheta < 1.56){
   rightTurn();
   mytheta = Enes100.location.theta;
   Enes100.updateLocation();
   //Enes100.println(mytheta);
   }


 moveForward();
 delay(8000);
 moveHalt();
 delay(10000);
*/
inCol0();
 }
 else if (col==1){
   //RIGHT WHEELS
digitalWrite(motorrightpin1, HIGH);
digitalWrite(motorrightpin2, LOW);
//LEFT WHEELS
digitalWrite(motorleftpin1, HIGH);
digitalWrite(motorleftpin2, LOW);
while (mytheta < -1.63 || mytheta > -1.57){
  mytheta = Enes100.location.theta;
  Enes100.updateLocation();
}
moveForward();
delay(8000);
moveHalt();
delay(10000);


 }
}


void inRowOne(){
 double colLow = 0;
 double colHigh = 0;


 if (col==0){
   colLow = 1.56;
   colHigh = 1.59;
 }
 else if (col==1){
   colLow = -1.63;
   colHigh = -1.57;
 }


 while (theta < colLow || theta > colHigh){
   rightTurn();
   theta = Enes100.location.theta;
   Enes100.updateLocation();
 }


   moveForward();
   delay(6500);
   moveHalt();
   delay(5000);


   progress++;
}
