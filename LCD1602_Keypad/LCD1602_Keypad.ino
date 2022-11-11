/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
FileName: LCD1602_Keypad.ino
ProjectName: 
FunctionDesc: 
CreateDate:
Version:
Author: Morgan
ModifyHistory:	2022.11.11
Remark:
- ADC Level:
Key   | A0 Value
------------------
RIGHT |   0 ~ 60
UP    |  61 ~ 200
DOWN  | 201 ~ 400
LEFT  | 401 ~ 600
SELECT| 601 ~ 800

<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
#include <LiquidCrystal.h>
// LCD pin to Arduino
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;
LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);
void setup()
{
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Elytone PCB Test");
  lcd.setCursor(0, 1);
  lcd.print("Press Key:");
}
void loop()
{
  int x;
  x = analogRead(0);
  lcd.setCursor(10, 1);
  if (x < 60)
  {
    lcd.print("Right ");
  }
  else if (x < 200)
  {
    lcd.print("Up    ");
  }
  else if (x < 400)
  {
    lcd.print("Down  ");
  }
  else if (x < 600)
  {
    lcd.print("Left  ");
  }
  else if (x < 800)
  {
    lcd.print("Select");
  }
}
