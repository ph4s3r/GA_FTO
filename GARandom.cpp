#include "GARandom.h"
#include <ctime>
#include <cmath>
#include <string>
#include <iostream>

using namespace std;

float float_rand(float startvalue, float endvalue) {
	if(endvalue != 0.0f) return ((rand()/(32768/endvalue))) + startvalue;
	else {
		cout << "Hibas float_rand hivas !!" << endl;
		return 0.1f;
	}
}

int int_rand(int startvalue, int endvalue) {
	if(endvalue != 0) return ((rand()/(32768/endvalue))) + startvalue;
	else {
		cout << "Hibas int_rand hivas !!" << endl;
		return 1;
	}
}