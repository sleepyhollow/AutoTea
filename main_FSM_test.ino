#include <LiquidCrystal.h>
#include <Stepper.h>
#include <Servo.h>
#include <SR04.h> // For ultrasonic sensor
//https://github.com/BretStateham/28BYJ-48/tree/master/Arduino/Code

// States of finite state machine
enum class State {
  INITIAL,
  WAIT_FOR_BAG,
  DIPPING,
  REMOVE_BAG,
  DONE
};

const int LCD_WIDTH = 16;
const int LCD_HEIGHT = 2;
const int PIN_LCD_RS = 7;
const int PIN_LCD_EN = 8;
const int PIN_LCD_D4 = 9;
const int PIN_LCD_D5 = 10;
const int PIN_LCD_D6 = 11;
const int PIN_LCD_D7 = 12;

const int PIN_STEPPER1_1 = 2;
const int PIN_STEPPER1_2 = 4;
const int PIN_STEPPER1_3 = 3;
const int PIN_STEPPER1_4 = 5;
// Steps per revolution [default 1500]
const int STEPPER_STEPS_PER_REV = 1500;

const int PIN_SERVO = 51;

// For the ultrasonic sensor
const int PIN_WATER_SENSOR_ECHO = 6;
const int PIN_WATER_SENSOR_TRIG = 13;
const int PIN_CUP_SENSOR_ECHO = 24;
const int PIN_CUP_SENSOR_TRIG = 26;

const int PIN_SWITCH = 53;

const int PIN_LED_R = 52;
const int PIN_LED_G = 50;
const int PIN_LED_B = 48;

// Message strings
const String MSG_ATTACH_TEABAG = "Attach Teabag";
const String MSG_DIPPING = "Dipping .......";
const String MSG_WAIT = "Please Wait";
const String MSG_DONE = "   Done!   ";
const String MSG_CLEANUP = "Remove Teabag";
const String MSG_THANK_YOU = "Thank You :) ";

const int TIMES_TO_DIP = 3;

static State activeState;
static State oldState;
static long currentTime;
static long previousTime;
int distanceToWater;
int distanceToCup;

// Initialize display
LiquidCrystal lcd(PIN_LCD_RS,
                  PIN_LCD_EN,
                  PIN_LCD_D4,
                  PIN_LCD_D5,
                  PIN_LCD_D6,
                  PIN_LCD_D7);

Stepper stepper1(STEPPER_STEPS_PER_REV,
                 PIN_STEPPER1_1,
                 PIN_STEPPER1_2,
                 PIN_STEPPER1_3,
                 PIN_STEPPER1_4);

Servo servo1;
Servo servo2;

SR04 waterSensor = SR04(PIN_WATER_SENSOR_ECHO,
                        PIN_WATER_SENSOR_TRIG);
                        
SR04 cupSensor = SR04(PIN_CUP_SENSOR_ECHO,
                      PIN_CUP_SENSOR_TRIG);

void setup() {
  activeState = State::INITIAL;
  
  // Serial out for debug
  Serial.begin(9600);

  // Set display's number of columns and rows
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
   
  // Set stepper speed at 20rpm
  stepper1.setSpeed(20);
  
  servo1.attach(PIN_SERVO);
  
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  // LOW means switch is pressed
  pinMode(PIN_SWITCH, INPUT_PULLUP); 
}

void loop() {
  switch (activeState) {
    case State::INITIAL:
      lcd.clear();
      
      lcd.print("AutoTea");
      stepper1.step(STEPPER_STEPS_PER_REV);

      //delay(3000);
      currentTime = millis();
      if (currentTime - previousTime >= 3000) {
        previousTime = currentTime;
        enterState(State::WAIT_FOR_BAG);
      }
        
      break;
    case State::WAIT_FOR_BAG:
      lcd.clear();
      
      // prompt user to place teabag
      lcd.print(MSG_ATTACH_TEABAG);

      if (digitalRead(PIN_SWITCH) == LOW) {
        enterState(State::DIPPING);
      }

      break;
      
    case State::DIPPING:
      lcd.clear();
      
      lcd.print(MSG_DIPPING);
      setLedColor(0, 255, 0);

      distanceToWater = waterSensor.Distance();
        
      currentTime = millis();
      if (currentTime - previousTime >= 3000) {
        lcd.print("Cup is ");
        lcd.print(distanceToWater);
        lcd.print("cm away");
        previousTime = currentTime;
      }
        
      if (distanceToWater > 3) {
        for (int pos = 0; pos <= 45; pos++) {
          servo1.write(pos);
          delay(15);
        }
      }
      
      for(int count = 0; count < TIMES_TO_DIP; count++) {
        stepper1.step(STEPPER_STEPS_PER_REV);
        setLedColor(0, 0, 255); // Blue Color
        stepper1.step(-STEPPER_STEPS_PER_REV);
        setLedColor(0, 255, 0); // Green Color
      }

      enterState(State::REMOVE_BAG);
      
      break;
    case State::REMOVE_BAG:
      lcd.clear();

      lcd.print(MSG_CLEANUP);

      if (digitalRead(PIN_SWITCH) == LOW) {
        enterState(State::DONE);
      }
    
      break;
    case State::DONE:
      lcd.clear();

      lcd.print(MSG_DONE);
      
      //delay(3000);
      currentTime = millis();
      if (currentTime - previousTime >= 3000) {
        previousTime = currentTime;
        enterState(State::INITIAL);
      }
      
      break;
  }
}

void enterState(State newState) {
  oldState = activeState;
  activeState = newState;
}

void setLedColor(int redValue, int greenValue, int blueValue) {  
  analogWrite(PIN_LED_R, redValue);
  analogWrite(PIN_LED_G, greenValue);
  analogWrite(PIN_LED_B, blueValue);
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
