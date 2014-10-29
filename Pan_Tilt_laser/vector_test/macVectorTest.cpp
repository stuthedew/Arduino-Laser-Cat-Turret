#include <iostream>
#include <stdio.h>
#include <cmath>
#include "stu_vector.h"
#include "stu_vector.cpp"

using namespace std;


SVector s(6, 8);
int main(int argc, char *argv[]) {
 
cout << s.x() << endl << s.y() << endl;
cout << s.getMag() << endl;

s = s.sub(3, 4);

cout << s.x() << endl << s.y() << endl;

cout << s.getMag() << endl;
}