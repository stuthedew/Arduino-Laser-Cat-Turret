#include "stu_vector.h"
#include <ArduinoUnit.h>
SVector s(10, 20);

void setup(){
  Serial.begin(115200);
  SVector a = s.fromAngle(1);
  Serial.println(s.x());
  Serial.println(s.y());
  Serial.println(a.x());
  Serial.println(a.y());

  s = s.rotate(M_PI_2);

  Serial.println(s.x());
  Serial.println(s.y());

}


void loop(){



}
