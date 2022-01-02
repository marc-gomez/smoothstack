#include <iostream>
#include <string>
#include <sstream>
#include <limits.h>
#include "BigNumber.h"

using namespace std;

bool verify_long(string str) {
	if (str.length() > 64) {
		return false;
	}
	for (int i = 0; i < str.length(); ++i) {
		if (isdigit(str[i]) == false) {
			return false;
		}
	}
	return true;
}

int main() {
	string a;
	string b;

	cout << "Enter integer A: ";
	getline(cin, a);

	if (!verify_long(a)) {
		cout << a << " is not a valid integer" << endl;
		return -1;
	}

	cout << "Enter integer B: ";
	getline(cin, b);

	if (!verify_long(b)) {
		cout << b << " is not a valid integer" << endl;
		return -1;
	}

	BigNumber A(a);
	BigNumber B(b);
	BigNumber C;

	A.print(A.simulate_multiply(B, C));

	return 0;
}
