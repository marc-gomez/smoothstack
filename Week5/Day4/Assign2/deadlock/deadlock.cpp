#include <iostream>
#include <thread>
#include <mutex>
#include <cstdlib>

using namespace std;

enum Currency {Dollars, Pounds};
enum Choices {Buy, Sell};

struct Request {
	double rate;
	double amount;
	int type;
	int choice;
};

class Country {
	private:
		int id;
		double dollars;
		double pounds;
		mutable mutex mut;

	public:
		Country(int id_in) : id(id_in), dollars(500000), pounds(500000) {}

		void trade(Country &other, const Request &r) {
			lock_guard<mutex> lock(mut);

			// Check available funds and simulate the offered Country's response
			if ((r.choice == Sell && r.type == Dollars && dollars < r.amount) ||
				(r.choice == Sell && r.type == Pounds && pounds < r.amount) ||
				(r.choice == Buy && r.type == Dollars && pounds < r.amount / r.rate) ||
				(r.choice == Buy && r.type == Pounds && dollars < r.amount / r.rate) ||
				!other.request(r)) {
				cout << "Country " << other.id << "'s request to Country " << id <<
					" is cancelled" << endl << endl;
			}
			else {
				if (r.choice == Sell) {
					if (r.type == Dollars) {
						dollars -= r.amount;
						pounds += r.amount / r.rate;
						cout << "Country " << id << " sells " << r.amount <<
							" dollars to Country " << other.id << " for " <<
							r.amount / r.rate << " pounds" << endl << endl;
					}
					else {
						pounds -= r.amount;
						dollars += r.amount / r.rate;
						cout << "Country " << id << " sells " << r.amount <<
							" pounds to Country " << other.id << " for " <<
							r.amount / r.rate << " dollars" << endl << endl;
					}
				}
				else {
					if (r.type == Dollars) {
						dollars += r.amount;
						pounds -= r.amount / r.rate;
						cout << "Country " << id << " buys " << r.amount <<
							" dollars from Country " << other.id << " for " <<
							r.amount / r.rate << " pounds" << endl << endl;
					}
					else {
						pounds += r.amount;
						dollars -= r.amount / r.rate;
						cout << "Country " << id << " buys " << r.amount <<
							" pounds from Country " << other.id << " for " <<
							r.amount / r.rate << " dollars" << endl << endl;
					}
				}
			}
		}

		bool request(const Request &r) {
			lock_guard<mutex> lock(mut);

			if ((r.rate < 1.3 && r.type == Dollars) ||
				(r.rate > 0.8 && r.type == Pounds) ||
				(r.type == Dollars && r.choice == Sell && pounds < r.amount / r.rate) ||
				(r.type == Dollars && r.choice == Buy && dollars < r.amount) ||
				(r.type == Pounds && r.choice == Sell && dollars < r.amount / r.rate) ||
				(r.type == Pounds && r.choice == Buy && pounds < r.amount)) {

				return false;
			}
			else {
				if (r.choice == Sell) {
					if (r.type == Dollars) {
						dollars += r.amount;
						pounds -= r.amount / r.rate;
					}
					else {
						pounds += r.amount;
						dollars -= r.amount / r.rate;
					}
				}
				else {
					if (r.type == Dollars) {
						dollars -= r.amount;
						pounds += r.amount / r.rate;
					}
					else {
						pounds -= r.amount;
						dollars += r.amount / r.rate;
					}
				}
			}

			return true;
		}

		void print() {
			cout << "Country: " << id << endl;
			cout << "Dollars: " << dollars << endl;
			cout << "Pounds: " << pounds << endl;
		}

};

Country c1(1);
Country c2(2);
Request r1 = {1.55, 200000, Dollars, Buy};
Request r2 = {1.55, 200000, Dollars, Sell};
Request r3 = {0.65, 130000, Pounds, Buy};
Request r4 = {0.65, 130000, Pounds, Sell};

void c1trades() {
	for (int i = 0; i < 500; ++i) {
		int t = (rand() %  4) +1;
		if (t == 1) {
			c1.trade(c2, r1);
		}
		else if (t == 2) {
			c1.trade(c2, r2);
		}
		else if (t == 3) {
			c1.trade(c2, r3);
		}
		else {
			c1.trade(c2, r4);
		}
	}
}

void c2trades() {
	for (int i = 0; i < 500; ++i) {
		int t = (rand() %  4) +1;
		if (t == 1) {
			c2.trade(c1, r1);
		}
		else if (t == 2) {
			c2.trade(c1, r2);
		}
		else if (t == 3) {
			c2.trade(c1, r3);
		}
		else {
			c2.trade(c1, r4);
		}
	}
}

int main() {
	// Print starting state
	c1.print();
	cout << endl;
	c2.print();
	cout << endl;

	thread t1(c1trades);
	thread t2(c2trades);
	t1.join();
	t2.join();

	// Print ending state
	c1.print();
	cout << endl;
	c2.print();
}
