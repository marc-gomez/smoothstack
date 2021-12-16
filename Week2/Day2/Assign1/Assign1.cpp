#include <iostream>
#include <vector>

using namespace std;

int computeFib(int term) {
	if (term == 1 || term == 2) {
		return 1;
	}
	return computeFib(term-1) + computeFib(term-2);
}

int main() {
	vector<int> fib;
	for(int i = 1; i <= 50; ++i) {
		int val = computeFib(i);
		fib.push_back(val);
		cout << val << " ";
	}
	cout << endl;
}

// The 47th fibonacci term prints out as negative.
// This is because the number becomes larger than an int can hold, so the integer overflows.
// Any term after 46 in this program is not an actual fibonacci number and is meaningless,
// 	but the program does not output an error
//
// Another problem is that using recursion to solve this problem
// 	 takes a long time to compute for large Fibonacci terms.
// A more efficient solution would use dynamic programming.
