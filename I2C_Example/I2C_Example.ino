// I2C_Example
// DAMP IC - MA12070, I2C interface
// Created by Morgan, 2019'10'22
#include <Wire.h>

const byte enableOutPin = 5;
const byte muteOutPin = 4;
const byte muteInPin = 2; // GPIO input

// GPIO parameters
#define AMP_ENABLE enableOutPin // PD5, pin 9
#define AMP_MUTE muteOutPin     // PD4, pin 2
#define DANTE_MUTE muteInPin    // PD2, pin 32, input
// UART parameters
bool stringComplete = false; // whether the string is complete
char inChar;
// --------------------------------------------------------------
void setup()
{
    // put your setup code here, to run once:
    pinMode(enableOutPin, OUTPUT);
    pinMode(muteOutPin, OUTPUT);
    pinMode(muteInPin, INPUT_PULLUP);
    Wire.begin(); // join i2c bus (address optional for master)

    // initialize serial:
    Serial.begin(115200);

    // GPIO inital state
    digitalWrite(AMP_MUTE, LOW);    // default to amp muted
    digitalWrite(AMP_ENABLE, HIGH); // default to amp disabled
}
// --------------------------------------------------------------
void loop()
{
    // UART Control
    if (stringComplete)
    {
        switch (inChar)
        {
        case '1':
            digitalWrite(AMP_ENABLE, LOW);
            // Initial MA12070
            // Configure amp A
            delay(10);
            Wire.beginTransmission(33); // transmit to device #33 (0x21)
            Wire.write(byte(0x35));     // MA12070P PCM word format register address
            Wire.write(byte(0x00));     // configure for I2S operation
            Wire.endTransmission();     // stop transmitting
            delay(10);
            // Configure amp B
            Wire.beginTransmission(34); // transmit to device #34 (0x22)
            Wire.write(byte(0x35));     // MA12070P PCM word format register address
            Wire.write(byte(0x00));     // configure for I2S operation
            Wire.endTransmission();     // stop transmitting
            delay(10);
            Serial.print("AMP ENABLE - ON\r\n");
            break;
        case '2':
            digitalWrite(AMP_ENABLE, HIGH);
            Serial.print("AMP ENABLE - OFF\r\n");
            break;
        case '3':
            digitalWrite(AMP_MUTE, LOW);
            Serial.print("AMP MUTE - ON\r\n");
            break;
        case '4':
            digitalWrite(AMP_MUTE, HIGH);
            Serial.print("AMP MUTE - OFF\r\n");
            break;
        }
        stringComplete = false;
    }
}
// --------------------------------------------------------------
void serialEvent()
{
    while (Serial.available())
    {
        // get the new byte:
        inChar = (char)Serial.read();
        if (inChar != NULL)
        {
            stringComplete = true;
        }
    }
}
// --------------------------------------------------------------
