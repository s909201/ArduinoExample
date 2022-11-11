unsigned int count = 0;
int counterPin = 2;
unsigned long time2;
unsigned int rpm;
unsigned int grid_num = 20;

void counter()
{
  count++;
}

void setup()
{
  Serial.begin(115200);
  pinMode(counterPin, INPUT);
  attachInterrupt(0, counter, FALLING);
  count = 0;
  rpm = 0;
  time2 = 0;
}

void loop()
{
  if (millis() - time2 >= 1000)
  { /* 每秒更新 */

    // 計算 rpm 時，停止計時
    detachInterrupt(0);

    // 偵測的格數count * (60 * 1000 / 一圈網格數20）/ 時間差)
    rpm = (60 * 1000 / grid_num) / (millis() - time2) * count;
    time2 = millis();
    count = 0;

    // 輸出至Console
    Serial.print("RPM = ");
    Serial.println(rpm, DEC);
    // Restart the interrupt processing
    attachInterrupt(0, counter, FALLING);
  }
}