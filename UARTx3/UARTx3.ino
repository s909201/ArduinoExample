/*
  UARTx3.ino
  MCU: STM32F103C8T6, 64KB Flash
  Refer: https://github.com/BLavery/STM32F103-Arduino
  Created by Morgan, 2019'11'07
 */

#define SPEED 500

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(115200);  // USB Virtual COM
    Serial1.begin(115200); // on PA9(Tx1), PA10(Rx1)
    Serial2.begin(115200); // on PA2(Tx2), PA3(Rx2)
    Serial3.begin(115200); // on PB10(Tx3), PB11(Rx3)
    pinMode(PC13, OUTPUT); // PC13 as GPIO/LED
}

void loop()
{
    // put your main code here, to run repeatedly:
    Serial.println("UART 0");
    Serial1.println("UART 1");
    Serial2.println("UART 2");
    Serial3.println("UART 3");
    digitalWrite(PC13, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(SPEED);             // wait for a second
    digitalWrite(PC13, LOW);  // turn the LED off by making the voltage LOW
    delay(SPEED);             // wait for a second
}
