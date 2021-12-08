#include <iostream>
#include <string>

using namespace std;

int main() {
	cout << "What data type would you like to know the size of?" << endl;
	string input;
	getline(cin, input);

	if (input.compare("char") == 0) {
		cout << sizeof(char) << " Byte" << endl;
	}
	else if (input.compare("int") == 0) {
		cout << sizeof(int) << " Bytes" << endl;
	}
	else if (input.compare("short int") == 0) {
		cout << sizeof(short int) << " Bytes" << endl;
	}
	else if (input.compare("long int") == 0) {
		cout << sizeof(long int) << " Bytes" << endl;
	}
	else if (input.compare("float") == 0) {
		cout << sizeof(float) << " Bytes" << endl;
	}
	else if (input.compare("double") == 0) {
		cout << sizeof(double) << " Bytes" << endl;
	}
	else if (input.compare("wchar_t") == 0) {
		cout << sizeof(wchar_t) << " Bytes" << endl;
	}
	else if (input.compare("long long int") == 0) {
		cout << sizeof(long long int) << " Bytes" << endl;
	}
	else if (input.compare("long double") == 0) {
		cout << sizeof(long double) << " Bytes" << endl;
	}
	else {
		cout << "Unknown data type" << endl;
	}
}
