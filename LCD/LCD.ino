#include <LiquidCrystal.h>

// Display pins to board pins
const int LCD_RS = 12,
          LCD_EN = 11,
          LCD_D4 = 10,
          LCD_D6 = 8,
          LCD_D7 = 7;

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

}

void loop() {
  // Turn on display
  lcd.display();

  // Print a message to display
  lcd.print("fml");
  
}
