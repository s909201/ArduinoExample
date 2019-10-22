/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
  Created by Morgan, 2019'10'22
 */

#define LED1 13 // D13, pin 17, High enable
#define LED2 0  // D0/Tx, pin 31, Low enable
#define LED3 1  // D1/Rx, pin 30, Low enable
#define SPEED 200

#define LOW_ON  LOW
#define LOW_OFF HIGH


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin PB1 as an output.
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED1, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED2, LOW_OFF);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(LED3, LOW_OFF);   // turn the LED on (HIGH is the voltage level)
  delay(SPEED);              // wait for a second
  digitalWrite(LED1, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(LED2, LOW_ON);    // turn the LED off by making the voltage LOW
  digitalWrite(LED3, LOW_OFF);   // turn the LED on (HIGH is the voltage level)
  delay(SPEED);              // wait for a second
  digitalWrite(LED1, LOW);    // turn the LED off by making the voltage LOW
  digitalWrite(LED2, LOW_OFF);    // turn the LED off by making the voltage LOW
  digitalWrite(LED3, LOW_ON);   // turn the LED on (HIGH is the voltage level)
  delay(SPEED);              // wait for a second
}
