#ifndef _CAR_H__
#define _CAR_H__
class car{
  public:
  int f;//定义一个变量记录是否进入黑线交界处
  int k;//记录当前路口数
  int runmethod=2;
  int PRECWB;//标记上一次run模式
  int NOWCWB=0;//标记当前run模式
  int lcd[12];//保存五个传感器的读取情况
  int lcd_s[12];
  int t;
  int lcd_max[12]={0,0,0,0,0,0,0,0,0,0,0,0};
  int lcd_min[12]={1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000};
  int lcd_value[12]={0,0,0,0,0,0,0,0,0,0,0,0};
  car(int LA, int LB, int RA, int RB,int L1, int L2, int R1, int R2);
  void lcd_read();//更新12个传感器的数据
  void lcd_init();//初始化临界值
  void runfoward(int i);//直走
  void runback(int j);//回头
  void turnL(int m = 255);//左转
  void turnL_s(int m = 255);//原地左转
  void turnR(int m = 255);//右转
  void turnR_s(int m = 255);//原地右转
  void stop();//停
  int measure(int a, int b);
  void fix(int cwbsecond);
  void followline();//巡线(对i号传感器巡线)
  const int Crossroads[12]={1,0,0,0,1,0,0,0,0,0,1,1};//十字路口
  int MatchingPath();
  unsigned long begintime = 0;
private:
  
  void motor(char pin,char pwmpin,char state,int val);
  int leftA_PIN;//记录引脚位置
  int leftB_PIN;
  int righA_PIN;
  int righB_PIN;
  int leftA_B_PIN;//记录引脚位置
  int leftB_B_PIN;
  int righA_B_PIN;
  int righB_B_PIN;
  
};

#endif
