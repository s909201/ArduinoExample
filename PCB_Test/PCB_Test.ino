/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
FileName: PCB_Test.ino
ProjectName:
FunctionDesc:
CreateDate:
Version:
Author: Morgan
ModifyHistory:	2022.11.23
Remark:

Pin Define:
- LCM : A0(Button), D4, D5, D6, D7, D8, D9, D10(Back Light)
- I2C : A5(SCL, PC5), A4(SDA, PC4)

- ADC Level:
Key   | A0 Value
------------------
RIGHT |   0 ~ 60
UP    |  61 ~ 200
DOWN  | 201 ~ 400
LEFT  | 401 ~ 600
SELECT| 601 ~ 800
<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

/***************************************************************************************************
** Declare all program constants                                                                  **
***************************************************************************************************/
const uint32_t SERIAL_SPEED{115200};   ///< Set the baud rate for Serial I/O
const uint8_t SPRINTF_BUFFER_SIZE{16}; ///< Buffer size for sprintf()
const uint8_t LED_PIN{13};             ///< Built-in Arduino green LED pin

#include <LiquidCrystal.h>
#include <Wire.h>

/***************************************************************************************************
** Declare global variables and instantiate classes                                               **
***************************************************************************************************/

// Arduino UNO
// #define I2C_SDA 27
// #define I2C_SCL 28

#define AddrU502 0x2D // MCP4651
#define AddrU207 0x3E // MCP45HV51
#define AddrU257 0x3F // MCP45HV51
#define AddrU110 0x74 // TCA9539

// LCD GPIO pin to Arduino
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;
LiquidCrystal lcd(pin_RS, pin_EN, pin_d4, pin_d5, pin_d6, pin_d7);

// Keypad
int adc_key_val[5] = {60, 200, 400, 600, 800};
int NUM_KEYS = 5;
int adc_key_in;
int key = -1;
int oldkey = -1;

// UART parameters
bool stringComplete = false;           // whether the string is complete
String inputString = "";               // a String to hold incoming data
char text_buffer[SPRINTF_BUFFER_SIZE]; ///< Buffer for sprintf()/sscanf()
uint8_t text_index = 0;                ///< Variable for buffer position

// EVENT
uint8_t LCDEvent; // bool
uint16_t LCDEventNo;

// State Machine
volatile unsigned char TASK_BTN_STEP;
volatile unsigned long TASK_BTN_TIMER;

// --------------------------------------------------------------
void setup()
{
  // Initialize
  pinMode(pin_BL, OUTPUT);

  // LCD
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("READY           ");

  // Serial
  Serial.begin(SERIAL_SPEED);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);

  // I2C initial
  Wire.begin(); // join i2c bus (address optional for master)

  TASK_INIT();
}
// --------------------------------------------------------------
void loop()
{
  TASK_Terminal();
  TASK_EVENT();
}
// --------------------------------------------------------------
void TASK_INIT()
{
  LCDEvent = 0;
  LCDEventNo = 0;
  stringComplete = false;

  TASK_BTN_STEP = 0;
  TASK_BTN_TIMER = 0;

  digitalWrite(pin_BL, HIGH); // Backlight, HIGH: Turn on, LOW: Turn off

  // Print_CMD_Format();
  Serial.println(F("Valid CMD Format: [Type] [CMD 1] [CMD 2]"));
  Serial.println(F("Example: a 0 0"));
  Serial.println("\nReady");
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
void TASK_BTN()
{
  switch (TASK_BTN_STEP)
  {
  case 0:
    TASK_BTN_STEP = 2;
    TASK_BTN_TIMER = millis();
    break;

  case 2:
    if (millis() - TASK_BTN_TIMER > 50)
    {
      adc_key_in = analogRead(0); // read the value from the sensor
      key = get_key(adc_key_in);  // convert into key press

      if (key != -1)
      {
        oldkey = key;
        TASK_BTN_STEP = 4;
        TASK_BTN_TIMER = millis();
      }
    }
    break;

  case 4:
    if (millis() - TASK_BTN_TIMER > 50)
    {
      adc_key_in = analogRead(0); // read the value from the sensor
      key = get_key(adc_key_in);  // convert into key press
      if (oldkey != key)          // release button
      {
        LCDEvent = 1;
        LCDEventNo = oldkey;
        TASK_BTN_STEP = 2;
      }
      TASK_BTN_TIMER = millis();
    }
    break;

  default:
    break;
  }
}
// --------------------------------------------------------------
void TASK_EVENT()
{
  if (LCDEvent)
  {
    LCDEvent = 0;
    lcd.setCursor(10, 0);
    switch (LCDEventNo)
    {
    case 0:
      lcd.print(F("Right "));
      Serial.println(F("Press Right"));
      break;

    case 1:
      lcd.print("Up    ");
      Serial.println(F("Press UP"));
      break;

    case 2:
      lcd.print(F("Down  "));
      Serial.println(F("Press Down"));
      break;

    case 3:
      lcd.print(F("Left  "));
      Serial.println(F("Press Left"));
      break;

    case 4:
      lcd.print(F("Select"));
      Serial.println(F("Press Select"));
      break;

    case 5:
      lcd.setCursor(0, 1);
      lcd.print(F("Back Light: On  "));
      Serial.println(F("Back Light: On"));
      Serial.println(F("Done"));
      break;

    case 6:
      lcd.setCursor(0, 1);
      lcd.print(F("Back Light: Off "));
      Serial.println(F("Back Light: Off"));
      Serial.println(F("Done"));
      break;

    case 7:
      lcd.setCursor(0, 1);
      lcd.print(F("I2C SCAN        "));
      // Serial.println(F("I2C SCAN"));
      break;

    case 8:
      lcd.setCursor(0, 1);
      lcd.print(F("UNKNOW CMD      "));
      // Serial.println(F("I2C SCAN"));
      break;

    case 9: // Clean 2nd Row
      lcd.setCursor(0, 1);
      lcd.print(F("                "));
      break;
    }
  }
}
// --------------------------------------------------------------
void Print_CMD_Format()
{
  // Serial.println(F("Unknown command"));
  Serial.println(F("Valid CMD Format: [Type] [CMD 1] [CMD 2]"));
  Serial.println(F("Example: a 0 0"));
  LCDEvent = 1;
  LCDEventNo = 8;
}
// --------------------------------------------------------------
void I2C_SCAN()
{
  byte error, address;
  int nDevices;
  Serial.println(F("I2C Scanning..."));
  nDevices = 0;
  for (address = 1; address < 127; address++)
  {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
      Serial.print(F("I2C device found at address 0x"));
      if (address < 16)
      {
        Serial.print(F("0"));
      }
      Serial.println(address, HEX);
      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print(F("Unknow error at address 0x"));
      if (address < 16)
      {
        Serial.print(F("0"));
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
  {
    Serial.println(F("No I2C devices found\n"));
  }
  else
  {
    Serial.println(F("Done\n"));
  }
}
// --------------------------------------------------------------
void TASK_Terminal()
{
  readCommand();
}
// --------------------------------------------------------------
void readCommand()
{
  // static char text_buffer[SPRINTF_BUFFER_SIZE]; ///< Buffer for sprintf()/sscanf()
  // static uint8_t text_index = 0;                ///< Variable for buffer position
  while (Serial.available()) // Loop while there is incoming serial data
  {
    text_buffer[text_index] = Serial.read(); // Get the next byte of data
    // keep on reading until a newline shows up or the buffer is full
    if (text_buffer[text_index] != '\n' && text_index < SPRINTF_BUFFER_SIZE)
    {
      text_index++;
    }
    else
    {
      text_buffer[text_index] = 0; // Add the termination character
      Serial.print(F("\nReceive: "));
      Serial.write(text_buffer); // debug
      Serial.print(F("\n"));

      if (text_buffer[0] == 'a' && text_buffer[1] == ' ') // [a x x]
      {
        // Serial.println(F("a class cmd"));

        if (text_buffer[2] == '0' && text_buffer[3] == ' ')
        {
          if (text_buffer[4] == '0') // [a 0 0]
          {
            // Serial.println(F("a 0 0"));
            I2C_SCAN();
            LCDEvent = 1;
            LCDEventNo = 7;
          }
          else if (text_buffer[4] == '1') // [a 0 1]
          {
            // Serial.println(F("a 0 1"));
            digitalWrite(pin_BL, HIGH); // Backlight, HIGH: Turn on, LOW: Turn off
            LCDEvent = 1;
            LCDEventNo = 5;
          }
          else if (text_buffer[4] == '2') // [a 0 2]
          {
            // Serial.println(F("a 0 2"));
            digitalWrite(pin_BL, LOW); // Backlight, HIGH: Turn on, LOW: Turn off
            LCDEvent = 1;
            LCDEventNo = 6;
          }
          else if (text_buffer[4] == '3') // [a 0 3]
          {
            READ_DEVICE_REG(AddrU502, 0x00);
            READ_DEVICE_REG(AddrU502, 0x10);
            READ_DEVICE_REG(AddrU207, 0x01);
            READ_DEVICE_REG(AddrU257, 0x01);

            READ_DEVICE_REG(AddrU110, 0x00);
            READ_DEVICE_REG(AddrU110, 0x01);
            READ_DEVICE_REG(AddrU110, 0x02);
            READ_DEVICE_REG(AddrU110, 0x03);
            READ_DEVICE_REG(AddrU110, 0x04);
            READ_DEVICE_REG(AddrU110, 0x05);
            READ_DEVICE_REG(AddrU110, 0x06);
            READ_DEVICE_REG(AddrU110, 0x07);

            Serial.println(F("Done"));
          }
          else
          {
            Serial.println(F("Unknown command"));
            Print_CMD_Format();
          }
        }
        else
        {
          Serial.println(F("Unknown command"));
          Print_CMD_Format();
        }
      }
      else if (text_buffer[0] == 'c' && text_buffer[1] == ' ') // [c x x]
      {
        // Serial.println("c class cmd");
        // case: [c 9 x]
        if (text_buffer[2] == '5' && text_buffer[3] == ' ' && text_buffer[4] == '0') // [c 5 0]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 5 0: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x02, 0x00);
          I2C_Write(AddrU110, 0x03, 0x00);
          I2C_Write(AddrU110, 0x06, 0xE0);
          I2C_Write(AddrU110, 0x07, 0x00);
          READ_DEVICE_REG(AddrU110, 0x02);
          READ_DEVICE_REG(AddrU110, 0x03);
          READ_DEVICE_REG(AddrU110, 0x06);
          READ_DEVICE_REG(AddrU110, 0x07);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '6' && text_buffer[3] == ' ' && text_buffer[4] == 'a') // [c 6 a]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 6 a: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x02, 0x04);
          READ_DEVICE_REG(AddrU110, 0x02);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '7' && text_buffer[3] == ' ' && text_buffer[4] == 'a') // [c 7 a]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 7 a: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x02, 0x08);
          READ_DEVICE_REG(AddrU110, 0x02);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '7' && text_buffer[3] == ' ' && text_buffer[4] == 'b') // [c 7 b]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 7 b: DONE     "));
          // I2C Ctrl
          READ_DEVICE_REG(AddrU110, 0x00);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '8' && text_buffer[3] == ' ' && text_buffer[4] == 'b') // [c 8 b]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 8 b: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0x10);
          I2C_Write(AddrU207, 0x01, 0xFF);
          READ_DEVICE_REG(AddrU110, 0x03);
          READ_DEVICE_REG(AddrU207, 0x01);

          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '8' && text_buffer[3] == ' ' && text_buffer[4] == 'c') // [c 8 c]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 8 c: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0x11);
          READ_DEVICE_REG(AddrU110, 0x03);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '8' && text_buffer[3] == ' ' && text_buffer[4] == 'd') // [c 8 d]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 8 d: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0x12);
          READ_DEVICE_REG(AddrU110, 0x03);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '8' && text_buffer[3] == ' ' && text_buffer[4] == 'e') // [c 8 e]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 8 e: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0x10);
          I2C_Write(AddrU207, 0x01, 0x7F);
          READ_DEVICE_REG(AddrU110, 0x03);
          READ_DEVICE_REG(AddrU207, 0x01);

          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '8' && text_buffer[3] == ' ' && text_buffer[4] == 'f') // [c 8 f]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 8 f: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0x00);
          READ_DEVICE_REG(AddrU110, 0x03);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '8' && text_buffer[3] == ' ' && text_buffer[4] == 'g') // [c 8 g]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 8 g: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0xC0);
          READ_DEVICE_REG(AddrU110, 0x03);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '8' && text_buffer[3] == ' ' && text_buffer[4] == 'h') // [c 8 h]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 8 h: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0x80);
          READ_DEVICE_REG(AddrU110, 0x03);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '9' && text_buffer[3] == ' ' && text_buffer[4] == 'b') // [c 9 b]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 9 b: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0x40);
          I2C_Write(AddrU257, 0x01, 0xFF);
          READ_DEVICE_REG(AddrU110, 0x03);
          READ_DEVICE_REG(AddrU257, 0x01);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '9' && text_buffer[3] == ' ' && text_buffer[4] == 'c') // [c 9 c]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 9 c: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0x44);
          READ_DEVICE_REG(AddrU110, 0x03);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '9' && text_buffer[3] == ' ' && text_buffer[4] == 'd') // [c 9 d]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 9 d: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0x48);
          READ_DEVICE_REG(AddrU110, 0x03);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '9' && text_buffer[3] == ' ' && text_buffer[4] == 'e') // [c 9 e]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 9 e: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0x40);
          READ_DEVICE_REG(AddrU110, 0x03);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '9' && text_buffer[3] == ' ' && text_buffer[4] == 'f') // [c 9 f]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 9 f: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0x00);
          READ_DEVICE_REG(AddrU110, 0x03);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '9' && text_buffer[3] == ' ' && text_buffer[4] == 'g') // [c 9 g]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 9 g: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0x30);
          READ_DEVICE_REG(AddrU110, 0x03);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '9' && text_buffer[3] == ' ' && text_buffer[4] == 'h') // [c 9 h]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 9 h: DONE     "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x03, 0x20);
          READ_DEVICE_REG(AddrU110, 0x03);
          Serial.println(F("Done"));
        }
        // ------------------------------------------------------
        // case: [c 10 x]
        else if (text_buffer[2] == '1' && text_buffer[3] == '0' && text_buffer[4] == ' ' && text_buffer[5] == 'b') // [c 10 b]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 10 b: DONE    "));
          // I2C Ctrl
          I2C_Write(AddrU207, 0x01, 0xFF);
          I2C_Write(AddrU257, 0x01, 0xFF);
          I2C_Write(AddrU110, 0x02, 0x0E);
          I2C_Write(AddrU110, 0x03, 0x50);
          READ_DEVICE_REG(AddrU207, 0x01);
          READ_DEVICE_REG(AddrU257, 0x01);
          READ_DEVICE_REG(AddrU110, 0x02);
          READ_DEVICE_REG(AddrU110, 0x03);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '1' && text_buffer[3] == '0' && text_buffer[4] == ' ' && text_buffer[5] == 'c') // [c 10 c]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 10 c: DONE    "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x02, 0x08);
          READ_DEVICE_REG(AddrU110, 0x02);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '1' && text_buffer[3] == '1' && text_buffer[4] == ' ' && text_buffer[5] == 'a') // [c 11 a]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 11 a: DONE    "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x02, 0x04);
          READ_DEVICE_REG(AddrU110, 0x02);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '1' && text_buffer[3] == '1' && text_buffer[4] == ' ' && text_buffer[5] == 'f') // [c 11 f]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 11 f: DONE    "));
          // I2C Ctrl
          I2C_Write(AddrU502, 0x10, 0x1F);
          READ_DEVICE_REG(AddrU502, 0x10);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '1' && text_buffer[3] == '1' && text_buffer[4] == ' ' && text_buffer[5] == 'g') // [c 11 g]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 11 g: DONE    "));
          // I2C Ctrl
          I2C_Write(AddrU502, 0x10, 0x7F);
          READ_DEVICE_REG(AddrU502, 0x10);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '1' && text_buffer[3] == '1' && text_buffer[4] == ' ' && text_buffer[5] == 'l') // [c 11 l]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 11 l: DONE    "));
          // I2C Ctrl
          I2C_Write(AddrU502, 0x00, 0x1F);
          READ_DEVICE_REG(AddrU502, 0x00);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '1' && text_buffer[3] == '1' && text_buffer[4] == ' ' && text_buffer[5] == 'm') // [c 11 m]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 11 m: DONE    "));
          // I2C Ctrl
          I2C_Write(AddrU502, 0x00, 0x7F);
          READ_DEVICE_REG(AddrU502, 0x00);
          Serial.println(F("Done"));
        }
        else if (text_buffer[2] == '1' && text_buffer[3] == '3' && text_buffer[4] == ' ' && text_buffer[5] == '0') // [c 13 0]
        {
          lcd.setCursor(0, 1);
          lcd.print(F("c 13 0: DONE    "));
          // I2C Ctrl
          I2C_Write(AddrU110, 0x02, 0x00);
          READ_DEVICE_REG(AddrU110, 0x02);
          Serial.println(F("Done"));
        }
        else
        {
          Serial.println(F("Unknown command"));
          Print_CMD_Format();
        }
      }
      else
      {
        Serial.println(F("Unknown command"));
        Print_CMD_Format();
      }

      text_index = 0; // reset the counter

    } // of if-then-else we've received full command

  } // of if-then there is something in our input buffer
} // of method readCommand
// --------------------------------------------------------------
// --------------------------------------------------------------
void SHOW_I2C_2nd_Data(uint8_t device, int Reg)
{
  uint8_t dummy;
  Serial.print(F("[RegAddr]DATA: "));
  Serial.print(F("0x"));
  dummy = I2C_Read_2ndBYTE(device, Reg); // dummy, for pre-Write Register_Address for next Read action
  Serial.print(I2C_Read_2ndBYTE(device, Reg), HEX);
}
// --------------------------------------------------------------
// --------------------------------------------------------------
void READ_DEVICE_REG(int device, int REG)
{
  // Serial.println(F("TCA9539(U110):"));
  uint8_t data = 0;
  switch (device)
  {
  case AddrU502:
    Serial.print(F("MCP4651(U502):  "));
    break;
  case AddrU207:
    Serial.print(F("MCP45HV51(U207):"));
    break;
  case AddrU257:
    Serial.print(F("MCP45HV51(U257):"));
    break;
  case AddrU110:
    Serial.print(F("TCA9539(U110):  "));
    break;
  default:
    Serial.println(F("Device Error! 1"));
    break;
  }
  if (REG < 0x10)
  {
    Serial.print(F("[0x0"));
  }
  else
  {
    Serial.print(F("[0x"));
  }
  Serial.print(REG, HEX);
  Serial.print(F("]= "));

  // data = I2C_Read_1stBYTE(device, REG);
  // data = I2C_Read_2ndBYTE(device, REG);

  switch (device)
  {
  case AddrU502:
    data = I2C_Read_2ndBYTE(device, REG);
    break;
  case AddrU207:
    data = I2C_Read_2ndBYTE(device, REG);
    break;
  case AddrU257:
    data = I2C_Read_2ndBYTE(device, REG);
    break;
  case AddrU110:
    data = I2C_Read_1stBYTE(device, REG);
    break;
  default:
    Serial.println(F("Device Error! 2"));
    break;
  }

  if (data < 0x10)
  {
    Serial.print(F("0x0"));
  }
  else
  {
    Serial.print(F("0x"));
  }
  Serial.println(data, HEX);
}
// --------------------------------------------------------------

// --------------------------------------------------------------
// I2C_Write(Device Address, Register, Data)
void I2C_Write(uint8_t device, uint8_t address, uint8_t value)
{
  uint8_t temp = 0;
  Wire.beginTransmission(device);   // transmit to device
  temp = Wire.write(byte(address)); // Reg address
  temp = Wire.write(byte(value));
  if (Wire.endTransmission()) // stop transmitting
  {
    Serial.println(F("I2C no ACK"));
  }
}
// --------------------------------------------------------------

// --------------------------------------------------------------
uint8_t I2C_Read_1stBYTE(int device, int address)
{
  uint8_t data;
  Wire.beginTransmission(device);
  Wire.write(byte(address));
  Wire.endTransmission(); // stop transmitting
  Wire.requestFrom(device, 1);
  data = Wire.read(); // Read 1 Byte data
  return data;
}
// --------------------------------------------------------------
uint8_t I2C_Read_2ndBYTE(int device, int address)
{
  uint8_t data, dummy;
  Wire.beginTransmission(device);
  Wire.write(byte(address));
  Wire.endTransmission();

  Wire.requestFrom(device, 2);
  dummy = Wire.read(); // ignore D8
  data = Wire.read(); // D0~D7
  return data;
}
// --------------------------------------------------------------
