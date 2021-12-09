#include <cassert>

int main(void) {
	int x = 4;
	int y = x+++2;

	assert(y == 6);
	assert(x == 5);

	int x2 = 4;
	int y2 = ++x2+2;

	assert(y2 == 7);
	assert(x2 == 5);

}

// the value of x is saved, x is incremented, then the saved value of x is added to 2..
// 
// If y was instead written as y = ++x+2 , then the value would change so that x is incremented without a copy.
