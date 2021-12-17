#include <iostream>
#include <cassert>
#include <limits>

using namespace std;

static int (*funcs[3])(int, int);

int add(int a, int b) {
	return a + b;
}

int subtract(int a, int b) {
	return a - b;
}

int multiply(int a, int b) {
	return a * b;
}

int compute(int arg1, int arg2, unsigned char funcmask) {
	if (funcmask == 1) {
		return funcs[0](arg1, arg2);
	}
	else if (funcmask == 2) {
		return funcs[1](arg1, arg2);
	}
	else if (funcmask == 4) {
		return funcs[2](arg1, arg2);
	}
	return numeric_limits<int>::max(); // Error output if function is called incorrectly
}

int main() {
	funcs[0] = &add;
	funcs[1] = &subtract;
	funcs[2] = &multiply;

	assert(compute(2, 3, 1) == 5);
	assert(compute(2, 3, 2) == -1);
	assert(compute(2, 3, 4) == 6);
}
