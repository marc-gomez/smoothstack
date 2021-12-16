#include <iostream>
#include <cmath>
#include <cassert>

using namespace std;

int binet(int b) {
	double phi = (1+sqrt(5))/2;
	//double psi = -1/phi;

	return b*(log(2) / log(phi)) + (log(5) / log(phi)/2);

	// this calculation is an upper-bounded approximation,
	// so the real value could be less than this
}

int main() {
	cout << binet(31) << endl;
	cout << binet(63) << endl;

	assert(binet(31) == 46);
	assert(binet(63) == 92);
	assert(binet(1) == 3);
	assert(binet(2) == 4);
	assert(binet(3) == 5);
}
