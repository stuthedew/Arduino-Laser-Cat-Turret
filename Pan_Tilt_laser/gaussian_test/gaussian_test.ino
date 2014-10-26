#include "stu_gauss.h"
#include <math.h>
#include <Gaussian.h>

StuGauss g;

void setup(){

  Serial.begin(9600);

}



void loop(){
  for(int i = 0; i < 1000; i++){
  Serial.print(g.gRandom(60, 5));

  }

}
