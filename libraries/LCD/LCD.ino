#include <LiquidCrystal.h>

/* LiquidCrystal display with:
LCD 4 (RS) to arduino pin 12
LCD 5 (R/W) to ground (pin 14)
LCD 6 (E) to arduino pin 11
LCD DB4, DB5, DB6, DB7 on arduino pins 7, 8, 9, 10 */

LiquidCrystal lcd(22,23,24,25,26,27,28);

void setup()
{
 lcd.begin(20,4);
 lcd.clear();
 lcd.setCursor(2,0); // Position cursor on line x=3,y=1
 lcd.print("emre"); // Print a message to the LCD
 lcd.setCursor(2,1);
 lcd.print("cemal");
 lcd.setCursor(2,2);
 lcd.print("burak");
 }

void loop()
{
}
