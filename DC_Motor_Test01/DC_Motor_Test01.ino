// Install: Adafruit motor shield library

#include <AFMotor.h> // 本程序中使用AFMotor庫

AF_DCMotor motor1(1); // 這4條語句的作用是建立4個直流電機對象，它們的名稱分別是：motor1/2/3/4.
AF_DCMotor motor2(2); // 這四條語句括號中的數字代表各個電機對象所連接在AFMotor擴展板的電機端口號碼。
AF_DCMotor motor3(3); // AF_DCMotor motor1(1); 代表motor1對象連接在AFMotor擴展板的M1端口上。
AF_DCMotor motor4(4); // AFMotor電機擴展板最多可以驅動4個直流電機，最少可以驅動1個直流電機。

void setup()
{
  Serial.begin(115200); // set up Serial library at 115200 bps
  Serial.println("Motor test!");
  motor1.setSpeed(255); // 這4條語句的作用是通過setSpped庫函數設置電機運行速度。
  motor2.setSpeed(255); // setSpped庫函數中的參數是運行速度參數。
  motor3.setSpeed(255); // 運行速度參數允許範圍是0～255。
  motor4.setSpeed(255); // 速度參數越大，運轉速度越快。參數為0時電機停止轉動。

  motor1.run(RELEASE); // 這4條語句的作用是讓4個電機在啟動時停止轉動
  motor2.run(RELEASE); // run庫函數允許使用的關鍵字參數有RELEASE、FORWARD、BACKWARD
  motor3.run(RELEASE); // 使用關鍵字RELEASE作為參數使用時，run庫函數將會讓擴展板停止提供電機運轉動力
  motor4.run(RELEASE); // 電機旋轉一旦失去動力就會自然的停止轉動。
}

void loop()
{
  /*
  motor1.run(FORWARD); // 這4條語句的作用是利用run庫函數控制4個電機"正向"旋轉。
  motor2.run(FORWARD); // 這里所謂的“正向”旋轉只是一種說法，假如您發現電機旋轉方向和您所要的“正向”不一致。
  motor3.run(FORWARD); // 您可以將電機的兩個引線從擴展板上斷開，然後交換順序再接到擴展板接口上
  motor4.run(FORWARD); // 這時您會看到同樣使用FORWARD關鍵字作為run庫函數的參數，電機的旋轉方向卻反過來了。

  for (int i = 0; i <= 255; i++)
  {                     // 這里使用for循環語句控制4個電機速度由停止逐步加速，最終電機運轉達到最大速度。
    motor1.setSpeed(i); // 在for循環語句的作用下,setSpeed庫函數的速度參數i由0逐漸增大，最終達到255。
    motor2.setSpeed(i); // 因此電機運行速度也是由停止逐漸加速，最終達到最大速度。
    motor3.setSpeed(i); // 對於一些模型電機來說，當速度參數小於一定數值以後就不能轉動了。也就是說，也許您的電機
    motor4.setSpeed(i); // 在速度參數在小於某一個速度參數數值的時候就無法轉動了。這屬於正常現象。
    delay(10);          // 具體這個最小值是多少，對於不同的電機來說是不同的。有的可能是50也有的可能是100。
  }                     // 換句話說，很可能您的某一個直流電機在速度參數小於50的情況下就無法轉動了。
                        // 也可能您的另一個直流電機在參數100以下的情況下就無法轉動了。

  for (int i = 255; i >= 0; i--)
  {                     // 這里使用for循環語句控制4個電機由最大旋轉速度逐步減速最終停止旋轉。
    motor1.setSpeed(i); // 這一系列語句的操作與上一段for循環語句類似。唯一區別是上一段for循環控制速度參數i由0漲到255
    motor2.setSpeed(i); // 而這一段語句控制速度參數i由255減小到0。同樣您可能會發現當速度參數沒有減小到零的時候，電機就已經
    motor3.setSpeed(i); // 停止旋轉了。這其中的原因在上一段for循環語句中已經介紹了。不在這里贅述了。
    motor4.setSpeed(i);
    delay(10);
  }

  motor1.run(BACKWARD); // 這4條語句的作用是利用run庫函數控制4個電機"反向"旋轉。
  motor2.run(BACKWARD); // 同樣的，這里所謂的“反向”旋轉也只是一種說法。這部分程序內容
  motor3.run(BACKWARD); // 與本程序33-36行中的內容十分類似。唯一區別就是使用了“BACKWARD”
  motor4.run(BACKWARD); // 關鍵字參數來通過庫函數run控制電機“反向”旋轉。

  for (int i = 0; i <= 255; i++)
  {                     // 利用for循環語句控制速度參數i由小到大
    motor1.setSpeed(i); // 電機也會以“反向”旋轉的方式由停止逐步達到最大速度
    motor2.setSpeed(i);
    motor3.setSpeed(i);
    motor4.setSpeed(i);
    delay(10);
  }

  for (int i = 255; i >= 0; i--)
  {                     // 利用for循環語句控制速度參數i由大到小
    motor1.setSpeed(i); // 電機也會以“反向”旋轉的方式由最大速度逐步減小到停止
    motor2.setSpeed(i);
    motor3.setSpeed(i);
    motor4.setSpeed(i);
    delay(10);
  }

  motor1.run(RELEASE); // 這四條語句作用是使用關鍵字RELEASE作為run函數的參數。
  motor2.run(RELEASE); // 在這種情況下，AFMotor擴展板將會停止為電機旋轉提供動力。
  motor3.run(RELEASE); // 電機也就會自然的停止轉動。
  motor4.run(RELEASE); // 本段程序後面的delay(1000)的作用就是讓4個電機保持無旋轉動力狀態1秒鐘

  delay(1000);
  */

  // motor3.setSpeed(255);

  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(2000);

  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(500);

  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(2000);

  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  delay(500);


}
