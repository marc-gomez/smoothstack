#include <future>
#include <algorithm>
#include <vector>
#include <iostream>
#include <iterator>
#include <cassert>

struct join_threads {
	std::vector<std::thread> &myvec;
	join_threads(std::vector<std::thread> &vec) : myvec(vec) {}
	~join_threads() {
		for (auto &ele : myvec) {
			ele.join();
		}
	}
};

template<typename Iterator, class UnaryPredicate>
bool parallel_any_of(Iterator first, Iterator last, UnaryPredicate pred) {
	struct find_element {
		void operator()(Iterator begin, Iterator end, UnaryPredicate pred,
			std::promise<bool> * result,
			std::atomic<bool> * done_flag) {
			try {
				for (;(begin != end) && !done_flag->load(); ++begin) {
					if (pred(*begin)) {
						result->set_value(true);
						done_flag->store(true);
						return;
					}
				}
			} catch(...) {
				try {
					result->set_exception(std::current_exception());
					done_flag->store(true);
				} catch(...) {}
			}
		}
	};

	unsigned long const length = std::distance(first, last);

	if (!length) {
		return false;
	}
	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length+min_per_thread-1)/min_per_thread;

	unsigned long const hardware_threads = std::thread::hardware_concurrency();

	unsigned long const num_threads = std::min(hardware_threads!=0?hardware_threads:2,max_threads);

	unsigned long const block_size = length/num_threads;

	std::promise<bool> result;
	std::atomic<bool> done_flag(false);
	std::vector<std::thread> threads(num_threads-1);
	{
		join_threads joiner(threads);

		Iterator block_start = first;
		for (unsigned long i = 0; i < (num_threads-1); ++i) {
			Iterator block_end = block_start;
			std::advance(block_end, block_size);
			threads[i] = std::thread(find_element(), block_start, block_end, pred, &result, &done_flag);
			block_start = block_end;
		}
		find_element()(block_start, last, pred, &result, &done_flag);
	}
	if (!done_flag.load()) {
		return false;
	}

	return result.get_future().get();
}

int main(void) {
	std::vector<std::string> myvec;
	for (int ii = 0; ii < 10000; ii++) {
		myvec.push_back("hello");
	}

	bool found = parallel_any_of(myvec.begin(), myvec.end(), [](std::string s){return s == "world";});
	assert(!found);

	std::vector<std::string>::iterator iter = myvec.begin();
	iter += 5000;
	myvec.insert(iter, "world");

	found = parallel_any_of(myvec.begin(), myvec.end(), [](std::string s){return s == "world";});
	assert(found);
}

// N is the size of the vector
// k is the number of threads

// Best case is when any_of() returns true at the start of a block

// Worst case is when any_of() returns false. k threads will have to check N elements in total. log_k(N) steps.
// A sequential version will take N steps