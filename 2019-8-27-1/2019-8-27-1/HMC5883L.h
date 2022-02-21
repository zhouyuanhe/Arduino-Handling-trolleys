#ifndef _HMC5883L_H__
#define _HMC5883L_H__
#include <Wire.h>
#include"Arduino.h"

class HMC5883L
{
  private:
    int address;
    int x;
    int y;
    int z;
    int offsetX,offsetY,offsetZ;  
    float angle;
    float a[10];
  public:
    float now_angle;
    int correct;//小车车头朝向变量
    float error_angle = 0;//校准角度
    void INIT();
    void calibrateMag();//初始化
    void getRawData(int* x ,int* y,int* z) ;//更新xyz
    float calculateHeading(int* x ,int* y,int* z) ;//返回当前角度朝向
    int angleoutput();//按下按钮记录当前角度并保存到now_angle里,并把小车内设磁场和实际磁场进行校准
    void angleinit(){//更新x,y,z和角度
      getRawData(&x, &y, &z);
      correct=calculateHeading(&x, &y, &z);
      }
      
};
#endif
