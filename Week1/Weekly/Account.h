#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <iostream>
#include <iomanip>
#include <time.h>

using namespace std;

class Account {
public:
	Account(string name_in, int id_in, int ssn_in) :
		name{name_in}, id{id_in}, ssn{ssn_in} { //timer(time(nullptr))
		
			struct tm y2k;
			y2k.tm_hour = 0;
			y2k.tm_min = 0;
			y2k.tm_sec = 0;
			y2k.tm_year = 100;
			y2k.tm_mon = 0;
			y2k.tm_mday = 1;
			timer = mktime(&y2k);
		}

	// Print customer name, account id, and date opened
	void showAccount() {
		cout << "Customer name: " << name << endl;
		cout << "Account: " << id << endl;
		cout << "Date Opened: " << asctime(localtime(&timer)) << endl;
	}

	// Print customer name, ssn, and date opened
	void displayAccount() {
		cout << "Customer name: " << name << endl;
		cout << "SSN: " << "XXX-XX-" << setw(4) << setfill('0') << ssn % 10000 << endl;
		cout << "Date Opened: " << asctime(localtime(&timer)) << endl;
	}

	// Return customer's name
	string getName() {
		return name;
	}

	// Return account's ID
	int getID() {
		return id;
	}

	// Return customer's SSN
	int getSSN() {
		return ssn;
	}

private:
	string name;
	time_t timer;
	int id;
	int ssn;
};

#endif
