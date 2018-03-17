#include <LiquidCrystal.h>

// Display pins to board pins
const int LCD_RS = 12,
          LCD_EN = 11,
          LCD_D4 = 5,
          LCD_D5 = 4,
          LCD_D6 = 3,
          LCD_D7 = 2;

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

  
  lcd.print("fml");
}

void loop() {
  // Turn on display
  //lcd.display();

  // Print a message to display
  //lcd.print("fml");

  // Set cursor to column and row number
  lcd.setCursor(0, 1);
  // Print numbe of secs since reset
  lcd.print(millis() / 1000);
}
