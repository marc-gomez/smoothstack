#include <iostream>
#include <string>
#include <cassert>

using namespace std;

class Person {
	private:
		//int index;

	public:
		Person(string name_in) {
			name = name_in;
			count = 1;
		}

		Person(string name_in, int count_in) {
			name = name_in;
			count = count_in + 1;
		}

		string name;
		int count;
		static Person* persons[10];
		static Person* getPerson(string name) {

			// loop through persons[]
			for (int i = 0; i < 10; ++i) {

				// if we reach an empty spot
				if (persons[i] == nullptr) {
					Person * p = new Person(name);
					persons[i] = p;
					return persons[i];
				}

				// if the name exists
				else if (persons[i]->name == name) {
					int c = persons[i]->count;
					Person * p = new Person(name, c);
					delete persons[i];
					persons[i] = p;
					return persons[i];
				}
			}

			return nullptr;			
		}
};

Person * Person::persons[10];

int main() {

	string names[] = {"bob", "james", "janet", "bob", "stella"};

	for (int ii = 0; ii < 5; ii++) {
		Person * p = Person::getPerson(names[ii]);
		cout << p->name << ": " << p->count << endl;
		
		// test names
		assert(p->name == names[ii]);
		
		// test counts
		if (ii == 3) { // second bob case
			assert(p->count == 2);
		}
		else {
			assert(p->count == 1);
		}
	}

	// free memory
	for (int i = 0; i < 10; ++i) {
		if (Person::persons[i]) {
			delete Person::persons[i];
			Person::persons[i] = nullptr;
		}
	}
}
