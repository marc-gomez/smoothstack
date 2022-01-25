#include <vector>
#include <thread>
#include <future>
#include <numeric>
#include <iostream>
#include <cassert>

template<typename Iterator, typename T>
struct accumulate_block {
	void operator()(Iterator first, Iterator last, std::promise<T> &p) {
		T result = std::accumulate(first, last, 0);
		p.set_value(result);
	}
};

template<typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
	unsigned long const length = std::distance(first, last);
	if (!length) {
		return init;
	}
	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length+min_per_thread-1)/min_per_thread;
	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	unsigned long const num_threads = std::min(hardware_threads!=0?hardware_threads:2,max_threads);
	std::cout << "num_threads: " << num_threads << std::endl;
	unsigned long const block_size = length/num_threads;

	std::vector< std::promise<T> > promises(num_threads);
	std::vector< std::future<T> > futures(num_threads);
	for (int i = 0; i < num_threads; ++i) {
		futures[i] = promises[i].get_future();
	}
	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads-1);
	Iterator block_start = first;

	for (unsigned long i = 0; i < (num_threads-1); ++i) {
		Iterator block_end = block_start;
		std::advance(block_end, block_size);
		threads[i] = std::thread(accumulate_block<Iterator,T>(),
				block_start, block_end, std::ref(promises[i]));
		block_start = block_end;
	}

	accumulate_block<Iterator,T>() (block_start, last, promises[num_threads-1]);

	for(int i = 0; i < num_threads; ++i) {
		results[i] = futures[i].get();
	}

	for (auto &entry : threads) {
		entry.join();
	}

	return std::accumulate(results.begin(), results.end(), init);
}

int main() {
	constexpr long LEN = 100;
	std::vector<long> myvec;
	for (long ii = 0; ii < LEN; ii++) {
		myvec.push_back(ii);
	}
	long sum = parallel_accumulate(myvec.begin(), myvec.end(), 0);
	std::cout << "sum: " << sum << std::endl;
	assert(sum == 4950);
}
