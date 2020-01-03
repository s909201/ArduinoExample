/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
FileName: Meter.ino
ProjectName: LCM + Arduino Uno
FunctionDesc: 
CreateDate:
Version:
Author: Morgan
ModifyHistory:  2020'1'3
Remark:
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
#include <LCD4Bit_mod.h>
#include <stdlib.h>
#include <string.h>

//create object to control an LCD.
//number of lines in display=1
LCD4Bit_mod lcd = LCD4Bit_mod(2);

#define ADC_PIN 3
#define LCD_BackLight_CH 10
#define LCD_BL_DURATION 1000 // unit: ms

//Key message
char msgs[5][15] = {"Right Key OK ",
                    "Up Key OK    ",
                    "Down Key OK  ",
                    "Left Key OK  ",
                    "Select Key OK"};
int adc_key_val[5] = {30, 150, 360, 535, 760};
int NUM_KEYS = 5;
int adc_key_in;
int key = -1;
int oldkey = -1;

char blink[] = "                ";
char myStg[16];

volatile unsigned char TASK_LCD_BL_STEP;
volatile unsigned long TASK_LCD_BL_TIMER_START;
volatile unsigned char TASK_AD_TO_LCD_STEP;
volatile unsigned long TASK_AD_TO_LCD_TIMER_START;

float val;
// --------------------------------------------------------------
void setup()
{
    pinMode(LCD_BackLight_CH, OUTPUT);

    lcd.init();
    lcd.clear();
    lcd.printIn("Power Meter");

    // Setup Serial connection
    Serial.begin(115200);

    TASK_INIT();
}
// --------------------------------------------------------------
void loop()
{
    TASK_AD_TO_LCD();

    // Serial.print("Time: ");
    // time = millis();
    // Serial.println(time); //prints time since program started
    // delay(1000);

    // TASK_LCD_BL();
    /*
    adc_key_in = analogRead(0); // read the value from the sensor
    key = get_key(adc_key_in); // convert into key press

    if (key != oldkey) // if keypress is detected
    {
        delay(50);                  // wait for debounce time
        adc_key_in = analogRead(0); // read the value from the sensor

        key = get_key(adc_key_in); // convert into key press
        if (key != oldkey)
        {
            oldkey = key;
            if (key >= 0)
            {
                lcd.cursorTo(2, 0); //line=2, x=0

                val = (float)adc_key_in * 12.0 / 967.0;
                dtostrf(val, 4, 2, myStg); //4 is mininum width, 2 is precision
                myStg[4] = ' ';
                myStg[5] = 'O';
                myStg[6] = 'h';
                myStg[7] = 'm';

                for (int i = 0; i < strlen(myStg); i++)
                {
                    lcd.print(myStg[i]);
                }

                // Serial.println(adc_key_in);
            }
        }
    }

    //delay(1000);
    // digitalWrite(LCD_BackLight_CH, LOW);
    */
}
// --------------------------------------------------------------
// Convert ADC value to key number
int get_key(unsigned int input)
{
    int k;

    for (k = 0; k < NUM_KEYS; k++)
    {
        if (input < adc_key_val[k])
        {

            return k;
        }
    }

    if (k >= NUM_KEYS)
        k = -1; // No valid key pressed

    return k;
}
// --------------------------------------------------------------
void TASK_INIT()
{
    // LCD
    TASK_LCD_BL_STEP = 0;
    TASK_LCD_BL_TIMER_START = 0;
    TASK_AD_TO_LCD_STEP = 0;
    TASK_AD_TO_LCD_TIMER_START = 0;
    digitalWrite(LCD_BackLight_CH, HIGH); // High: enable, LOW: disable
}
// --------------------------------------------------------------
void TASK_LCD_BL()
{
    switch (TASK_LCD_BL_STEP)
    {
    case 0:
        TASK_LCD_BL_TIMER_START = millis();
        TASK_LCD_BL_STEP = 2;
        break;

    case 2:
        if ((millis() - TASK_LCD_BL_TIMER_START) > LCD_BL_DURATION)
        {
            digitalWrite(LCD_BackLight_CH, LOW); // High: enable, LOW: disable
            TASK_LCD_BL_TIMER_START = millis();
            TASK_LCD_BL_STEP = 4;
        }
        break;

    case 4:
        if ((millis() - TASK_LCD_BL_TIMER_START) > LCD_BL_DURATION)
        {
            digitalWrite(LCD_BackLight_CH, HIGH); // High: enable, LOW: disable
            TASK_LCD_BL_TIMER_START = millis();
            TASK_LCD_BL_STEP = 2;
        }
        break;
    }
}
// --------------------------------------------------------------
void TASK_AD_TO_LCD()
{
    switch (TASK_AD_TO_LCD_STEP)
    {
    case 0:
        TASK_AD_TO_LCD_TIMER_START = millis();
        TASK_AD_TO_LCD_STEP = 2;
        break;

    case 2:
        if ((millis() - TASK_LCD_BL_TIMER_START) > LCD_BL_DURATION)
        {
            adc_key_in = analogRead(5); // read the value from the sensor
            lcd.cursorTo(2, 0);         //line=2, x=0

            val = (float)adc_key_in * 12.0 / 967.0;
            // val = (float)adc_key_in / 88.0;
            // val = (float)adc_key_in / 101.0;
            // convert float to char array
            dtostrf(val, 4, 2, myStg); //4 is mininum width, 2 is precision
            myStg[4] = ' ';
            myStg[5] = 'O';
            myStg[6] = 'h';
            myStg[7] = 'm';
            myStg[8] = ' ';
            // convert int to char array
            sprintf(&myStg[9], "%04d", adc_key_in);

            // LCD display, only this way because of the lcd print function
            for (int i = 0; i < strlen(myStg); i++)
            {
                lcd.print(myStg[i]);
            }

            // get current time
            TASK_LCD_BL_TIMER_START = millis();
        }
        break;
    }
}
// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
// --------------------------------------------------------------
