#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

void floatRep(float f) {
	float f_copy = f;
	int n = 0;

	// loop until an integer value is reached
	while(f_copy != (int)f_copy) {
		f_copy *= 2;
		n++;
	}

	cout << "J = " << f_copy << ", N = " << n << endl;
	cout << f << endl;
}

int main() {

	cout << "Enter float: ";
	float x;
	cin >> x;

	cout << setprecision(55);
	floatRep(x);
}