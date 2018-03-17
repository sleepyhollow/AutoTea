#define IN1  7
#define IN2  8
#define IN3  9
#define IN4  10
#define TRIG_PIN 12
#define ECHO_PIN 11

#include <SR04.h>
#include <Servo.h>

#define MAX_HEIGHT_CM 20
#define STEP_PER_CIRCLE 4095
#define CM_PER_CIRCLE 3

SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);
long distance_cm;

int Steps = 0;
boolean Direction = true;// gre
unsigned long last_time;
unsigned long currentMillis;
int steps_left=4095;
unsigned long time = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(IN1, OUTPUT); 
  pinMode(IN2, OUTPUT); 
  pinMode(IN3, OUTPUT); 
  pinMode(IN4, OUTPUT); 
}
void loop()
{
  distance_cm=sr04.Distance();
  //Serial.print(distance_cm);
  //Serial.println("cm");
  if (distance_cm < MAX_HEIGHT_CM)
  {
    dipping(distance_cm);
  }
}

void dipping(int height) 
{
  while(steps_left>0){
    currentMillis = micros();
    if(currentMillis-last_time>=1000){
      stepper(1); 
      time=time+micros()-last_time;
      last_time=micros();
      steps_left--;
    }
  }
  Direction=!Direction;
  steps_left=height / CM_PER_CIRCLE*steps_left;
  
   while(steps_left>0){
    currentMillis = micros();
    if(currentMillis-last_time>=1000){
      stepper(1); 
      time=time+micros()-last_time;
      last_time=micros();
      steps_left--;
    }
  }
  Direction=!Direction;
  steps_left=height / CM_PER_CIRCLE*steps_left;
}

void stepper(int xw){
  for (int x=0; x<xw;x++){
    switch(Steps){
       case 0:
         digitalWrite(IN1, LOW); 
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, HIGH);
       break; 
       case 1:
         digitalWrite(IN1, LOW); 
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, HIGH);
         digitalWrite(IN4, HIGH);
       break; 
       case 2:
         digitalWrite(IN1, LOW); 
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, HIGH);
         digitalWrite(IN4, LOW);
       break; 
       case 3:
         digitalWrite(IN1, LOW); 
         digitalWrite(IN2, HIGH);
         digitalWrite(IN3, HIGH);
         digitalWrite(IN4, LOW);
       break; 
       case 4:
         digitalWrite(IN1, LOW); 
         digitalWrite(IN2, HIGH);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, LOW);
       break; 
       case 5:
         digitalWrite(IN1, HIGH); 
         digitalWrite(IN2, HIGH);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, LOW);
       break; 
         case 6:
         digitalWrite(IN1, HIGH); 
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, LOW);
       break; 
       case 7:
         digitalWrite(IN1, HIGH); 
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, HIGH);
       break; 
       default:
         digitalWrite(IN1, LOW); 
         digitalWrite(IN2, LOW);
         digitalWrite(IN3, LOW);
         digitalWrite(IN4, LOW);
       break; 
    }
    SetDirection();
  }
} 
  void SetDirection(){
  if(Direction==1){ 
    //Serial.println("Going up");
    Steps++;
  }
  if(Direction==0){ 
    //Serial.println("Going down");
    Steps--; 
  }
  if(Steps>7){Steps=0;}
  if(Steps<0){Steps=7; }
}
