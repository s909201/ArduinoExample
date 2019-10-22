/*
  Serial Event example
  created by Morgan, 2019'10'22
*/

String inputString = "";     // a String to hold incoming data
bool stringComplete = false; // whether the string is complete
char inChar;

void setup()
{
    // initialize serial:
    Serial.begin(115200);
    // reserve 200 bytes for the inputString:
    inputString.reserve(200);
}

void loop()
{
    // print the string when a newline arrives:
    if (stringComplete)
    {
        switch (inChar)
        {
        case '1':
            Serial.print("Press 1");
            break;
        case '2':
            Serial.print("Press 2");
            break;
        case '3':
            Serial.print("Press 3");
            break;
        case '4':
            Serial.print("Press 4");
            break;
        }
        stringComplete = false;
    }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
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
