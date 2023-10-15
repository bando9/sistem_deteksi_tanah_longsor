#include <Wire.h>  // include library
#include<LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
    lcd.init();                     
    // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);    //kolom dan baris
  lcd.print("SINAU PROGRAMING");
}

void loop()
{
 
} 