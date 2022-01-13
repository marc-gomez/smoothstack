#include <iostream>
#include <cassert>
#include <string>

using namespace std;

template<typename T>
class Vector {
	private:
		T* mData;
		int mSize;
		int mCapacity;
		T mUndefined;

	public:
		Vector() {
			mSize = 0;
			mCapacity = 0;
			reserve(15);
		}

		~Vector() {
			if (mCapacity > 0) {
				delete[] mData;
			}
		}

		Vector(const Vector<T> &other) {
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			T * newData = new T[other.mCapacity];
			for(int i = 0; i < other.mSize; ++i) {
				newData[i] = other.mData[i];
			}
			mData = newData;
		}
		
		Vector &operator=(const Vector<T> &other) {
			if (this == &other) {
				return *this;
			}
			if (mCapacity > 0) {
				delete[] mData;
			}
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			T * newData = new T[other.mCapacity];
			for (int i = 0; i < other.mSize; ++i) {
				newData[i] = other.mData[i];
			}
			mData = newData;
			return *this;
		}
		
		void push_back(const T &tItem) {
			if (mSize == mCapacity) {
				reserve(2 * mCapacity);
			}
			mData[mSize] = tItem;
			++mSize;
		}
		
		void pop_back() {
			if (mSize > 0) {
				--mSize;
				mData[mSize] = mUndefined;
			}
		}
		
		T &at(int tWhere) {
			if (tWhere >= mSize) {
				return mUndefined;
			}
			return mData[tWhere];
		}
		
		void clear() {
			mSize = 0;
			if (mCapacity > 0) {
				delete[] mData;
			}
			mCapacity = 0;
			reserve(15);
		}
		
		int size() const {
			return mSize;
		}
		
		void reserve(int tCount) {
			T * newData = new T[tCount];
			for (int i = 0; i < mSize; ++i) {
				newData[i] = mData[i];
			}
			if (mCapacity > 0) {
				delete[] mData;
			}
			mData = newData;
			mCapacity = tCount;
		}

		int capacity() const {
			return mCapacity;
		}
};

int main() {

	// Test initialization
	Vector<int> a;
	assert(a.size() == 0);
	assert(a.capacity() == 15);

	// Test push_back
	a.push_back(1);
	assert(a.size() == 1);
	assert(a.capacity() == 15);
	assert(a.at(0) == 1);

	// Test pop_back
	a.pop_back();
	assert(a.size() == 0);
	assert(a.capacity() == 15);

	// Test clear on empty Vector
	a.clear();
	assert(a.size() == 0);
	assert(a.capacity() == 15);

	// Test clear on Vector with elements
	a.push_back(1);
	a.push_back(2);
	a.clear();
	assert(a.size() == 0);
	assert(a.capacity() == 15);

	// Test reserve
	a.push_back(1);
	a.reserve(20);
	assert(a.size() == 1);
	assert(a.at(0) == 1);
	assert(a.capacity() == 20);

	// Test capacity growth
	Vector<int> b;
	for (int i = 0; i < 15; ++i) {
		b.push_back(i);
	}
	assert(b.size() == 15);
	assert(b.capacity() == 15);
	b.push_back(16);
	assert(b.size() == 16);
	assert(b.capacity() == 30);

	// Test copy constructor
	Vector<int> c(a);
	assert(c.size() == 1);
	assert(c.capacity() == 20);
	assert(c.at(0) == 1);

	// Test assignment operator
	Vector<int> d;
	d = c;
	assert(d.size() == 1);
	assert(d.capacity() == 20);
	assert(d.at(0) == 1);

	// Test Vector with strings
	Vector<string> e;
	e.push_back("first");
	e.push_back("second");
	assert(e.size() == 2);
	assert(e.capacity() == 15);
	assert(e.at(1) == "second");
}
