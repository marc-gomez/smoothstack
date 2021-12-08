#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

int main() {
	float arr[3] = {2,3,4};
	vector<float> vec;
	for(int i = 0; i < 3; ++i) {
		vec.push_back(arr[i]);
	}
	for(int i = 0; i < vec.size(); ++i) {
		if (i < 2) {
			cout << vec.at(i) << ", ";
		}
		else {
			cout << vec.at(i) << endl;
		}
	}
	assert(arr[0] == vec.at(0));
	assert(arr[1] == vec.at(1));
	assert(arr[2] == vec.at(2));

}
