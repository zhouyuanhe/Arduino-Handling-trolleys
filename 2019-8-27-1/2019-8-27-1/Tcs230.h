#include<Arduino.h>
class Tcs230{
  public:
  Tcs230();
  void color();
  void check();
int s0 = 8; 
int s1 = 9; 
int s2 = 12; 
int s3 = 11; 
int out = 10; 
int redLed = 2; 
int greenLed = 3; 
int blueLed = 4;
int red = 0; 
int green = 0; 
int blue = 0; 
};
