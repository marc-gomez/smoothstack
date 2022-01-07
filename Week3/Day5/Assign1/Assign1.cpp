#include <string>
#include <type_traits>
#include <cassert>

using namespace std;

struct A {};

class B {
	B &operator=(const B&) = delete;
};

struct C {
	string str[2];
};

struct D {
	int n;
	D &operator=(const D&) = default;
};

struct E {
	E(const E &other) = delete;
};

struct F {
	F(const F &other) {};
};

struct G {
	G &operator=(const G &g) {return *this;}
};

struct H {
	H &operator=(H &h) {return *this;}
};

int main() {
	assert(is_copy_assignable<int>::value);
	assert(!is_copy_assignable<int[]>::value);
	assert(is_copy_assignable<A>::value); // Copy assignment is implicitly added
	assert(!is_copy_assignable<B>::value); // Copy assignment is explicitly deleted
	assert(is_copy_assignable<C>::value); // Copy assignment is implicitly added
	assert(is_copy_assignable<D>::value); // Copy assignment is trivial
	assert(is_trivially_copy_assignable<D>::value);
	assert(is_copy_assignable<E>::value); // Copy assignment is implicitly added, doesn't depend on copy ctor
	assert(is_copy_assignable<F>::value); // Copy assignment is implicitly added
	assert(is_copy_assignable<G>::value); // Copy assignment is explicitly added
	assert(!is_copy_assignable<H>::value); // Copy assignment is not applicable
}
