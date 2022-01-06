#include <cassert>
#include <cstring>
#include <iostream>

using namespace std;

class MyString {
	private:
		char * str;
	public:
		MyString(char const * inStr) {
			int len = strlen(inStr);
			str = new char[len + 1];
			strncpy(str, inStr, len);
			str[len] = '\0';
		}

		MyString(const MyString &other) {
			char * otherStr = other.data();
			int len = strlen(otherStr);
			str = new char[len + 1];
			strncpy(str, otherStr, len);
			str[len] = '\0';
		}

		MyString & operator=(const MyString &other) {
			if (this == &other) {
				return *this;
			}
			delete[] str;
			char * otherStr = other.data();
			int len = strlen(otherStr);
			str = new char[len + 1];
			strncpy(str, otherStr, len);
			str[len] = '\0';
			return *this;
		}

		MyString(MyString &&other) {
			str = other.data();
			other.str = nullptr;
		}

		MyString & operator=(MyString &&other) {
			if (this == &other) {
				return *this;
			}
			delete[] str;
			str = other.data();
			other.str = nullptr;
			return *this;
		}

		MyString operator+(const MyString &other) {
			int len = strlen(str) + strlen(other.data());
			char * newStr = new char[len + 1];
			strncpy(newStr, str, strlen(str));
			strncpy(newStr + strlen(str), other.data(), strlen(other.data()));
			newStr[len] = '\0';
			MyString tempStr(newStr);
			delete[] newStr;
			return tempStr;
		}

		MyString operator+(int increment) {
			MyString tempStr(str+increment);
			return tempStr;
		}

		MyString operator*(int mult) {
			int len = strlen(str) * mult;
			char * newStr = new char[len + 1];
			for (int i = 0; i < mult; ++i) {
				strncpy(newStr + (i * strlen(str)), str, strlen(str));
			}
			newStr[len] = '\0';
			MyString tempStr(newStr);
			delete[] newStr;
			return tempStr;
		}

		char operator[](int index) {
			return str[index];
		}

		bool operator==(char const * other) {
			if (strcmp(other, str) == 0) {
				return true;
			}
			return false;
		}

		~MyString() {
			delete[] str;
		}

		char * data() const {
			return str;
		}
};

int main() {
	MyString s = "abc";
	assert((s + s) == "abcabc");
	assert((s * 3) == "abcabcabc");
	assert(s[2] == 'c');
	assert((s + 1) == "bc");
}
