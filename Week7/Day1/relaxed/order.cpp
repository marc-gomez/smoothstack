#include <iostream>
#include <atomic>
#include <thread>
#include <cassert>

using namespace std;

atomic<int> x, y, Cx, Cy, Dx, Dy;

void store_x() {
	x.store(1, memory_order_relaxed);
}

void store_y() {
	y.store(2, memory_order_relaxed);
}

void read_xy() {
	while(!(Cx = x.load(memory_order_relaxed)));
	Cy = y.load(memory_order_relaxed);
}

void read_yx() {
	while(!(Dy = y.load(memory_order_relaxed)));
	Dx = x.load(memory_order_relaxed);
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


// Running the program multiple times usually shows:
// Cx: 1, Cy: 2
// Dx: 1, Dy: 2
//		but sometimes Cy is 0