#include "car.h"
#include "Floyd.h"
#include "HMC5883L.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include "DFRobot_TCS34725.h"
DFRobot_TCS34725 tcs = DFRobot_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
#define PIN_SERVO 10 dd
#include<Stepper.h>
const int STEPS = 65;
int i = 0, color;
Stepper stepper(STEPS, 50, 52, 51, 53);
Servo myservo1;
Servo myservo2;
Servo myservo3;
Servo myservo4;
//黑红绿蓝白
int material[5]; 
const int TrigPin = 46; //发出超声波
const int EchoPin = 47; //收到反射回来的超声波
float cm; //因为测得的距
void keep()
{
  myservo1.attach(10);
  myservo2.attach(11);
  myservo3.attach(41 );
  myservo4.attach(12);
  stepper.setSpeed(100);
  //Serial.begin(9600);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  //  if (tcs.begin()) {
  //    Serial.println("Found sensor");
  //  } else {
  //    Serial.println("No TCS34725 found ... check your connections");
  //    while (1); // halt!
  //
  //  }
}
int origin()
{
  myservo1.write(40);
  myservo2.write(100);
  myservo3.write(0);
  myservo4.write(30);
  return 0;
}
void putdown()
{
  myservo1.write(0);
  for (int i = 100; i <= 180; i++)
  {
    myservo2.write(i);
    delay(10);
  }
  delay(1000);
  myservo4.write(140);
  delay(500);
}
void putup()
{
  for (int i = 180; i >= 100; i--)
  {
    myservo2.write(i);
    delay(10);
  }
  myservo1.write(40);
}
void catchup1()
{
  myservo2.write(90);
  for (int i = 90; i <= 120; i++)
  {
    myservo1.write(i);
    delay(10);
  }
}
void putdown1()
{
    myservo2.write(90);
    for(int i=120;i>=40;i--)
    {
      myservo1.write(i);
      delay(10);  
    }
}
void catchdown()
{
  myservo2.write(80);
  for (int i = 120; i > 80; i--)
  {
    myservo1.write(i);
    delay(10);
  }
  myservo4.write(30);
}
void turnback()
{
  for (int i = 0; i <= 165; i++)
  {
    myservo3.write(i);
    delay(10);
  }
}
void turnfoward()
{
  for (int i = 165; i >= 0; i--)
  {
    myservo3.write(i);
    delay(10);
  }
}
int colorcheck()
{
  uint16_t clear, red, green, blue;
  tcs.unlock();      // turn on LED
  delay(60);  // takes 50ms to read
  tcs.getRGBC(&red, &green, &blue, &clear);
  tcs.lock();  // turn off LED
  Serial.print("C:\t"); Serial.print(clear);
  Serial.print("\tR:\t"); Serial.print(red);
  Serial.print("\tG:\t"); Serial.print(green);
  Serial.print("\tB:\t"); Serial.print(blue);
  Serial.println("\t");
  if (clear > 800)//白
    return 0;
  if (clear < 200 && red < 100)//黑
    return 1;
  if (red > green && red > blue && clear < 800 && clear > 200 && red > 100)//红
    return 2;
  if (green > red && green > blue && clear < 800 && clear > 200)//绿
    return 3;
  if (blue > red && blue > green && clear < 800 && clear > 200)//蓝
    return 4;
}
void catchup2()
{
  origin();
  delay(1000);
  catchup1();
  delay(1000);
  turnback();
  delay(1000);
  myservo4.write(30);
  catchdown();
  delay(1000);
  myservo4.write(140);
  delay(1000);
  catchup1();
  delay(1000);
}
void input(int i)
{
  origin();
  delay(1000);
  putdown();
  delay(1000);
  putup();
  delay(1000);
  catchup1();
  delay(1000);
  turnback();
  delay(1000);
  catchdown();
  delay(1000);
  catchup1();
  delay(1000);
  turnfoward();
  delay(1000);
  origin();
  stepper.step(435);
  material[i] = colorcheck();
}
void output(int b)
{ int a;
  for (int i = 0; i < 5; i++)
  {
    if (material[i] == b)
    {
      a = i;
      break;
    }
  }
  if (a == 0)
  {
    catchup2();
    turnfoward();
    delay(500);
    putdown1();
    putdown();
    myservo4.write(30);
  }
  if (a == 1)
  {
    stepper.step(435);
    catchup2();
    turnfoward();
    delay(500);
    putdown1();
    putdown();
    myservo4.write(30);
    stepper.step(-435);
  }
  if (a == 2)
  {
    stepper.step(870);
    catchup2();
    turnfoward();
    delay(500);
    putdown1();
    putdown();
    myservo4.write(30);
    stepper.step(-870);
  }
  if (a == 4)
  {
    stepper.step(-435);
    catchup2();
    turnfoward();
    delay(500);
    putdown1();
    putdown();
    myservo4.write(30);
    stepper.step(435);
  }
  if (a == 3)
  {
    stepper.step(-870);
    catchup2();
    turnfoward();
    delay(500);
    putdown1();
    putdown();
    myservo4.write(30);
    stepper.step(870);
  }
}
int ultrasonic()
{
  digitalWrite(TrigPin, LOW); //低高低电平发一个短时间脉冲去TrigPin
  delayMicroseconds(2);       // delayMicroseconds在更小的时间内延时准确
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW); //通过这里控制超声波的发射
  cm = pulseIn(EchoPin, HIGH) / 58.0; //将回波时间换算成cm
  cm = (int(cm * 100.0)) / 100.0; //保留两位小数
  Serial.print("Distance:");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  return cm;
}
Floyd G;
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
car a(2, 3, 4, 5, 6, 7, 8, 9); //初始化小车四个轮子的引脚
HMC5883L HMC;
int x, y, z;
int speeds;
int begin_flag = 0;
int flags = 0;
int old_flags = 0;
int lcd_print()//显示屏显示函数
{
  a.lcd_read();
  lcd.clear();
  for (int i = 0; i < 12; i++) {
    if (i < 6) {
      lcd.setCursor(2 * i, 0);
      lcd.print(char('A' + i));
      lcd.setCursor(2 * i + 1, 0);
      lcd.print(a.lcd[i]);
    }
    else if (i >= 6) {
      lcd.setCursor(0 + 2 * (i - 6), 1);
      lcd.print(char('A' + i));
      lcd.setCursor(1 + 2 * (i - 6), 1);
      lcd.print(a.lcd[i]);
    }
    Serial.print(char('A' + i));
    Serial.print(":");
    Serial.print(a.lcd[i]);
    Serial.print("|");
    Serial.print(a.lcd_s[i]);
    Serial.print("  ");
  }
  Serial.println("  ");
}

void turn(int angle)//转动angle角度，正为左转，负为右转
{
  HMC.getRawData(&x, &y, &z);
  int head = HMC.calculateHeading(&x, &y, &z) + angle;
  if (head >= 360)
    head = head - 360;
  if (head <= 0)
    head = head + 360;
  while (HMC.calculateHeading(&x, &y, &z) <= (head - 5) || HMC.calculateHeading(&x, &y, &z) >= (head + 5))
  {

    if (HMC.calculateHeading(&x, &y, &z) <= head)
      while (HMC.calculateHeading(&x, &y, &z) <= (head - 5))
      {
        lcd.clear();
        a.turnR(255);
        HMC.getRawData(&x, &y, &z);
        lcd.print(HMC.calculateHeading(&x, &y, &z));
        lcd.setCursor(0, 1);
        lcd.print("turnR");
      }
    else if (HMC.calculateHeading(&x, &y, &z) >= head)
      while (HMC.calculateHeading(&x, &y, &z) >= (head + 5))
      {
        lcd.clear();
        a.turnL(255);
        HMC.getRawData(&x, &y, &z);
        lcd.print(HMC.calculateHeading(&x, &y, &z));
        lcd.setCursor(0, 1);
        lcd.print("turnL");
      }
    else if (HMC.calculateHeading(&x, &y, &z) >= (head - 5) && HMC.calculateHeading(&x, &y, &z) <= (head + 5))
      break;
  }
  a.stop();
  delay(1000);
}
void Reachattitude(int attitude)//转向八向角度
{
  if (attitude == 0) {
    a.stop();
    return;
  }
  HMC.getRawData(&x, &y, &z);
  int head = attitude * 45 - 45;
  if (head >= 360)
    head = head - 360;
  if (head <= 0)
    head = head + 360;
  while (HMC.calculateHeading(&x, &y, &z) <= (head - 2.5) || HMC.calculateHeading(&x, &y, &z) >= (head + 2.5))
  {

    if (HMC.calculateHeading(&x, &y, &z) <= head)
      while (HMC.calculateHeading(&x, &y, &z) <= (head - 2.5))
      {

        a.turnR(255);
        HMC.getRawData(&x, &y, &z);
        //        lcd.clear();
        //        lcd.print(HMC.calculateHeading(&x, &y, &z));
        //        lcd.setCursor(0, 1);
        //        lcd.print("turnR");
      }
    else if (HMC.calculateHeading(&x, &y, &z) >= head)
      while (HMC.calculateHeading(&x, &y, &z) >= (head + 2.5))
      {
        a.turnL(255);
        HMC.getRawData(&x, &y, &z);
        //        lcd.clear();
        //        lcd.print(HMC.calculateHeading(&x, &y, &z));
        //        lcd.setCursor(0, 1);
        //        lcd.print("turnL");
      }
    else if (HMC.calculateHeading(&x, &y, &z) >= (head - 2.5) && HMC.calculateHeading(&x, &y, &z) <= (head + 2.5))
      break;
  }
}
void getway(int x, int y) //从节点a到节点b的路径和转向的规划
{
  int x_s, y_s, z_s;
  int a_flag = 0;
  for (int i = 0; i < 16; i++) {
    G.WhereNode[i] = 0;
    G.WhichNode[i] = 0;
  }
  G.FindWays(x, y);
  Serial.print("[G.nowNode]=");

  while (G.nowNode < G.numNode) {
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print(G.WhereNode[G.nowNode]);    //在显示屏的第一行显示下一个路口
    lcd.setCursor(6, 1);
    lcd.print(a_flag);                    //在显示屏第二行显示下一个路口的方向
    Serial.print(G.WhereNode[G.nowNode]);
    a.lcd_read();
    flags = a.MatchingPath();
    if (flags == 1) {
      G.nowNode++;
      a.stop();
      delay(500);
      a.runfoward(255);
      delay(2000);
      if (G.nowNode >= 1)               //防止下一步-1时出现内存误读
        if (G.WhichNode[G.nowNode - 1] == 0) //whichNode里记录的是下一个路口的位置，因此要 -1
          delay(2000);                  //如果是遇到米字形路口，额外延迟1秒
      Reachattitude(G.WhereNode[G.nowNode]);//转向
    }
    else if (flags == 0) a.followline();

  }
}


void catch_that() {
  a.runback(255);
  delay(1600);
//  float cm_s = ultrasonic();
//  a.runback(255);
//  delay(1000);
//  while (cm_s < 9 || cm_s > 10) {
//    if (cm_s > 10) {
//      a.runfoward(255);
//      delay(1);
//    }
//    else if (cm_s < 9) {
//      a.runback(255);
//      delay(1);
//    }
//    cm_s = ultrasonic();
//  }
  a.stop();

  return ;
}

void setup() {
  Serial.begin(9600);
  keep();
  Wire.begin();
  G.CreateMGraph();//创建邻接矩阵
  G.Create_Floyd();//创建佛罗伦萨最短路径矩阵
  HMC.INIT();
  HMC.calibrateMag();//手动初始化
  lcd.init();                  // 初始化LCD
  lcd.backlight();             //设置LCD背景等亮
  origin();
}

void loop() {
  //-----------开关---------
  if (begin_flag == 0) {
    origin();

    a.lcd_init();
    lcd_print();
    begin_flag = HMC.angleoutput();
    delay(100);
    return;
  }
  a.lcd_read();

  //------------------------
  //-------------------------
 
  a.runfoward(255);
  delay(3000);
  //出发，走出原点,前往12号节点取物块
  getway(9, 12);
  catch_that();     //取物块前，后退1.5秒，
  input(0);         //取第一个物块
  a.runfoward(255); //取物块之后前进一点距离，回到正常位置，防止检测不到路口
  delay(2500);
  Reachattitude(8);//转向
  a.stop();
  delay(1000);

  
  //前往14号节点取物块
  getway(12, 0);
  Reachattitude(6);//转向
  getway(0, 14);
  catch_that();     //取物块前，后退1.5秒，
  input(1);         //取第二个物块
  a.runfoward(255);//取物块之后前进一点距离，回到正常位置，防止检测不到路口
  delay(2500);
  Reachattitude(2);//转向
  a.stop();
  delay(1000);

  
  //前往16号节点取物块
  getway(14, 0);
  Reachattitude(8);//转向
  getway(0, 16);
  catch_that();     //取物块前，后退1.5秒，
  input(2);         //取第三个物块
  a.runfoward(255);//取物块之后前进一点距离，回到正常位置，防止检测不到路口
  delay(2500);
  Reachattitude(4);//转向
  a.stop();
  delay(1000);

  
  //前往10号节点取物块
  getway(16, 0);
  Reachattitude(2);//转向
  getway(0, 10);
  catch_that();     //取物块前，后退1.5秒，
  input(3);         //取第四个物块
  a.runfoward(255);//取物块之后前进一点距离，回到正常位置，防止检测不到路口
  delay(2500);
  Reachattitude(6);//转向
  a.stop();
  delay(1000);
  

  //前往5号路口，放置物块
  getway(10, 5);
  a.runback(255);
  delay(3700);
  a.stop();
  output(0);        //放置第一个物块（白色）
  a.runfoward(255);//放置物块之后前进一点距离，回到正常位置，防止检测不到路口
  delay(2000);      //------*********如果发生碰到物块，减小此处delay数值，不要小于1.5s
  Reachattitude(1);//转向
  a.stop();
  delay(1000);

  //前往7号路口，放置物块
  getway(5, 7);
  a.runback(255);
  delay(3700);
  a.stop();
  output(2);        //放置第二个物块（红色）
  a.runfoward(255);//放置物块之后前进一点距离，回到正常位置，防止检测不到路口
  delay(2000);      //------*********如果发生碰到物块，减小此处delay数值，不要小于1.5s
  Reachattitude(3);//转向
  a.stop();
  delay(1000);

  //前往3号路口，放置物块
  getway(1, 3);
  a.runback(255);
  delay(3700);
  a.stop();
  output(4);        //放置第三个物块（蓝色）
  a.runfoward(255);//放置物块之后前进一点距离，回到正常位置，防止检测不到路口
  delay(2000);      //------*********如果发生碰到物块，减小此处delay数值，不要小于1.5s
  Reachattitude(7);//转向
  a.stop();
  delay(1000);

  //前往1号路口，放置物块
  getway(7, 9);//一号路口放完之后就回家了，所以，车从九号位置，面一号位置放，方便回家
  Reachattitude(5);//转向
  getway(9, 1);
  a.runback(255);
  delay(3700);
  a.stop();
  output(3);        //放置第四个物块（绿色）
  a.runfoward(255);//放置物块之后前进一点距离，回到正常位置，防止检测不到路口
  delay(2000);          //------*********如果发生碰到物块，减小此处delay数值，不要小于1.5s
  Reachattitude(1);//转向
  a.stop();
  delay(1000);


  //前往9号路口，再向前一点距离，返回原点。
  getway(1, 9);
  a.runfoward(255);
  delay(3000);
  a.stop();
  delay(99999);//回到起点，任务一到此结束，请继续完善任务二的部分。
  //备注，如果没有开始任务二（夹取至少一个任务二的物块），回到起点是没有额外分数的。
  //---------------------------------

}
