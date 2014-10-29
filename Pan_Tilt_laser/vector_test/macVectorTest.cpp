#include <iostream>
#include <stdio.h>
#include <cmath>
#include "stu_vector.h"
#include "stu_vector.cpp"

using namespace std;


SVector s(100, 200);
SVector t(600, 800);
int main(int argc, char *argv[]) {
 

cout << s.x() << endl << s.y() << endl;
cout << s.mag() << endl;

s = s.setMag(500);
cout << s.x() << endl << s.y() << endl;
cout << s.mag() << endl;

return(0);
cout << s.x() << endl << s.y() << endl;


s = s.mult(2);
cout << s.x() << endl << s.y() << endl;


cout << s.x() << endl << s.y() << endl;
}