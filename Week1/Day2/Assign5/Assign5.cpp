// Floats in C++ are usually precise for 6 or 7 digits.
// Doubles in C++ have more precision than a float.
// You could compare the same number as a float and a double to measure the precision.

#include <iostream>
#include <iomanip>

using namespace std;

int main() {
	cout << setprecision(11);
	cout << 21987654321.0f << endl;
	cout << 1987654321.0f << endl;
	cout << 987654321.0f << endl;
	cout << 87654321.0f << endl;
	cout << 7654321.0f << endl;
	cout << 654321.0f << endl;
	cout << 54321.0f << endl;
	cout << 4321.0f << endl;
	cout << 321.0f << endl;
	cout << 21.0f << endl;
	cout << 1.0f << endl;
	cout << 0.1f << endl;
	cout << 0.12f << endl;
	cout << 0.123f << endl;
	cout << 0.1234f << endl;
	cout << 0.12345f << endl;
	cout << 0.123456f << endl;
	cout << 0.1234567f << endl;
	cout << 0.12345678f << endl;
	cout << 0.123456789f << endl;
	cout << 0.1234567891f << endl;
	cout << 0.12345678912f << endl;
}
