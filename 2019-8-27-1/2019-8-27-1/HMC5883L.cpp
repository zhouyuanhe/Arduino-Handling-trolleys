#include"HMC5883L.h"
#include"Arduino.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd_in_HMC(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display
#define MagnetcDeclination 5.3 //合肥市2017年12月31日磁偏角，请根据情况自行百度 
#define CalThreshold 0   
#define address 0x1E



void HMC5883L::INIT()
{
  Serial.begin(9600);
  Wire.begin();
  
  Wire.beginTransmission(address); //开始通信  
  Wire.write(0x00); //选择配置寄存器A  
  Wire.write(0x70); //0111 0000b，具体配置见数据手册  
  Wire.endTransmission();  
   
  Wire.beginTransmission(address);  
  Wire.write(0x02); //选择模式寄存器  
  Wire.write(0x00); //连续测量模式:0x00,单一测量模式:0x01  
  Wire.endTransmission();  
}



void HMC5883L::getRawData(int* x ,int* y,int* z)//角度读取程序 
{  
  Wire.beginTransmission(address);  
  Wire.write(0x03); //从寄存器3开始读数据  
  Wire.endTransmission();  
  //每轴的数据都是16位的  
  Wire.requestFrom(address, 6);  
  if(6<=Wire.available()){  
    *x = Wire.read()<<8; //X msb，X轴高8位  
    *x |= Wire.read(); //X lsb，X轴低8位  
    *z = Wire.read()<<8; //Z msb  
    *z |= Wire.read(); //Z lsb  
    *y = Wire.read()<<8; //Y msb  
    *y |= Wire.read(); //Y lsb  
  }  
}  


float HMC5883L::calculateHeading(int* x ,int* y,int* z)//角度计算程序  
{  
  float headingRadians = atan2((double)((*y)-offsetY),(double)((*x)-offsetX));  
  
  // 保证数据在0-2*PI之间  
  if(headingRadians < 0)  
    headingRadians += 2*PI;  
   
  float headingDegrees = headingRadians * 180/M_PI;  
  headingDegrees += MagnetcDeclination; //磁偏角  
  headingDegrees -= error_angle;//路径规划的校准变量
  //保证数据在0-360之间
  while(headingDegrees>360||headingDegrees<0){
  if(headingDegrees > 360)  
    headingDegrees -= 360; 
  if(headingDegrees < 0)  
    headingDegrees += 360;   
  }
  return headingDegrees ;  
}  



void HMC5883L::calibrateMag()//手动校正程序  
{  
  int x,y,z; //三轴数据  
  int xMax, xMin, yMax, yMin, zMax, zMin;  
  //初始化  
  getRawData(&x,&y,&z);    
  xMax=xMin=x;  
  yMax=yMin=y;  
  zMax=zMin=z;  
  offsetX = offsetY = offsetZ = 0;  
   
  Serial.println("Starting Calibration......");  
  Serial.println("Please turn your device around in 20 seconds");  
   
  for(int i=0;i<200;i++)  
  {  
    getRawData(&x,&y,&z);  
    // 计算最大值与最小值  
    // 计算传感器绕X,Y,Z轴旋转时的磁场强度最大值和最小值  
    if (x > xMax)  
      xMax = x;  
    if (x < xMin )  
      xMin = x;  
    if(y > yMax )  
      yMax = y;  
    if(y < yMin )  
      yMin = y;  
    if(z > zMax )  
      zMax = z;  
    if(z < zMin )  
      zMin = z;  
   
    delay(100);  
   
    if(i%10 == 0)  
    {  
      lcd_in_HMC.clear();
      lcd_in_HMC.setCursor(0, 0);
      lcd_in_HMC.print(xMax);
      lcd_in_HMC.setCursor(4, 0);
      lcd_in_HMC.print(xMin);
      lcd_in_HMC.setCursor(8, 0);
      lcd_in_HMC.print(yMax);
      lcd_in_HMC.setCursor(12, 0);
      lcd_in_HMC.print(yMin);
      lcd_in_HMC.setCursor(0, 1);
      lcd_in_HMC.print(zMax);
      lcd_in_HMC.setCursor(4, 1);
      lcd_in_HMC.print(zMin);
      Serial.print("x:");  
      Serial.print(xMax);  
      Serial.print(" ");  
      Serial.print(xMin);
      Serial.print("   ");  
      
      Serial.print("y:");
      Serial.print(yMax);  
      Serial.print(" ");  
      Serial.print(yMin);
      Serial.print("   ");
      
      Serial.print("z:");
      Serial.print(zMax);  
      Serial.print(" ");  
      Serial.println(zMin);   
    }  
  }  
  //计算修正量  
  if(abs(xMax - xMin) > CalThreshold )  
    offsetX = (xMax + xMin)/2;  
  if(abs(yMax - yMin) > CalThreshold )  
    offsetY = (yMax + yMin)/2;  
  if(abs(zMax - zMin) > CalThreshold )  
    offsetZ = (zMax +zMin)/2;  
   
  Serial.print("offsetX:");  
  Serial.print("");  
  Serial.print(offsetX);  
  Serial.print(" offsetY:");  
  Serial.print("");  
  Serial.print(offsetY);  
  Serial.print(" offsetZ:");  
  Serial.print("");  
  Serial.println(offsetZ);  
   
  delay(500);    
}  
int HMC5883L::angleoutput()
{
  int n = 1;
  pinMode(25, INPUT);
  n = digitalRead(25);
  if (n > 0)return 0;
  
  getRawData(&x, &y, &z);
  now_angle = calculateHeading(&x, &y, &z);
  error_angle = now_angle - 180;
  return 1;
}
