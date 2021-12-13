#include <iostream>
#include <string>
#include <map>
#include <ctime>
#include <vector>
#include <deque>
#include <algorithm>
#include <iomanip>
#include "Account.h"

using namespace std;

// Print available console commands
void printCommands() {
	cout << "Console commands:" << endl << endl;
	cout << "1. \"show accounts\"" << endl;
	cout << "2. \"display account\"" << endl;
	cout << "3. \"search name\"" << endl;
	cout << "4. \"new account\"" << endl;
	cout << "5. \"close account\"" << endl;
	cout << "6. \"quit\"" << endl << endl;
}

// Return the Levenshtein distance between two strings
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

// When user enters "show accounts"
// Print a list of all users' names, id, and date opened
void showCommand(vector<Account*> &accs) {
	cout << endl;
	for (int i = 0; i < accs.size(); ++i) {
		accs[i]->showAccount();
	}
}

// When user enters "display account"
// Print a specific user's name, ssn, and date opened
void displayCommand(vector<Account*> &accs) {
	cout << "account > ";
	int accountNum;
	cin >> accountNum;
	bool found = false;
	for(int i = 0; i < accs.size(); ++i) {
		if (accs[i]->getID() == accountNum) {
			cout << endl;
			accs[i]->displayAccount();
			found = true;
			break;
		}
	}
	if (!found) {
		cout << endl << "Invalid account" << endl << endl;
	}
}

// When user enters "search name"
// Print a list of similar names to the user input name,
// Then print a chosen user's name, ssn, and date opened
void searchCommand(vector<Account*> &accs) {
	cout << "name > ";
	string nameSearch;
	getline(cin, nameSearch);
	deque<Account*> matches;
	for(int i = 0; i < accs.size(); ++i) {
		int similarity = stringSimilarity(nameSearch, accs[i]->getName());
		if(similarity == 0) {
			matches.push_front(accs[i]);
		}
		else if(similarity < 4) {
			matches.push_back(accs[i]);
		}
	}

	cout << endl;
	for(int i = 0; i < matches.size(); ++i) {
		cout << i+1 << ". " << matches[i]->getName() << endl;
	}
	cout << matches.size()+1 << ". Home" << endl << endl;
			
	int chosen = -1;
	while (chosen <= 0 || chosen > matches.size()+1) {
		cout << "Select number > ";
		cin >> chosen;
		if (chosen <= 0 || chosen > matches.size()+1) {
			cout << endl << "Invalid choice" << endl << endl;
		}
	}

	cout << endl;
	if(chosen != matches.size()+1) {
		matches[chosen-1]->displayAccount();
	}
}

// When user enters "new account"
// Create a new account with given information if the SSN doesn't already exist
void newCommand(vector<Account*> &accs, int &count) {
	cout << "Customer Name > ";
	string newName;
	getline(cin, newName);

	cout << "SSN > ";
	int newSSN;
	cin >> newSSN;

	bool exists = false;
	for (int i = 0; i < accs.size(); ++i) {
		if (accs[i]->getSSN() == newSSN) {
			exists = true;
			break;
		}
	}

	if (exists) {
		cout << endl << "Account exists" << endl << endl;
	}
	else {
		accs.push_back(new Account(newName, count, newSSN));
		++count;
		cout << endl << "Account created" << endl << endl;
	}
}

// When user enters "close account"
// Close an account, with confirmation
void closeCommand(vector<Account*> &accs) {
	cout << "account > ";
	int toDelete;
	cin >> toDelete;

	bool found = false;
	int index;
	for (index = 0; index < accs.size(); ++index) {
		if(accs[index]->getID() == toDelete) {
			found = true;
			break;
		}
	}

	if (found) {
		cout << endl << "Are you sure you want to delete this account? (yes/no)" << endl;
		string confirmation;
		cin >> confirmation;

		if (confirmation.compare("yes") == 0 || confirmation.compare("y") == 0) {
			Account * temp = accs[index];
			accs.erase(accs.begin()+index);
			delete temp;
			cout << endl << "Account deleted" << endl << endl;
		}
	}
	else {
		cout << endl << "Invalid account" << endl << endl;
	}
}

// Authorize admin login atempt
// Return true if login is successful
bool authorize(map<string, string> &list) {
	string username;
	cout << "Username: ";
	getline(cin, username);

	string password;
	cout << "Password: ";
	getline(cin, password);

	map<string, string>::iterator adminIt = list.find(username);
	if(adminIt != list.end() && password == adminIt->second) {
		cout << endl << "Login successful" << endl << endl;
		return true;
	}

	cout << endl << "Incorrect user" << endl << endl;
	return false;
}

int main() {

	// Initialize dummy account data
	map<string, string> adminList;
	adminList["Bob"] = "password1";
	adminList["Lisa J"] = "password2";
	adminList["user1"] = "password3";

	vector<Account*> accounts;
	int totalAccs = 0;

	accounts.push_back(new Account("Jane Doe", totalAccs, 123450000));
	totalAccs++;
	accounts.push_back(new Account("John Smith", totalAccs, 987654321));
	totalAccs++;
	accounts.push_back(new Account("Bob Lee", totalAccs, 123454321));
	totalAccs++;	
	accounts.push_back(new Account("Bob Parson", totalAccs, 123464321));
	totalAccs++;
	accounts.push_back(new Account("Bob Parsons", totalAccs, 123454321));
	totalAccs++;
	accounts.push_back(new Account("Bobby Parsons", totalAccs, 123454721));
	totalAccs++;

	// User authentication
	bool authorized = false;
	while (!authorized) {
		authorized = authorize(adminList);
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
			showCommand(accounts);
		}

		else if (commInput.compare("display account") == 0) {
			displayCommand(accounts);
		}

		else if (commInput.compare("search name") == 0) {
			searchCommand(accounts);
		}

		else if (commInput.compare("new account") == 0) {
			newCommand(accounts, totalAccs);
		}

		else if (commInput.compare("close account") == 0) {
			closeCommand(accounts);
		}

		else if (commInput.compare("quit") == 0) {
  			break;	
		}

		// When user enters none of the above commands
		else {
			cout << endl << "Invalid command: " << commInput << endl << endl;
		}
	}

	// Clean up memory
	for(int i = 0; i < accounts.size(); ++i) {
		delete accounts[i];
		accounts[i] = nullptr;
	}

	return 0;

}
