#include <iostream>
#include <cassert>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <cstdlib>

using namespace std;

template<typename T>
class threadsafe_queue {
	private:
		mutable mutex mut;
		queue<T> data_queue;
		condition_variable data_cond;

		void print(){
			queue<T> copy(data_queue);
			cout << "line: ";
			while(!copy.empty()) {
				cout << copy.front() << " ";
				copy.pop();
			}
			cout << endl << endl;
		}

	public:
		threadsafe_queue() {}

		threadsafe_queue(const threadsafe_queue &other) {
			lock_guard<mutex> lk(other.mut);
			data_queue = other.data_queue;
		}

		threadsafe_queue& operator=(const threadsafe_queue &) = delete;

		void push(T new_value, int index) {
			lock_guard<mutex> lk(mut);
			cout << "Thread " << index << " pushing " << new_value << endl;
			data_queue.push(new_value);
			print();
			data_cond.notify_one();
		}

		bool try_pop(T &value, int index) {
			lock_guard<mutex> lk(mut);
			if (data_queue.empty()) {
				return false;
			}
			value = data_queue.front();
			cout << "Thread " << index << " popping " << value << endl;
			data_queue.pop();
			print();
			return true;
		}

		shared_ptr<T> try_pop(int index) {
			lock_guard<mutex> lk(mut);
			if (data_queue.empty()) {
				return shared_ptr<T>();
			}
			shared_ptr<T> res(make_shared<T>(data_queue.front()));
			cout << "Thread " << index << " popping " << *res << endl;
			data_queue.pop();
			print();
			return res;
		}

		void wait_and_pop(T &value, int index) {
			unique_lock<mutex> lk(mut);
			data_cond.wait(lk, [this] {return !data_queue.empty();});
			value = data_queue.front();
			cout << "Thread " << index << " popping " << value << endl;
			data_queue.pop();
			print();
		}

		shared_ptr<T> wait_and_pop(int index) {
			unique_lock<mutex> lk(mut);
			data_cond.wait(lk, [this] {return !data_queue.empty();});
			shared_ptr<T> res(make_shared<T>(data_queue.front()));
			cout << "Thread " << index << " popping " << *res << endl;
			data_queue.pop();
			print();
			return res;
		}

		bool empty() const {
			lock_guard<mutex> lk(mut);
			return data_queue.empty();
		}
};

// assert that a popped value is within the mod 100 universe
void check_val(int val) {
	assert(val >= 0 && val < 100);
}

void handle_queue(threadsafe_queue<int> &q, int index) {
	// push 4 random values
	for (int i = 0; i < 4; ++i) {
		q.push(rand() % 100, index);
	}

	// then pop all 4 using different methods
	int popped;

	q.try_pop(popped, index);
	check_val(popped);

	shared_ptr<int> p_ptr = q.try_pop(index);
	check_val(*p_ptr);

	q.wait_and_pop(popped, index);
	check_val(popped);

	p_ptr = q.wait_and_pop(index);
	check_val(*p_ptr);
}

int main() {
	threadsafe_queue<int> line;

	thread t1(handle_queue, ref(line), 0);
	thread t2(handle_queue, ref(line), 1);

	t1.join();
	t2.join();
}
