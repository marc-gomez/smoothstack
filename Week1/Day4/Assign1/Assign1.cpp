#include <cassert>
#include <iostream>

using namespace std;

int main(void) {
	assert(3 == 3.0);     // This works because an int and double can be compared and have enough precision to be equal.
	//float x = 3.1;      //
	//assert(x == 3.1);   // This fails because x is a float and is being compared to a double. Floats aren't as precise as doubles, so there are minor differences in the numbers.
	assert(3.1 == 3.1);   // This works because both numbers are treated as a double
	assert(true == 1.0);  // This works because true is treated as a 1 -- false could be compared to 0 in the same way
	assert('a' == 97);    // This works because 97 is the ASCII value of 'a', which 'a' is converted to
	int * p = 0;          //
	assert(NULL == p);    // This works because NULL is a pointer that points to address 0, which p also does
	//assert(NULL == 0);  // This line fails because NULL is type pointer and 0 is not
}
