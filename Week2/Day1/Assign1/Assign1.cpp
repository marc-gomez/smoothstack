#include <iostream>
#include <iomanip>
#include <cmath>

using namespace std;

void floatRep(double f) {
	auto q = round(pow(2, 56)*f)/2;
	auto appr = q * pow(10, 55) / pow(2, 55);

	cout << "J = " << q << ", N = " << 55 << endl;
	cout << "0." << appr << endl;
}

int main() {

	cout << "Enter float: ";
	double x;
	cin >> x;

	cout << setprecision(55);
	floatRep(x);
}
