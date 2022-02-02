#include <iostream>
#include <atomic>
#include <thread>
#include <cassert>

using namespace std;

atomic<int> x, y, Cx, Cy, Dx, Dy;

void store_x() {
	x.store(1, memory_order_release);
}

void store_y() {
	y.store(2, memory_order_release);
}

void read_xy() {
	while(!(Cx = x.load(memory_order_consume)));
	assert(Cx == 1);
	Cy = y.load(memory_order_consume);
}

void read_yx() {
	while(!(Dy = y.load(memory_order_consume)));
	assert(Dy == 2);
	Dx = x.load(memory_order_consume);
}

int main() {
	x = 0;
	y = 0;

	thread A(store_x);
	thread B(store_y);
	thread C(read_xy);
	thread D(read_yx);

	A.join();
	B.join();
	C.join();
	D.join();

	cout << "Cx: " << Cx << ", Cy: " << Cy << endl;
	cout << "Dx: " << Dx << ", Dy: " << Dy << endl;
}

// The loads of x or y in the while loop carry a dependency through Cx or Dy

// Running the program multiple times shows:
// Cx: 1, Cy: 2
// Dx: 1, Dy: 2