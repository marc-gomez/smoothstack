#include <iostream>
#include <atomic>
#include <thread>
#include <cassert>

using namespace std;

atomic<int> x, y, Cx, Cy, Dx, Dy;

void store_x() {
	x.store(1, memory_order_seq_cst);
}

void store_y() {
	y.store(2, memory_order_seq_cst);
}

void read_xy() {
	while(!(Cx = x.load(memory_order_seq_cst)));
	Cy = y.load(memory_order_seq_cst);
}

void read_yx() {
	while(!(Dy = y.load(memory_order_seq_cst)));
	Dx = x.load(memory_order_seq_cst);
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

// The store to x or the store to y must happen first.

// If Cy in read_xy is 0,
// the store of x happens before the store of y.
// Dx must then be 1 is read_yx

// Running the program multiple times always shows:
// Cx: 1, Cy: 2
// Dx: 1, Dy: 2