#include <iostream>
#include <cassert>
#include <string>

using namespace std;

template<typename T>
class myvector {
	private:
		T* mData;
		int mSize;
		int mCapacity;
		T mUndefined;

	public:
		myvector() {
			mSize = 0;
			mCapacity = 0;
			reserve(15);
		}

		myvector(T * t, int sz) {
			mSize = sz;
			mCapacity = 2 * sz;
			T * newData = new T[mCapacity];
			for(int i = 0; i < mSize; ++i) {
				newData[i] = t[i];
			}
			mData = newData;
		}

		~myvector() {
			if (mCapacity > 0) {
				delete[] mData;
			}
		}

		myvector(const myvector<T> &other) {
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			T * newData = new T[other.mCapacity];
			for(int i = 0; i < other.mSize; ++i) {
				newData[i] = other.mData[i];
			}
			mData = newData;
		}
		
		myvector &operator=(const myvector<T> &other) {
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

		myvector(myvector<T> &&other) {
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			mData = other.mData;
			other.mSize = 0;
			other.mCapacity = 0;
			other.mData = nullptr;
		}

		myvector &operator=(myvector &&other) {
			if (this == &other) {
				return *this;
			}
			if (mCapacity > 0) {
				delete[] mData;
			}
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			mData = other.mData;
			other.mSize = 0;
			other.mCapacity = 0;
			other.mData = nullptr;		
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

		T &operator[](int offset) {
			if (offset >= mSize) {
				return mUndefined;
			}
			return mData[offset];
		}
		
		T &at(int offset) {
			if (offset >= mSize) {
				return mUndefined;
			}
			return mData[offset];
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

		class iterator {
			private:
				friend myvector;
				const myvector *myvec;
				int current;

			public:
				iterator(const myvector * myvec, int offset) : myvec(myvec), current(offset) {}

				void operator++() {
					current++;
				}

				void operator--() {
					current--;
				}

				bool operator==(const iterator &other) {
					if (myvec == other.myvec && current == other.current) {
						return true;
					}
					return false;
				}

				bool operator!=(const iterator &other) {
					if (myvec != other.myvec || current != other.current) {
						return true;
					}
					return false;
				}

				T& operator*() {
					return myvec->mData[current];
				}

		};

		iterator begin() const {
			return iterator(this, 0);
		}

		iterator end() const {
			return iterator(this, mSize);
		}
};

int main() {

	// Test initialization
	myvector<int> a;
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
	assert(a[0] == 1);
	assert(a.capacity() == 20);

	// Test capacity growth
	myvector<int> b;
	for (int i = 0; i < 15; ++i) {
		b.push_back(i);
	}
	assert(b.size() == 15);
	assert(b.capacity() == 15);
	b.push_back(16);
	assert(b.size() == 16);
	assert(b.capacity() == 30);

	// Test copy constructor
	myvector<int> c(a);
	assert(c.size() == 1);
	assert(c.capacity() == 20);
	assert(c.at(0) == 1);

	// Test assignment operator
	myvector<int> d;
	d = c;
	assert(d.size() == 1);
	assert(d.capacity() == 20);
	assert(d.at(0) == 1);

	// Test Vector with strings
	myvector<string> e;
	e.push_back("first");
	e.push_back("second");
	assert(e.size() == 2);
	assert(e.capacity() == 15);
	assert(e[1] == "second");

	// Test array initialization
	int arr[] = {1, 2, 3};
	myvector<int> f(arr, sizeof(arr)/sizeof(arr[0]));
	assert(f.size() == 3);
	assert(f.capacity() == 6);
	assert(f[2] == 3);

	// Test move constructor
	myvector<int> g = move(f);
	assert(g.size() == 3);
	assert(g.capacity() == 6);
	assert(g[2] == 3);
	assert(f.size() == 0);
	assert(f.capacity() == 0);

	// Test move assignment operator
	myvector<int> h;
	h.push_back(5);
	h = move(g);
	assert(h.size() == 3);
	assert(h.capacity() == 6);
	assert(h[0] == 1);
	assert(g.size() == 0);
	assert(g.capacity() == 0);

	// Test iterator
	int index = 1;
	for (auto it = h.begin(), end = h.end(); it != end; ++it) {
		assert(*it == index);
		++index;
	}
	assert(index == 4);

	// Test iterator equality
	auto it1 = d.begin();
	auto it2 = d.end();
	--it2;
	assert(it1 == it2);
}
