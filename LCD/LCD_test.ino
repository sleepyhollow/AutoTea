// https://www.arduino.cc/en/Reference/LiquidCrystal
#include <LiquidCrystal.h>

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
15(Bkl+)          	                              +5V
16(Bkl-)                                          GND

*/

// Assign display pins to board pins
const int LCD_RS = 12,
          LCD_EN = 11,
          LCD_D4 = 5,
          LCD_D5 = 4,
          LCD_D6 = 3,
          LCD_D7 = 2;

// Message strings
const String MSG_ATTACH_TEABAG = "Attach teabag";
const String MSG_DIPPING = "Dipping";
const String MSG_WAIT = "Please wait";
const String MSG_DONE = "Done";
const String MSG_CLEANUP = "Remove teabag";

// Initialize display
LiquidCrystal lcd(LCD_RS,
                  LCD_EN,
                  LCD_D4,
                  LCD_D5,
                  LCD_D6,
                  LCD_D7);

void setup() {
  // Set display's number of columns and rows
  lcd.begin(16, 2);
  // Print a message to display
  lcd.print(MSG_DIPPING);        
}

void loop() {
  // Show display content
  //lcd.display();

  

  
  // Hide display content
  //lcd.noDisplay();
  
  delay(1000);
  scrollText(MSG_ATTACH_TEABAG);
          
  // Set cursor to column and row number
  //lcd.setCursor(0, 0);
  // Print numbe of secs since reset
  //lcd.print(millis() / 1000);
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
