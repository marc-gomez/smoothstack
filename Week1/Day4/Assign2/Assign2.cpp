#include <cassert>
#include <iostream>
#include <typeinfo>

using namespace std;

int main() {

	int myint = 1;
	float myfloat = 2.0;
	char mychar = 'z';
	bool mybool = true;

	auto a = myint + myfloat;
	cout << typeid(a).name() << endl; // float

	auto b = myint + mychar;
	cout << typeid(b).name() << endl;  // int

	auto c = myint + mybool;
	cout << typeid(c).name() << endl;  // int

	auto d = myfloat + mychar;
	cout << typeid(d).name() << endl; // float

	auto e = myfloat + mybool;
	cout << typeid(e).name() << endl; // float

	auto f = mychar + mybool;
	cout << typeid(f).name() << endl; // int

}
