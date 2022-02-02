#include <thread>
#include <vector>
#include <atomic>
#include <deque>
#include <future>
#include <mutex>
#include <condition_variable>
#include <memory>
#include <queue>
#include <cassert>
#include <cstdlib>
#include <numeric>
#include <functional>
#include <iostream>

using namespace std;

template<typename T>
class threadsafe_queue {
    private:
        mutable mutex mut;
        queue<T> data_queue;
        condition_variable data_cond;

    public:
        threadsafe_queue() {}

        threadsafe_queue(const threadsafe_queue &other) {
            lock_guard<mutex> lk(other.mut);
            data_queue = other.data_queue;
        }

        threadsafe_queue& operator=(const threadsafe_queue &) = delete;

        void push(T new_value, int index) {
            lock_guard<mutex> lk(mut);
            data_queue.push(new_value);
            data_cond.notify_one();
        }

        bool try_pop(T &value, int index) {
            lock_guard<mutex> lk(mut);
            if (data_queue.empty()) {
                return false;
            }
            value = data_queue.front();
            data_queue.pop();
            return true;
        }

        shared_ptr<T> try_pop(int index) {
            lock_guard<mutex> lk(mut);
            if (data_queue.empty()) {
                return shared_ptr<T>();
            }
            shared_ptr<T> res(make_shared<T>(data_queue.front()));
            data_queue.pop();
            return res;
        }

        void wait_and_pop(T &value, int index) {
            unique_lock<mutex> lk(mut);
            data_cond.wait(lk, [this] {return !data_queue.empty();});
            value = data_queue.front();
            data_queue.pop();
        }

        shared_ptr<T> wait_and_pop(int index) {
            unique_lock<mutex> lk(mut);
            data_cond.wait(lk, [this] {return !data_queue.empty();});
            shared_ptr<T> res(make_shared<T>(data_queue.front()));
            data_queue.pop();
            return res;
        }

        bool empty() const {
            lock_guard<mutex> lk(mut);
            return data_queue.empty();
        }
};

class function_wrapper {
    struct impl_base {
        virtual void call()=0;
        virtual ~impl_base() {}
    };
    std::unique_ptr<impl_base> impl;
    template<typename F>
    struct impl_type: impl_base {
        F f;
        impl_type(F&& f_): f(std::move(f_)) {}
        void call() { f(); }
    };
public:
    template<typename F>
    function_wrapper(F&& f): impl(new impl_type<F>(std::move(f))) {}

    void call() { impl->call(); }

    function_wrapper(function_wrapper&& other): impl(std::move(other.impl)) {}

    function_wrapper& operator=(function_wrapper&& other) {
        impl=std::move(other.impl);
        return *this;
    }

    function_wrapper(const function_wrapper&)=delete;
    function_wrapper(function_wrapper&)=delete;
    function_wrapper& operator=(const function_wrapper&)=delete;
};

struct join_threads {
    join_threads(std::vector<std::thread>&)
    {}
};

class thread_pool {
    std::atomic_bool done;
    threadsafe_queue<function_wrapper> work_queue;
    std::vector<std::thread> threads;
    join_threads joiner;

    void worker_thread() {
        while(!done) {
            std::function<void()> task;
            if(work_queue.try_pop(task)) {
                task();
            } else {
                std::this_thread::yield();
            }
        }
    }
public:
    template<typename FunctionType>
    std::future<typename std::result_of<FunctionType()>::type>
    submit(FunctionType f) {
        typedef typename std::result_of<FunctionType()>::type result_type;
        
        std::packaged_task<result_type()> task(std::move(f));
        std::future<result_type> res(task.get_future());
        work_queue.push_back(std::move(task));
        return res;
    }

    thread_pool(): done(false),joiner(threads) {
        unsigned const thread_count=std::thread::hardware_concurrency();
        try {
            for(unsigned i=0;i<thread_count;++i) {
                threads.push_back(
                    std::thread(&thread_pool::worker_thread,this));
            }
        } catch(...) {
            done=true;
            throw;
        }
    }

    ~thread_pool() {
        done=true;
    }
};

template<typename Iterator, typename T>
struct accumulate_block {
    T operator()(Iterator first, Iterator last) {
        return std::accumulate(first, last, 0);
    }
};

template<typename Iterator,typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
    unsigned long const length = std::distance(first, last);
    if (!length)
        return init;
    unsigned long const block_size = 25;
    unsigned long const num_blocks = (length + block_size-1)/block_size;
    std::vector<std::future<T> > futures(num_blocks-1);
    thread_pool pool;
    Iterator block_start = first;
    for (unsigned long i = 0;i < (num_blocks-1); ++i) {
        Iterator block_end = block_start;
        std::advance(block_end, block_size);
        futures[i] = pool.submit([=] {
            return accumulate_block<Iterator,T>()(block_start, block_end);
        });
        block_start = block_end;
    }
    T last_result = accumulate_block<Iterator,T>() (block_start,last);
    T result = init;
    for (unsigned long i = 0; i < (num_blocks-1); ++i) {
        result += futures[i].get();
    }
    result += last_result;
    return result;
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