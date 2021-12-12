#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;

class Account {
	public:
	Account(string name_in, int id_in, int ssn_in) :
		name{name_in}, timer(time(nullptr)), id{id_in}, ssn{ssn_in} {
		}

	void showAccount() {
		cout << "Customer name: " << name << endl;
		cout << "Account: " << id << endl;
	       	cout << "Date Opened: " << asctime(localtime(&timer)) << endl;
	}

	void displayAccount() {
		cout << "Customer name: " << name << endl;
		cout << "SSN: " << "XXX-XX-" << ssn % 10000 << endl;
		cout << "Date Opened: " << asctime(localtime(&timer)) << endl;
	}

	string getName() {
		return name;
	}

	int getId() {
		return id;
	}

	int getSSN() {
		return ssn;
	}

	private:
	string name;
	time_t timer;
	int id;
	int ssn;
};

void printCommands() {
	cout << "Console commands:" << endl << endl;
	cout << "1. \"show accounts\"" << endl;
	cout << "2. \"display account\"" << endl;
	cout << "3. \"search name\"" << endl;
	cout << "4. \"new account\"" << endl;
	cout << "5. \"close account\"" << endl;
	cout << "6. \"quit\"" << endl << endl;
}

int stringSimilarity(const string &s1, const string &s2) {
	vector< vector<int> > valTable(s1.size() + 1, vector<int>(s2.size() + 1));

	valTable[0][0] = 0;
	for (int i = 1; i <= s1.size(); ++i) {
		valTable[i][0] = i;
	}
	for (int i = 1; i <= s2.size(); ++i) {
		valTable[0][i] = i;
	}

	for(int i = 1; i <= s1.size(); ++i) {
		for(int j = 1; j <= s2.size(); ++j) {
			valTable[i][j] =
				min({ valTable[i-1][j] + 1,
						valTable[i][j-1] + 1,
						valTable[i-1][j-1] + (s1[i-1] == s2[j-1] ? 0 : 1) });
		}
	}
	return valTable[s1.size()][s2.size()];
}

int main() {

	// Initialize dummy account data
	map<string, string> userList;
	userList["Bob"] = "password1";
	userList["Lisa J"] = "password2";
	userList["user1"] = "password3";

	vector<Account*> accounts;
	int totalAccs = 0;
	accounts.push_back(new Account("Jane Doe", totalAccs, 123456789));
	totalAccs++;
	accounts.push_back(new Account("John Smith", totalAccs, 987654321));
	totalAccs++;
	accounts.push_back(new Account("Bob Lee", totalAccs, 123454321));
	totalAccs++;

	// User authentication
	string username;
	string password;
	map<string, string>::iterator userIt;
	while (true) {
		cout << "Username: ";
		getline(cin, username);
		cout << "Password: ";
		getline(cin, password);

		userIt = userList.find(username);

		if(userIt != userList.end() && password == userIt->second) {
			cout << "Login successful" << endl << endl;
			break;
		}

		cout << "Incorrect user" << endl << endl;
	}

	// Console prompt
	printCommands();
	while(true) {
		string commInput = "";
		cout << "> ";
		while (commInput.compare("") == 0) {
			getline(cin, commInput);
		}

		if (commInput.compare("show accounts") == 0) {
			cout << endl;
			for (int i = 0; i < accounts.size(); ++i) {
				accounts[i]->showAccount();
			}
		}
		else if (commInput.compare("display account") == 0) {
			cout << "account > ";
			int accountNum;
			cin >> accountNum;
			bool found = false;
			for(int i = 0; i < accounts.size(); ++i) {
				if (accounts[i]->getId() == accountNum) {
					cout << endl;
					accounts[i]->displayAccount();
					found = true;
					break;
				}
			}
			if (!found) {
				cout << endl << "Invalid account" << endl << endl;
			}
		}
		else if (commInput.compare("search name") == 0) {
			cout << "name > ";
			string nameSearch;
			getline(cin, nameSearch);
			vector<Account*> matches;
			for(int i = 0; i < accounts.size(); ++i) {
				if(stringSimilarity(nameSearch, accounts[i]->getName()) < 4) {
					matches.push_back(accounts[i]);
				}
			}
			for(int i = 0; i < matches.size(); ++i) {
				cout << i+1 << ". " << matches[i]->getName() << endl;
			}
			cout << matches.size()+1 << ". Home" << endl << endl;
			
			int chosen = -1;
			while (chosen <= 0 || chosen > matches.size()+1) {
				cout << "Select number > ";
				cin >> chosen;
				if (chosen <= 0 || chosen > matches.size()+1) {
					cout << "Invalid choice" << endl;
				}
			}
			if(chosen != matches.size()+1) {
				matches[chosen-1]->displayAccount();
			}

		}
		else if (commInput.compare("new account") == 0) {
			cout << "Customer Name > ";
			string newName;
			getline(cin, newName);
			cout << "SSN > ";
			int newSSN;
			cin >> newSSN;
			bool exists = false;
			for (int i = 0; i < accounts.size(); ++i) {
				if (accounts[i]->getSSN() == newSSN) {
					exists = true;
					break;
				}
			}
			if (exists) {
				cout << "Account exists" << endl;
			}
			else {
				accounts.push_back(new Account(newName, totalAccs, newSSN));
				++totalAccs;
				cout << endl << "Account created" << endl << endl;
			}
		}
		else if (commInput.compare("close account") == 0) {
			cout << "account > ";
			int toDelete;
			cin >> toDelete;
			bool found = false;
			int index;
			for (index = 0; index < accounts.size(); ++index) {
				if(accounts[index]->getId() == toDelete) {
					found = true;
					break;
				}
			}
			if (found) {
				cout << "Are you sure you want to delete this account? (yes/no)" << endl;
				string confirmation;
				cin >> confirmation;
				if (confirmation.compare("yes") == 0) {
					Account * temp = accounts[index];
					accounts.erase(accounts.begin()+index);
					delete temp;
					cout << endl << "Account deleted" << endl << endl;
				}
			}
			else {
				cout << "Invalid account" << endl;
			}
		}
		else if (commInput.compare("quit") == 0) {
  			break;	
		}
		else {
			cout << "Invalid command: " << commInput << endl;
		}
	}

	// Clean up memory
	for(int i = 0; i < accounts.size(); ++i) {
		delete accounts[i];
		accounts[i] = nullptr;
	}

	return 0;

}
