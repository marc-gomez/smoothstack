#include <iostream>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <string>
#include <bitset>
#include <limits.h>

using namespace std;

// Validate hex number
bool validHex(const string &input) {
	for (int i = 0; i < input.size(); ++i) {
		if ((input[i] >= '0' && input[i] <= '9') ||
				(input[i] >= 'a' && input[i] <= 'f') ||
				(input[i] >= 'A' && input[i] <= 'F')) {}
		else {
			return false;
		}
	}
	return true;
}

// Validate binary number
bool validBin(const string &input) {
	for (int i = 0; i < input.size(); ++i) {
		if (input[i] < '0' || input[i] > '1') {
			return false;
		}
	}
	return true;
}

// Validate decimal number
bool validDec(const string &input) {
	for (int i = 0; i < input.size(); ++i) {
		if (input[i] < '0' || input[i] > '9') {
			return false;
		}
	}
	return true;
}


// Convert input string(s) to int
int handleInput(string &input) {
	int num;

	// hex input case
	if (input[0] == '0' && input[1] == 'x') {
		input.erase(0, 2);
		if (!validHex(input)) {
			return INT_MAX;
		}
		num = stoi(input, nullptr, 16);
	}

	// binary input case
	else if (input[0] == '0' && input[1] == 'b') {
		input.erase(0, 2);
		if (!validBin(input)) {
			return INT_MAX;
		}
		num = stoi(input, nullptr, 2);
	}

	// decimal input case
	else {
		if (!validDec(input)) {
			return INT_MAX;
		}
		num = stoi(input);
	}

	return num;
}

// Convert int to desired number type in string format
string handleOutput(int num, bool isBin, bool isHex) {
	// output hex number
	if (isHex) {
		stringstream tempstream;
		tempstream << hex << num;
		return "0x" + tempstream.str();
	}

	// output binary number
	else if (isBin) {
		string bits = bitset<32>(num).to_string();
		while(bits[0] == '0') {
			bits.erase(0, 1);
		}
		return "0b" + bits;
	}

	// output decimal number
	return to_string(num);
}

// Convert input string(s) into the desired number type and return as string
string convertNum(istream &instream, ostream &outstream) {
	// get input
	string line;
	outstream << "prompt > ";
	getline(instream, line);
	stringstream ss(line);
	
	// check for quit
	if (ss.str() == "quit" || ss.str() == "exit") {
		return ss.str();
	}

	// initialize
	string first;
	string second;
	ss >> first;
	bool inBin = false;
	bool inHex = false;
	bool toAdd = false;
	int num;
	int num2 = 0;

	// check for output type
	if (first == "bin") {
		inBin = true;
		ss >> first;
	}
	else if (first == "hex") {
		inHex = true;
		ss >> first;
	}

	// check for addition
	if (!ss.eof()) {
		string plus;
		ss >> plus;
		if (plus == "+") {
			toAdd = true;
			ss >> second;
		}
		else {
			return "Invalid Command";
		}
	}

	// convert input string(s) to int
	num = handleInput(first);
	if (num == INT_MAX) {
		return "Invalid Number";
	}
	if (toAdd) {
		num2 = handleInput(second);
		if (num2 == INT_MAX) {
			return "Invalid Number";
		}
	}
	num += num2;

	// convert int to desired number type in string format
	return handleOutput(num, inBin, inHex);
}

int main() {

	// Tests
	stringstream teststream;
	stringstream dummyout;

	teststream.str("0x3839");
	assert(convertNum(teststream, dummyout) == "14393");

	teststream.clear();
	teststream.str("0x1515 + 8675");
	assert(convertNum(teststream, dummyout) == "14072");

	teststream.clear();
	teststream.str("hex 14072");
	assert(convertNum(teststream, dummyout) == "0x36f8");

	teststream.clear();
	teststream.str("bin 0x36f8");
	assert(convertNum(teststream, dummyout) == "0b11011011111000");

	teststream.clear();
	teststream.str("0xg4f1");
	assert(convertNum(teststream, dummyout) == "Invalid Number");

	// Main program
	while(true) {
		string output = convertNum(cin, cout);
		if (output == "quit" || output == "exit") {
			break;
		}
		// I prefer regular output rather than right justified with 20 width
		cout << output << endl;
	}
}
