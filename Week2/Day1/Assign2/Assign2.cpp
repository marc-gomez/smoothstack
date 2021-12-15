#include <iostream>

using namespace std;

struct Sample {
	void myfunc(float f) {
		cout << "in non-const myfunc" << endl;
	}

	void myfunc(float f) const {
		cout << "in const myfunc" << endl;
	}
};

int main() {
	Sample s1;
	const Sample s2;
	float i = 1.0;
	s1.myfunc(i);
	s2.myfunc(i);
}

