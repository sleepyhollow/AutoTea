
#include <LiquidCrystal.h> // For LCD
#include <Stepper.h>  // For Steeper Motor
#include "SR04.h" // For Stepper Motor
//https://github.com/BretStateham/28BYJ-48/tree/master/Arduino/Code

/* DEFFINE PINNS */
// For the Ultrasonic sensor
#define TRIG_PIN 13
#define ECHO_PIN 6
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

// For the RGB LED
#define BLUE 48
#define GREEN 50
#define RED 52


const int stepsPerRevolution = 1500;  // change this to fit the number of steps per revolution [default 1500]

// Code for the stepper motor
Stepper myStepper(stepsPerRevolution, 2, 4, 3, 5);

/* Diagrams
LCD Pin             Potentiometer Pin             Arduino Mega 2560 Pin
1(GND)              GND                           GND
2(Vcc)              Vcc                           +5V
3(Vo)               Out
4(RS)                                             12
5(RW)
6(EN)                                             11
7(DB0)
8(DB1)
9(DB2)
10(DB3)
11(DB4)                                           5
12(DB5)                                           4
13(DB6)                                           3
14(DB7)                                           2
15(Bkl+)                                          +5V
16(Bkl-)                                          GND
*/

// Switch's pin
const int SWITCH = 53;

// Assign display pins to board pins
const int LCD_RS = 7,
          LCD_EN = 8,
          LCD_D4 = 9,
          LCD_D5 = 10,
          LCD_D6 = 11,
          LCD_D7 = 12;

// Initialize display
LiquidCrystal lcd(LCD_RS,
                  LCD_EN,
                  LCD_D4,
                  LCD_D5,
                  LCD_D6,
                  LCD_D7);

// Message strings
const String MSG_ATTACH_TEABAG = "Attach Teabag";
const String MSG_DIPPING = "Dipping .......";
const String MSG_WAIT = "Please Wait";
const String MSG_DONE = "   Done!   ";
const String MSG_CLEANUP = "Remove Teabag";
const String MSG_THANK_YOU = "Thank You :) ";


// Declare Var to store distance measurements 
long distanceInCm; // Store Distance In Cm
long distanceInInches; // Store Distance In Inches

void setup() {
  Serial.begin(9600);
  
  // LED 
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  digitalWrite(RED, HIGH);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);

  // Button pin
  pinMode(SWITCH, INPUT);


  // Set the Stepper speed at 20rpm
  myStepper.setSpeed(20);

  // Set display's number of columns and rows
  lcd.begin(18, 2);

  // prompt user to place teabag
  lcd.print(MSG_ATTACH_TEABAG); 

  // Wait till the User Pushes Button
  while (digitalRead(SWITCH) == LOW) {  /* Do Nothing, Just Wait the*/ }
  
  Serial.println("button pressed");
 
  lcd.print(MSG_WAIT); delay(000);
  lcd.clear();

  distanceInCm=sr04.Distance(); /// Get the distance in cm  // 1″ = 2.54cm
  distanceInInches = distanceInCm * 2.54;
  //lcd.print("Cup is (%ld)In or %ld (cm) Away",distanceInInches, distanceInCm);
  
  lcd.print("Cup is ");
  lcd.print(distanceInCm);
  lcd.print(" cm Away");
  delay(3000);
  lcd.clear();

   
  // Display the Dipping Message With Flash
  ledSetColor(0,255,0); // Green Color
  lcd.print(MSG_DIPPING);

  int count = 0;

  // Dip 3 Times 
  while(count < 3) // count < dipTime
  {
    myStepper.step(stepsPerRevolution);
    ledSetColor(0,0,255); // Blue Color
    myStepper.step(-stepsPerRevolution);
    ledSetColor(0,255,0); // Green Color

    count++;
  }

  lcd.clear(); 
  ledSetColor(255,0,0); // Red Color
  lcd.print(MSG_DONE); delay(3000);
  lcd.clear();
   
  lcd.print(MSG_CLEANUP); 
  ledSetColor(255,0,255); // Purple Color
  while (digitalRead(SWITCH) == LOW) {  /* Do Nothing, Just Wait the*/ }
  lcd.clear();
   
  lcd.print(MSG_THANK_YOU);
  scrollText(MSG_THANK_YOU);
  delay(3000);
  myStepper.step(stepsPerRevolution);
  lcd.clear();
  ledSetColor(0,0,0); // Blank Light 
  exit(0); // exit Program
     
}

// define variables For LED
int redValue;
int greenValue;
int blueValue;

void loop() {


}

void ledSetColor(int redValue, int greenValue, int blueValue)
{
  
  analogWrite(RED, redValue);
  analogWrite(GREEN, greenValue);
  analogWrite(BLUE, blueValue);
   
}
// https://www.arduino.cc/en/Tutorial/LiquidCrystalScroll
void scrollText(String str) {
 // scroll 13 positions (string length) to the left
  // to move it offscreen left:
  for (int positionCounter = 0; positionCounter < str.length() / 2; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(150);
  }

  // scroll 29 positions (string length + display length) to the right
  // to move it offscreen right:
  for (int positionCounter = 0; positionCounter < str.length() / 2; positionCounter++) {
    // scroll one position right:
    lcd.scrollDisplayRight();
    // wait a bit:
    delay(150);
  }

}
