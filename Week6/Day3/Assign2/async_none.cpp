#include <atomic>
#include <future>
#include <vector>
#include <iterator>
#include <cassert>

template<typename Iterator, typename UnaryPredicate>
bool parallel_find_impl(Iterator first, Iterator last, UnaryPredicate pred, std::atomic<bool> &done) {
	try {
		unsigned long const length = std::distance(first, last);
		unsigned long const min_per_thread = 25;
		if (length < (2 * min_per_thread)) {
			for (;(first != last) && !done.load(); ++first) {
				if (pred(*first)) {
					done = true;
					return false;
				}
			}
			return true;
		} else {
			Iterator const mid_point = first + (length/2);
			std::future<bool> async_result =
				std::async(&parallel_find_impl<Iterator,UnaryPredicate>,
					mid_point, last, pred, std::ref(done));
			bool const direct_result = parallel_find_impl(first, mid_point, pred, done);
			return direct_result ? async_result.get() : direct_result;
		}
	}
	catch(...) {
		done = true;
		throw;
	}
}

template<typename Iterator, typename UnaryPredicate>
bool parallel_none_of(Iterator first, Iterator last, UnaryPredicate pred) {
	std::atomic<bool> done(false);
	return parallel_find_impl(first, last, pred, done);
}

int main(void) {
	std::vector<std::string> myvec;
	for (int ii = 0; ii < 10000; ii++) {
		myvec.push_back("hello");
	}

	auto predicate = [](std::string s){return s == "world";};

	bool not_in = parallel_none_of(myvec.begin(), myvec.end(), predicate);
	assert(not_in);

	std::vector<std::string>::iterator iter = myvec.begin();
	iter += 5000;
	myvec.insert(iter, "world");

	not_in = parallel_none_of(myvec.begin(), myvec.end(), predicate);
	assert(!not_in);
}

// N is the size of the vector
// k is the number of threads

// Best case is when none_of() returns false at the start of a block.

// Worst case is when none_of() returns true. k threads will have to check N elements in total. log_k(N) steps.
// A sequential version will take N steps