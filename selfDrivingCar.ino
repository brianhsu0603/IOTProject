#include <Servo.h> 
#include  <SoftwareSerial.h>
#include <Wire.h>

float distance=100;       
float rdistance = 100;     
float ldistance = 100;   
Servo myservo;     
int pinI1=7;     
int pinI2=8;     
int pinI3=9;    
int pinI4=10;    
int ENA=3;  
int ENB=5;   
int ECHO = A0;  
int TRIG = A1;
int LLS = 11;  
int LS = 6;
int RS = 4;
int RRS = 2;
void setup()
{
  myservo.attach(A2);      
  pinMode(pinI1,OUTPUT); // left motor
  pinMode(pinI2,OUTPUT); // left motor
  pinMode(pinI3,OUTPUT); // right motor
  pinMode(pinI4,OUTPUT); // right motor
  pinMode(ENA,OUTPUT);  // power of left motor
  pinMode(ENB,OUTPUT);  // power of right motor
  pinMode(ECHO, INPUT);    // ultra sonic sensor input
  pinMode(TRIG, OUTPUT);  // ultra sonic sensor output
  pinMode(LS,INPUT); // left IR sensor
  pinMode(RS,INPUT); // right IR sensor
  pinMode(LLS,INPUT); // left IR sensor
  pinMode(RRS,INPUT); // right IR sensor
  Serial.begin(9600);        
  myservo.write(90); // let the servo motor face front
  delay(200); // delay time
  // start the robot car by putting my hand infront of the sensor
  while(distance > 2.5 ) 
     {
      detect(90);
     } 
      delay(1000); 
}



void loop()
{
  detect(90); // detect the distance
  
  if (distance <14) // if too close to the obstacle
  {
    stop_();
    while(distance<14) // reverse until the distance is safe
    {
      reverse(); //reverse the robot car
      stop_();
      detect(90); 
      delay(100);
    }
  }
  
  if(distance < 15) // the safe distance to avoid obstacle
  {
  stop_(); // stop 
  avoid_obstacle(); // avoid obstacle
  } 
  
  else
  {
  follow_line(); // follow line
  }
}



  
void follow_line() // following the line
{
  if(!(digitalRead(LS)) && !(digitalRead(RS)) && !(digitalRead(LLS)) && !(digitalRead(RRS))) // Move forward
  { 
   move_forward(50);
  }
  
  if(digitalRead(LS) && !(digitalRead(RS)) && !(digitalRead(LLS)) && !(digitalRead(RRS))) // Turn left
  {
    turn_left();
    delay(100);
  }
  
  if(!(digitalRead(LS)) && digitalRead(RS) && !(digitalRead(LLS)) && !(digitalRead(RRS))) // Turn right
  {
    turn_right();
    delay(100);
  }

  if(digitalRead(LLS) && !(digitalRead(LS)) && !(digitalRead(RS)) && !(digitalRead(RRS))) // Turn left
  {
    turn_left();
    delay(100);
  }
  
  if(digitalRead(RRS) && !(digitalRead(LS)) && !(digitalRead(RS)) && !(digitalRead(LLS))) // Turn right
  {
    turn_right();
    delay(100);
  }
  
  if((digitalRead(LS)) && (digitalRead(RS)) && (digitalRead(LLS)) && (digitalRead(RRS)) ) // Stop
  {
    stop_();
    delay(100);
  }
  
}


void avoid_obstacle()
{
  
detect(60); // check if left side is blocked
delay(400);

detect(120); // check if right side is blocked
delay(400);

myservo.write(90);
delay(200);

if ((ldistance > 20) && (rdistance > 20)) // Case1: both sides are ok to pass
{
  
  if(ldistance >= rdistance) // left side more spacious than right side
  {
    avoid_by_left();
  }

  else if(ldistance<rdistance) // right side more spacious than left side
  {
    avoid_by_right();
  }
}

else if ((ldistance > 20 ) && (rdistance < 20)) // Case2: left side is ok to pass
{
  avoid_by_left();
}

else if((ldistance < 20) && (rdistance >20 )) // Case3: right side is ok to pass
{
  avoid_by_right();
}
    
else if(ldistance<20 && rdistance<20) // Case 4: both sides are blocked
{
  while(distance<15 && ldistance<20 && rdistance<20) // keep detecting the distances
  {
  detect(60);
  delay(400);
  detect(90);
  delay(400);
  detect(120);
  delay(400);
  }
  if (distance>15){ // if obstacle removed, move forward
    move_forward(60);
  }
  else if (distance <15 && ldistance>20){ // if obstacle move a bit right, pass by going left
    avoid_by_left();
  }
  else if (distance < 15 && rdistance>20){ // if obstacle move a bit left, pass by going right
    avoid_by_right();
  }

 }
}

void test1(){
  while(distance<15)
 {
  detect(90);
  delay(200);
 }
 move_forward(50);
}

void test2(){
   while(distance<15 && ldistance<20 && rdistance<20)
  {
  detect(60);
  delay(400);
  detect(90);
  delay(400);
  detect(120);
  delay(400);
  }
  if (distance>15){
    move_forward(60);
  }
  else if (distance <15 && ldistance>20){
    avoid_by_left();
  }
  else if (distance < 15 && rdistance>20){
    avoid_by_right();
  }

}



void stop_() // stop the robot car
{ 
    
    analogWrite(ENA,50); // setting the speed of left motor
    analogWrite(ENB,50); // setting the speed of right motor
    digitalWrite(pinI1, 0);
    digitalWrite(pinI2, 0);
    digitalWrite(pinI3, 0);
    digitalWrite(pinI4, 0);
 }

void detect(int angle) // detect the distance with the obstacles, angle = 90,120,60
{
        myservo.write(angle); // ultrasonic sensor faces front/left/right 
        digitalWrite(TRIG, LOW);  // emit sound pulse
        delayMicroseconds(2);
        digitalWrite(TRIG, HIGH); // emit sound pulse 
        delayMicroseconds(10);
        digitalWrite(TRIG, LOW);  // emit sound pulse 
        distance = pulseIn(ECHO, HIGH); // calculating time difference 
        distance = distance/5.8/10; //calculating distance from time difference
}  

void move_forward(int se) // move foward
{
    analogWrite(ENA,se);
    analogWrite(ENB,se);
    digitalWrite(pinI1, 1);
    digitalWrite(pinI2, 0);
    digitalWrite(pinI3, 1);
    digitalWrite(pinI4, 0);
  }

void turn_left() // turn left
{
    analogWrite(ENA,80);
    analogWrite(ENB,80);
    digitalWrite(pinI1, 0);
    digitalWrite(pinI2, 0);
    digitalWrite(pinI3, 1);
    digitalWrite(pinI4, 0);
  
}

void turn_right() // turn right
{
    analogWrite(ENA,80);
    analogWrite(ENB,80);
    
    digitalWrite(pinI1, 1);
    digitalWrite(pinI2, 0);
    digitalWrite(pinI3, 0);
    digitalWrite(pinI4, 0);
  
}

void reverse() //reverse
{
    analogWrite(ENA,60);
    analogWrite(ENB,60);
    
    digitalWrite(pinI1, 0);
    digitalWrite(pinI2, 1);
    digitalWrite(pinI3, 0);
    digitalWrite(pinI4, 1);
}

void avoid_by_right() // avoid the obstacle by going right side
{ 
    turn_right();
    delay(300);
    
    move_forward(60);
    delay(1000);
    
    turn_left();
    delay(300);
    
    myservo.write(180);
    delay(200);

    detect(90);
    
// this step is to make sure the robot car completely passed the obstacle
    while(distance<10) 
    { 
    
    move_forward(60);
    detect(90);
    }
    myservo.write(90);
    delay(200);

    move_forward(60);
    delay(200);
    
    turn_left();
    delay(600);
    }


void avoid_by_left() // avoid the obstacle by going left side
{ 
    turn_left();
    delay(300);
    
    move_forward(60);
    delay(1000);
    
    turn_right();
    delay(300);
    
    myservo.write(0);
    delay(200);

    detect(90);
    while(distance<10){
    
    move_forward(60);
    detect(90);
    }
    myservo.write(90);
    delay(200);

    move_forward(60);
    delay(200);
    
    turn_right();
    delay(600);
  
}
