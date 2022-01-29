#include <iostream>
#include <cassert>
#include <string>
#include <coroutine>
#include <memory>
#include <cstdlib>
#include <chrono>
#include <sstream>
#include <random>

using namespace std;

enum myenum {credit, debit};

unsigned int random_char() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    return dis(gen);
}

std::string generate_hex(int len) {
    std::stringstream ss;
    for (auto i = 0; i < len; i++) {
        const auto rc = random_char();
        std::stringstream hexstream;
        hexstream << std::hex << rc;
        auto hex = hexstream.str();
        ss << (hex.length() < 2 ? '0' + hex : hex);
    }
    return ss.str();
}

struct transaction {
	long post_date; // post date of transaction
	string src_accountId;  // use UUIDs
	string dest_accountId;  // use UUIDs
	myenum tranType; // transaction type enum can be credit or debit
	double amount; // in dollars ranging [1..1000000]

	transaction() {
		post_date = duration_cast<chrono::microseconds>(
			chrono::system_clock::now().time_since_epoch()).count();
		src_accountId = generate_hex(4);
		dest_accountId = generate_hex(4);
		tranType = [](){
			int x = rand() % 2;
			if (x == 0) {
				return credit;
			}
			return debit;
		}();
		amount = (rand() % 999999) + 1;
	}

	transaction(const transaction &other) {
		post_date = other.post_date;
		src_accountId = other.src_accountId;
		dest_accountId = other.dest_accountId;
		tranType = other.tranType;
		amount = other.amount;
	}

	transaction &operator=(const transaction &other) {
		if (this == &other) {
			return *this;
		}
		post_date = other.post_date;
		src_accountId = other.src_accountId;
		dest_accountId = other.dest_accountId;
		tranType = other.tranType;
		amount = other.amount;
		return *this;
	}

	void print() const {
		string tran = [*this]() {
			if (tranType == credit) {
				return "credit";
			}
			return "debit";
		}();
		cout << "post_date: " << post_date << ", src_accountId: " << src_accountId <<
			", dest_accountId: " << dest_accountId << ", tranType: " << tran <<
			", amount: $" << amount << "\n";
	}
};

template<typename T>
struct Generator {
	struct promise_type;
	using handle_type = coroutine_handle<promise_type>;

	Generator(handle_type h): coro(h) {}

	handle_type coro;

	~Generator() {
		if (coro) coro.destroy();
	}

	Generator(const Generator&) = delete;

	Generator& operator=(const Generator&) = delete;

	Generator(Generator&& oth): coro(oth.coro) {
		oth.coro = nullptr;
	}

	Generator& operator=(Generator &&oth) {
		coro = oth.coro;
		oth.coro = nullptr;
		return *this;
	}

	transaction getNextValue() {
		coro.resume();
		return coro.promise().current_value;
	}

	struct promise_type {
		promise_type() {}

		~promise_type() {}

		suspend_always initial_suspend() {
			return {};
		}

		suspend_always final_suspend() noexcept {
			return {};
		}

		auto get_return_object() {
			return Generator{handle_type::from_promise(*this)};
		}

		suspend_always yield_value(transaction value) {
			current_value = value;
			return {};
		}

		void return_void() {}

		void unhandled_exception() {
			exit(1);
		}

		T current_value;
	};
};

Generator<transaction> getNext() {
	auto value = transaction();
	while (true) {
		co_yield value;
		value = transaction();
	}
}

int main() {
	auto gen = getNext();
	for (int i = 0; i <= 2; ++i) {
		auto val = gen.getNextValue();
		val.print();
	}
}
