#include <iostream>
#include <cassert>
#include <cmath>
#include <type_traits>

using namespace std;

class Dollar {
	public:
		double value;
		Dollar(double val) : value(val) {}
		Dollar() : value(0) {}
};

class Yen {
	public:
		double value;
		Yen(double val) : value(val) {}
		Yen() : value(0) {}
};

class Euro {
	public:
		double value;
		Euro(double val) : value(val) {}
		Euro() : value(0) {}
};

class Pound {
	public:
		double value;
		Pound(double val) : value(val) {}
		Pound() : value(0) {}
};

class Convert {
	public:
		Dollar operator()(Dollar d, Dollar d2) {
			return Dollar(d.value);
		}
		
		Dollar operator()(Pound p, Dollar d) {
			return Dollar(p.value * 1.37);
		}

		Dollar operator()(Yen y, Dollar d) {
			return Dollar(y.value * 0.0087);
		}

		Dollar operator()(Euro e, Dollar d) {
			return Dollar(e.value * 1.14);
		}

		Pound operator()(Pound p, Pound p2) {
			return Pound(p.value);
		}

		Pound operator()(Dollar d, Pound p) {
			return Pound(d.value * 0.73);
		}

		Pound operator()(Yen y, Pound p) {
			return Pound(y.value * 0.0064);
		}

		Pound operator()(Euro e, Pound p) {
			return Pound(e.value * 0.83);
		}

		Yen operator()(Yen y, Yen y2) {
			return Yen(y.value);
		}
		
		Yen operator()(Pound p, Yen y) {
			return Yen(p.value * 157.07);
		}

		Yen operator()(Dollar d, Yen y) {
			return Yen(d.value * 114.53);
		}

		Yen operator()(Euro e, Yen y) {
			return Yen(e.value * 131.04);
		}

		Euro operator()(Euro e, Euro e2) {
			return Euro(e.value);
		}

		Euro operator()(Dollar d, Euro e) {
			return Euro(d.value * 0.87);
		}

		Euro operator()(Yen y, Euro e) {
			return Euro(y.value * 0.0076);
		}

		Euro operator()(Pound p, Euro e) {
			return Euro(p.value * 1.20);
		}
};

template <class T, class C>
class Account {
	private:
		T amt;
	public:
		Account(T val) : amt(val) {}

		Account(double val) : amt(T(val)) {}
		
		Account() : amt(T(0)) {}
		
		Account(const Account &other) {
			amt = other.getAmt();
		}

		Account &operator=(const Account &other) {
			if (this == &other) {
				return *this;
			}
			amt = other.getAmt();
			return *this;
		}

		Account(Account &&other) {
			amt = other.getAmt();
			other.amt = T(0);
		}

		Account &operator=(Account &&other) {
			if (this == &other) {
				return *this;
			}
			amt = other.getAmt();
			other.amt = T(0);
			return *this;
		}

		template <class X>
		bool operator==(const Account<X, C> a) const {
			C c;
			if (typeid(amt) == typeid(a.getAmt())) {
				return abs(amt.value - a.getAmt().value) < 0.01;
			}
			else {
				return abs(amt.value - c(a.getAmt(), amt).value) < 0.01;
			}
		}

		template <class X>
		bool operator!=(const Account<X, C> a) const {
			return !(*this == a);
		}

		template <class X>
		bool operator<(const Account<X, C> a) const {
			C c;
			if (typeid(amt) == typeid(a.getAmt())) {
				return amt.value < a.getAmt().value;
			}
			else {
				return amt.value < c(a.getAmt(), amt).value;
			}
		}

		template <class X>
		bool operator>(const Account<X, C> a) const {
			if (*this == a || *this < a) {
				return false;
			}
			return true;
		}

		template <class X>
		bool operator<=(const Account<X, C> a) const {
			if (*this < a || *this == a) {
				return true;
			}
			return false;
		}

		template <class X>
		bool operator>=(const Account<X, C> a) const {
			if (*this > a || *this == a) {
				return true;
			}
			return false;
		}

		template <class X>
		T operator+(const X x) {
			if (is_arithmetic_v<X>) {
				T t(amt.value + x);
				return t;
			}
			return amt;
		}

		template <class X>
		T operator-(const X x) {
			if (is_arithmetic_v<X>) {
				T t(amt.value - x);
				return t;
			}
			return amt;
		}

		template <class X>
		T operator*(const X x) {
			if (is_arithmetic_v<X>) {
				T t(amt.value * x);
				return t;
			}
			return amt;
		}

		template <class X>
		T operator/(const X x) {
			if (is_arithmetic_v<X> && x != 0) {
				T t(amt.value / x);
				return t;
			}
			return amt;
		}

		template <class X>
		T operator%(const X x) {
			if (is_arithmetic_v<X>) {
				T t(amt.value % x);
				return t;
			}
			return amt;
		}

		template <class X>
		T operator+=(const X x) {
			if (is_arithmetic_v<X>) {
				amt.value += x;
			}
			return amt;
		}

		template <class X>
		T operator-=(const X x) {
			if (is_arithmetic_v<X>) {
				amt.value -= x;
			}
			return amt;
		}

		template <class X>
		T operator*=(const X x) {
			if (is_arithmetic_v<X>) {
				amt.value *= x;
			}
			return amt;
		}

		template <class X>
		T operator/=(const X x) {
			if (is_arithmetic_v<X> && x != 0) {
				amt.value /= x;
			}
			return amt;
		}

		template <class X>
		T operator%=(const X x) {
			if (is_arithmetic_v<X>) {
				amt.value %= x;
			}
			return amt;
		}

		template <class X>
		T operator++(const X x) {
			*this += 1;
			return amt;
		}

		template <class X>
		T operator--(const X x) {
			*this -= 1;
			return amt;
		}

		template <class Q>
		T deposit(const Q dep) {
			C c;
			if (typeid(dep) == typeid(amt)) {
				amt.value += dep.value;
			}
			else {
				amt.value += c(dep, amt).value;
			}
			return amt;
		}

		template <class Q>
		T withdraw(const Q wit) {
			C c;
			if (typeid(wit) == typeid(amt)) {
				amt.value -= wit.value;
			}
			else {
				amt.value -= c(wit, amt).value;
			}
			return amt;
		}

		T getAmt() const {
			return amt;
		}

		// Currency
		template <class R>
		Account<R, C> * currency(const R r_in) const {
			C c;
			if (typeid(R) == typeid(T)) {
				R r(amt.value);
				Account<R, Convert> * a = new Account<R, Convert>(r);
				return a;
			}
			R r(c(amt, r_in).value);
			Account<R, Convert> * a = new Account<R, Convert>(r);
			return a;
		}

		// Cast operator
		template <class Q>
		operator Account<Q, Convert>() const {
			C c;
			if (typeid(Q) == typeid(T)) {
				Account<Q, Convert> a(amt.value);
				return a;
			}
			Q q(c(amt, Q()));
			Account<Q, Convert> a(q);
			return a;
		}
};

int main() {
	// Test initialization
	Account<Dollar, Convert> a(1);
	Account<Pound, Convert> b(Pound(2.34));
	Account<Yen, Convert> c;
	Account<Euro, Convert> d(0.87);

	// Test deposit
	a.deposit(Dollar(1));
	assert(a.getAmt().value == 2);

	// Test deposit with different currency
	a.deposit(Pound(1));
	assert(a.getAmt().value == 3.37);

	// Test withdraw
	b.withdraw(Pound(1));
	assert(abs(b.getAmt().value - 1.34) < 0.01);
	
	// Test withdraw with different curreny
	b.withdraw(Euro(1));
	assert(abs(b.getAmt().value - 0.51) < 0.01);

	// Test copy constructors and assignment
	Account<Dollar, Convert> a2(a);
	Account<Dollar, Convert> a3;
	a3 = a;
	assert(a == a2 && a == a3);

	// Test comparison operators
	a = Account<Dollar, Convert>(1);
	assert(a == d);
	assert(a != b);
	assert(a > b);
	assert(b < a);
	assert(a >= d);
	assert(b <= a);

	// Test arithmetic operators
	b += 1.49f;
	assert(abs(b.getAmt().value - 2) < 0.01);
	assert(abs((b * 2).value - 4) < 0.01);
	b--;
	assert(abs(b.getAmt().value - 1) < 0.01);

	// Test currency function
	Account<Yen, Convert> * e = a.currency(Yen());
	assert(*e == a);
	delete e;

	// Test cast operator
	Account<Yen, Convert> f = d;
	assert(f == d);
	Account<Yen, Convert> g = f;
	assert(g == f);
}
