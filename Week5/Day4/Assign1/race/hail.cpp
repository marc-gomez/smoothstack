#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <iomanip>

using namespace std;

mutex m;

void hailstone(vector<int> &vec, int id) {
	while (vec.back() != 1) {
		m.lock();
		if (vec.back() % 2 == 0) {
			vec.push_back(vec.back() / 2);
		}
		else {
			vec.push_back(vec.back() * 3 + 1);
		}
		cout << setw(3) << id << " ";
		m.unlock();
	}
}

int main() {
	vector<int> seq;
	seq.push_back(11); // Custom starting value
	cout << "Thread:    ";

	thread t1(hailstone, ref(seq), 0);
	thread t2(hailstone, ref(seq), 1);

	t1.join();
	t2.join();

	cout << endl << "Value: ";
	for (auto s : seq) {
		cout << setw(3) << s << " ";
	}
	cout << endl;
}
