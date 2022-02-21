
#include"car.h"

#include<arduino.h>
int car::measure(int a, int b)
{
  if (a == 1 && b == 2)
    return 1;
  else if (a == 0 && b == 2)
    return 2;
  else if (a == 4 && b == 2)
    return 3;
  else if (a == 5 && b == 2)
    return 4;
  else return -1;
}
void car::fix(int cwbsecond)
{
  PRECWB=cwbsecond;
}

car::car(int LA, int LB, int RA, int RB, int L1, int L2, int R1, int R2)//一些变量的初始化
{
  f = 0;
  k = 0;
  int leftA_PIN = LA;//记录引脚位置
  int leftB_PIN = LB;
  int righA_PIN = RA;
  int righB_PIN = RB;
  int leftA_B_PIN = L1;
  int leftB_B_PIN = L2;
  int righA_B_PIN = R1;
  int righB_B_PIN = R2;
  pinMode(leftA_PIN, OUTPUT);
  pinMode(leftB_PIN, OUTPUT);
  pinMode(righA_PIN, OUTPUT);
  pinMode(righB_PIN, OUTPUT);
  pinMode(leftA_B_PIN, OUTPUT);
  pinMode(leftB_B_PIN, OUTPUT);
  pinMode(righA_B_PIN, OUTPUT);
  pinMode(righB_B_PIN, OUTPUT);
}
void car::lcd_read()//初始化lcd的临界值
{
    lcd_s[0] = analogRead(A0) ;
    lcd_s[1] = analogRead(A1) ;
    lcd_s[2] = analogRead(A2) ;
    lcd_s[3] = analogRead(A3) ;
    lcd_s[4] = analogRead(A6) ;
    lcd_s[5] = analogRead(A11);
    lcd_s[6] = analogRead(A9) ;
    lcd_s[7] = analogRead(A10) ;
    lcd_s[8] = analogRead(A7) ;
    lcd_s[9] = analogRead(A8) ;
    lcd_s[10] = analogRead(A12) ;
    lcd_s[11] = analogRead(A13);
  for(int i=0;i<12;i++){
    if(lcd_s[i]>lcd_max[i])lcd_max[i]=lcd_s[i];
    if(lcd_s[i]<lcd_min[i])lcd_min[i]=lcd_s[i];
    lcd_value[i] = 2*lcd_max[i]/3+lcd_min[i]/3;
    
    }
  for(int i=0;i<12;i++)
    if(lcd_s[i]>lcd_value[i])lcd[i]=1;
    else lcd[i]=0;

  
  //if(lcd[2]==0&&lcd[5]==0){}//中间
  if(lcd[2]==0&&lcd[3]==1){fix(NOWCWB);NOWCWB=0;t=0;}//偏左
  else if(lcd[1]==1&&lcd[2]==0){fix(NOWCWB);NOWCWB=1;t=0;}//偏右
  else if(lcd[0]==0&&lcd[2]==1){fix(NOWCWB);NOWCWB=4;t=0;}//极偏左
  else if(lcd[4]==0&&lcd[2]==1){fix(NOWCWB);NOWCWB=5;t=0;}//极偏右
  else if((lcd[1]==1&&lcd[3]==1&&lcd[5]==0)||(lcd[1]==1&&lcd[2]==1&&lcd[5]==0)){t=1;}
  else if(lcd[0]==1&&lcd[1]==1&&lcd[2]==1&&lcd[3]==1&&lcd[4]==1&&lcd[5]==1){NOWCWB=-1;fix(NOWCWB);t=0;}//原地左转
  else{fix(NOWCWB);NOWCWB=2;t=0;}
}
void car::lcd_init()//更新lcd的数值
{

  for (int i = 0; i < 12; i++) {
    lcd[0] += analogRead(A0) > lcd_value[0] ? 1 : 0;
    lcd[1] += analogRead(A1) > lcd_value[1] ? 1 : 0;
    lcd[2] += analogRead(A2) > lcd_value[2] ? 1 : 0;
    lcd[3] += analogRead(A3) > lcd_value[3] ? 1 : 0;
    lcd[4] += analogRead(A6) > lcd_value[4] ? 1 : 0;
    lcd[5] += analogRead(A11) > lcd_value[5] ? 1 : 0;
    lcd[6] += analogRead(A9) > lcd_value[6] ? 1 : 0;
    lcd[7] += analogRead(A10) > lcd_value[7] ? 1 : 0;
    lcd[8] += analogRead(A7) > lcd_value[8] ? 1 : 0;
    lcd[9] += analogRead(A8) > lcd_value[9] ? 1 : 0;
    lcd[10] += analogRead(A12) > lcd_value[10] ? 1 : 0;
    lcd[11] += analogRead(A13) > lcd_value[11] ? 1 : 0;
  }
  lcd[0] = lcd[0] > 2 ? 1 : 0;
  lcd[1] = lcd[1] > 2 ? 1 : 0;
  lcd[2] = lcd[2] > 2 ? 1 : 0;
  lcd[3] = lcd[3] > 2 ? 1 : 0;
  lcd[4] = lcd[4] > 2 ? 1 : 0;
  lcd[5] = lcd[5] > 2 ? 1 : 0;
  lcd[6] = lcd[6] > 2 ? 1 : 0;
  lcd[7] = lcd[7] > 2 ? 1 : 0;
  lcd[8] = lcd[8] > 2 ? 1 : 0;
  lcd[9] = lcd[9] > 2 ? 1 : 0;
  lcd[10] = lcd[10] > 2 ? 1 : 0;
  lcd[11] = lcd[11] > 2 ? 1 : 0;
  
    lcd_s[0] = analogRead(A0) ;
    lcd_s[1] = analogRead(A1) ;
    lcd_s[2] = analogRead(A2) ;
    lcd_s[3] = analogRead(A3) ;
    lcd_s[4] = analogRead(A6) ;
    lcd_s[5] = analogRead(A11);
    lcd_s[6] = analogRead(A9) ;
    lcd_s[7] = analogRead(A10) ;
    lcd_s[8] = analogRead(A7) ;
    lcd_s[9] = analogRead(A8) ;
    lcd_s[10] = analogRead(A12) ;
    lcd_s[11] = analogRead(A13);

  
  //if(lcd[2]==0&&lcd[5]==0){}//中间
  if(lcd[2]==0&&lcd[3]==1){fix(NOWCWB);NOWCWB=0;t=0;}//偏左
  else if(lcd[1]==1&&lcd[2]==0){fix(NOWCWB);NOWCWB=1;t=0;}//偏右
  else if(lcd[0]==0&&lcd[2]==1){fix(NOWCWB);NOWCWB=4;t=0;}//极偏左
  else if(lcd[4]==0&&lcd[2]==1){fix(NOWCWB);NOWCWB=5;t=0;}//极偏右
  else if((lcd[1]==1&&lcd[3]==1&&lcd[5]==0)||(lcd[1]==1&&lcd[2]==1&&lcd[5]==0)){t=1;}
  else if(lcd[0]==1&&lcd[1]==1&&lcd[2]==1&&lcd[3]==1&&lcd[4]==1&&lcd[5]==1){NOWCWB=-1;fix(NOWCWB);t=0;}//原地左转
  else{fix(NOWCWB);NOWCWB=2;t=0;}
//  for(int i=0;i<6;i++){
//  Serial.print(i);
//  Serial.print(":");
//  Serial.print(lcd[i]);
//  Serial.print("||");
//  Serial.print(lcd_s[i]);
//  Serial.print("  ");
//  }
//  Serial.println();
//  Serial.println(NOWCWB);
//  Serial.println(NOWCWB);
}
  
int car::MatchingPath()//匹配路径 0-普通巡线 1-十字路口
{
  lcd_read();
  if(millis()<begintime)return 0;
  int sum1=0;
  int sum2=0;
  //for(int i=0;i<5;i++)sum1+=lcd[i];
  sum2+=((lcd[6]==0||lcd[7]==0)&&(lcd[8]==0||lcd[9]==0));//侧四两边至少各有一个检测到黑
  if(sum2>0){begintime=millis()+5000;return 1;}//在线上
  return 0;
}
void car::motor(char pin, char pwmpin, char state, int val)
{

  pinMode(pin, OUTPUT);
  if (state == 1)
  {
    analogWrite(pwmpin, 255-val);
    digitalWrite(pin, 1);
  }
  else if (state == 2)
  {
    analogWrite(pwmpin, val);
    digitalWrite(pin, 0);
  }
  else if (state == 0)
  {
    analogWrite(pwmpin, 0);
    digitalWrite(pin, 0);
  }
}
void car::runfoward(int i)   //前进
{
  motor(3, 2, 1, i);
  motor(5, 4, 1, i);
  motor(7, 6, 1, i);
  motor(9, 8, 1, i);
}
void car::runback(int j)    //后退
{
  motor(3, 2, 2, j);
  motor(5, 4, 2, j);
  motor(7, 6, 2, j);
  motor(9, 8, 2, j);
}
void car::turnL_s(int m = 255)     //左转
{
  motor(3, 2, 1, 50);
  motor(5, 4, 1, m);
  motor(7, 6, 1, 50);
  motor(9, 8, 1, m);
}
void car::turnL(int m = 255)      //原地左转
{
  motor(3, 2, 2, m);
  motor(5, 4, 1, m);
  motor(7, 6, 2, m);
  motor(9, 8, 1, m);
}
void car::turnR_s(int m = 255)      //右转 
{
  motor(3, 2, 1, m);
  motor(5, 4, 1, 50);
  motor(7, 6, 1, m);
  motor(9, 8, 1, 50);
}
void car::turnR(int m = 255)       //原地右转 
{
  motor(3, 2, 1, m);
  motor(5, 4, 2, m);
  motor(7, 6, 1, m);
  motor(9, 8, 2, m);
}
void car::stop()            //停止
{
  motor(3, 2, 1, 0);
  motor(5, 4, 0, 0);
  motor(7, 6, 1, 0);
  motor(9, 8, 0, 0);
}
void car::followline() {
  lcd_read();
  if((lcd[6]+lcd[7]+lcd[8]+lcd[9])<=2)return;
  if(t==1){
      runfoward(255);
      delay(500);
      lcd_read();
  }
  else{
    for(int i=1;i<=6;i++){
  //      Serial.println(i);
  //      Serial.println(PRECWB);
  //      Serial.println(NOWCWB);
  //      delay(5000);
      switch (i){
        case 1:{
          if(NOWCWB==0){
            turnL_s();
            delay(30);
            runmethod=0;
            lcd_read();
            break;}
          }
        case 2:{
           if(NOWCWB==1){
            turnR_s();
            delay(30);
            runmethod=1;
            lcd_read();
            break;}
          }
        case 3:{
           if(NOWCWB==2){
            runfoward(255);
            delay(30);
            runmethod=2;
            lcd_read();
            break;}
          }
        case 4:{
           if(NOWCWB==4){
            turnL_s();
            delay(60);
            runmethod=4;
            lcd_read();
            break;}
          }
        case 5:{
           if(NOWCWB==5){
            turnR_s();
            delay(60);
            runmethod=5;
            lcd_read();
            break;}
          }
         default:{
          if(NOWCWB==-1){
            runfoward(255);
            delay(50);
            runmethod=4;
            lcd_read();
            break;}
        }
      }
    }
    int time=60;
    int sum=0;
    while(NOWCWB==PRECWB&&NOWCWB!=2&&sum<10){
        time=time/2;
        sum++;
  //      Serial.println(PRECWB);
  //      Serial.println(NOWCWB);
  //      Serial.print("方式");
  //      Serial.println(runmethod);
  //      delay(5000);
        switch (runmethod){
          case 0:{
              lcd_read();
              if((lcd[6]+lcd[7]+lcd[8]+lcd[9])<=2)return;
              turnL_s(200);
              delay(time);
              break;
            }
          case 1:{
              lcd_read();
              if((lcd[6]+lcd[7]+lcd[8]+lcd[9])<=2)return;
              turnR_s(200);
              delay(time);
              lcd_read();
              break;
          }
          case 2:{
              lcd_read();
              if((lcd[6]+lcd[7]+lcd[8]+lcd[9])<=2)return;
              runfoward(255);
              delay(time);
              lcd_read();
              break;
            }
          case 4:{
              lcd_read();
              if((lcd[6]+lcd[7]+lcd[8]+lcd[9])<=2)return;
              turnL_s();
              delay(time);
              lcd_read();
              break;
  //            turnL();
  //            delay(time);
  //            lcd_read();
              break;
            }
          case 5:{
              lcd_read();
              if((lcd[6]+lcd[7]+lcd[8]+lcd[9])<=2)return;
              turnR_s();
              delay(time);
              lcd_read();
              break;
  //            turnR();
  //            delay(time);
  //            lcd_read();
              break;
            }
          default:lcd_read();
        }
    }
  }


//速度较大时，添加回转力
//  if(measure(PRECWB,NOWCWB)==1){
//      turnL();
//      delay(1);
//    }
//  else if(measure(PRECWB,NOWCWB)==2){
//      turnR();
//      delay(1);
//    }
//  else if(measure(PRECWB,NOWCWB)==3){
//      turnR();
//      delay(2);
//    }
//  else if(measure(PRECWB,NOWCWB)==4){
//      turnL();
//      delay(2);
//    }
//  else;
//  delay(10000);
  }
