#include <iostream>
#include <unordered_map>
#include <cassert>

using namespace std;

unordered_map<int, char> mymap;

class Grade {
	

	public:
		char grade(const int&& score) {
			return mymap[score];
		}
};


int main() {
	Grade g;
	mymap[10] = 'D';
	mymap[20] = 'C';
	mymap[30] = 'B';
	mymap[40] = 'A';
	int scores[4] = {10, 20, 30, 40};
	
	assert(g.grade(10) == 'D');
	assert(g.grade(20) == 'C');
	assert(g.grade(30) == 'B');
	assert(g.grade(40) == 'A');
	
	
	for (int i = 0; i < sizeof(scores)/sizeof(scores[0]); ++i) {
		cout <<  "Score = " << scores[i] << ": Grade " << g.grade(move(scores[i])) << endl;
	}
}
