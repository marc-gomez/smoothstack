#include <iostream>
#include <cassert>

using namespace std;

bool isPowTwo(int myint) {
	// myint has a hamming weight of 1
	return (myint != 0) && !(myint & (myint-1));
}

int main() {
	assert(isPowTwo(0) == false);
	assert(isPowTwo(1) == true);
	assert(isPowTwo(2) == true);
	assert(isPowTwo(3) == false);
	assert(isPowTwo(4) == true);
	assert(isPowTwo(5) == false);
	assert(isPowTwo(6) == false);
	assert(isPowTwo(7) == false);
	assert(isPowTwo(8) == true);
}
