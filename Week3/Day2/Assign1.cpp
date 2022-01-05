#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <cstring>

using namespace std;

class Sentence {
	private:
		char* sentence;
		vector<int> vsentence;

		void fillvec() {
			vsentence.push_back(0);

			bool prep = false;
			
			for (int i = 0; i < strlen(sentence); ++i) {
				if (sentence[i] == ' ') {
					prep = true;
				}
				else if (sentence[i] != ' ' && prep == true) {
					vsentence.push_back(i);
					prep = false;
				}
			}
		}
		
	public:
		// Constructor
		Sentence(char * sent) {
			int len = strlen(sent) + 1;
			sentence = new char[len];
			strncpy(sentence, sent, len);
			sentence[len] = '\0';

			fillvec();
		}

		// Copy constructor
		Sentence(const Sentence &other) {
			char * sent = other.data();

			int len = strlen(sent) + 1;
			sentence = new char[len];
			strncpy(sentence, sent, len);
			sentence[len] = '\0';

			fillvec();
		}

		// Assignment operator
		Sentence & operator=(const Sentence &other) {
			if (this == &other) {
				return *this;
			}

			replace(other.data());
			return *this;
			
		}

		// Move constructor
		Sentence(Sentence &&other) {
			sentence = other.sentence;
			vsentence = other.vsentence;
			other.sentence = nullptr;
			other.vsentence.clear();
		}

		// Move assignment operator
		Sentence & operator=(Sentence &&other) {
			if (this == &other) {
				return *this;
			}
			delete[] sentence;
			sentence = other.sentence;
			vsentence = other.vsentence;
			other.sentence = nullptr;
			other.vsentence.clear();
			return *this;
		}

		char * data() const{
			return sentence;
		}

		int num_words() const {
			return vsentence.size();
		}

		string get_word(int n) {
			string s(sentence + vsentence[n]);
			for (int i = 0; i < s.length(); ++i) {
				if (s[i] == ' ') {
					return s.substr(0, i);
				}
			}
			return s;
		}

		void replace(char* sent) {
			delete[] sentence;
			int len = strlen(sent) + 1;
			sentence = new char[len];
			strncpy(sentence, sent, len);
			sentence[len] = '\0';

			vsentence.clear();
			fillvec();
		}

		~Sentence() {
			delete[] sentence;
		}
};

int main() {
	char str1[] = "Quick brown fox jumps over the lazy dog.";
	char str2[] = "A new sentence.";

	// Test constructor
	Sentence s(str1);

	assert(s.num_words() == 8);
	assert(s.get_word(1) == "brown");
	assert(strcmp(s.data(), str1) == 0);

	// Test replace
	s.replace(str2);

	assert(s.num_words() == 3);
	assert(s.get_word(0) == "A");
	assert(strcmp(s.data(), str2) == 0);

	// Test copy constructor
	Sentence s2(s);

	assert(s2.num_words() == 3);
	assert(s2.get_word(0) == "A");
	assert(strcmp(s2.data(), str2) == 0);
	assert(&s2 != &s);

	// Test another copy constructor
	Sentence s3 = s2;

	assert(s3.num_words() == 3);
	assert(s3.get_word(0) == "A");
	assert(strcmp(s3.data(), str2) == 0);
	assert(&s3 != &s2);

	// Test assignment operator
	Sentence s4(str1);
	s4 = s3;

	assert(s4.num_words() == 3);
	assert(s4.get_word(0) == "A");
	assert(strcmp(s4.data(), str2) == 0);
	assert(&s4 != &s3);

	// Test move constructor
	Sentence s5 = move(s4);

	assert(s5.num_words() == 3);
	assert(s5.get_word(0) == "A");
	assert(strcmp(s5.data(), str2) == 0);
	assert(&s5 != &s4);
	assert(s4.num_words() == 0);
	assert(!s4.data());

	// Test move assignment operator
	Sentence s6(str1);
	s6 = Sentence(str2);
	
	assert(s6.num_words() == 3);
	assert(s6.get_word(0) == "A");
	assert(strcmp(s6.data(), str2) == 0);
}
