#include <vector>
#include <algorithm>
#include <numeric>
#include <thread>
#include <future>
#include <cassert>

template <typename RandomIt>
void parallel_sum(RandomIt beg, RandomIt end, std::promise<int> acc_promise) {
	auto len = end - beg;

	if (len < 1000) {
		int sum = std::accumulate(beg, end, 0);
		acc_promise.set_value(sum);
		return;
	}

	RandomIt mid = beg + len/2;

	std::promise<int> acc_promise_rec;
	std::future<int> acc_future_rec = acc_promise_rec.get_future();
	std::thread work_thread(parallel_sum<RandomIt>, mid, end, std::move(acc_promise_rec));

	std::promise<int> acc_promise_this;
	std::future<int> acc_future_this = acc_promise_this.get_future();
	parallel_sum<RandomIt>(mid, end, std::move(acc_promise_this));

	acc_promise.set_value(acc_future_this.get() + acc_future_rec.get());
	work_thread.join();
}

int main() {
	std::vector<int> v(2000, 1);
	std::promise<int> acc_promise;
	std::future<int> acc_future = acc_promise.get_future();
	std::thread work_thread(parallel_sum<std::vector<int>::iterator>,
		v.begin(), v.end(), std::move(acc_promise));
	assert(acc_future.get() == 2000);
	work_thread.join();
}
