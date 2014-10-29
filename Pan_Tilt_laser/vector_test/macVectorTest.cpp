#include <iostream>
#include <stdio.h>
#include <cmath>
#include "stu_vector.h"
#include "stu_vector.cpp"

using namespace std;


SVector s(10, 20);
SVector t(60, 80);
int main(int argc, char *argv[]) {
 

cout << s.x() << endl << s.y() << endl;
s = s.rotate(M_PI_4);

cout << s.x() << endl << s.y() << endl;
return(0);

s = s.mult(2);
cout << s.x() << endl << s.y() << endl;


cout << s.x() << endl << s.y() << endl;
}