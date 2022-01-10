#include <exception>
#include <cassert>
#include <cmath>
#include <string>

using namespace std;

class FloatException : public exception {
	public:
		string err;
		FloatException(float f) : err(to_string(f) + " is not a valid integer") {}
		const char * what() const throw() {
			return err.c_str();
		}
};

bool lessthan(float a, float b) {
	if (floorf(a) != a) {
		throw FloatException(a);
	}
	else if (floorf(b) != b) {
		throw FloatException(b);
	}
	return a < b;
}

int main() {
	float x1 = 1.0f;
	float y1 = 2.0f;

	float x2 = 1.0f;
	float y2 = 2.001f;

	try {
		assert(lessthan(x1, y1));
		lessthan(x2, y2);
	}
	catch(FloatException &e) {
		terminate();
	}
}
