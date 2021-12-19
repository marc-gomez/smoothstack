#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <google/protobuf/util/time_util.h>
#include <ctime>
#include <vector>
#include <deque>
#include <algorithm>
#include <iomanip>
#include "bankaccounts.pb.h"

using namespace std;

using google::protobuf::util::TimeUtil;

ofstream logfile;

// Return true if given username and password are in the AdminList.
bool inAdminList(const bank::AdminList& admin_list, const string &username, const string &password) {
  for (int i = 0; i < admin_list.admins_size(); i++) {
    const bank::Admin& admin = admin_list.admins(i);

    if(admin.username().compare(username) == 0 && admin.password().compare(password) == 0) {
    	return true;
    }

  }
  return false;
}

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
void showCommand(bank::AccountList &account_list) {
	cout << endl;

	for (int i = 0; i < account_list.accounts_size(); i++) {
		const bank::Account& account = account_list.accounts(i);

		cout << "Customer name: " << account.name() << endl;
		cout << "Account: " << account.id() << endl;
		cout << "Date Opened: " << account.date_opened() << endl << endl;

	}

	logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
		"Admin viewed the list of all accounts" << endl;
}

// This function fills in an Account message based on user input.
void displayAccount(const bank::Account* account) {
	cout << "Customer name: " << account->name() << endl;
	cout << "SSN: " << "XXX-XX-" << setw(4) << setfill('0') << account->ssn() % 10000 << endl;
	cout << "Date Opened: " << account->date_opened() << endl << endl;
	logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
		"Admin displayed account info for user with id " << account->id() << endl;
}

// When user enters "display account"
// Print a specific user's name, ssn, and date opened
void displayCommand(bank::AccountList &account_list) {
	cout << "account > ";
	int accountNum;
	cin >> accountNum;
	bool found = false;


	for (int i = 0; i < account_list.accounts_size(); i++) {
		const bank::Account& account = account_list.accounts(i);

		if (account.id() == accountNum) {
			cout << endl;
			displayAccount(&account);
			found = true;
			break;
		}

	}

	if (!found) {
		cout << endl << "Invalid account" << endl << endl;
		logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
			"Admin attempted to display account info for user with id " << accountNum << endl;
	}
}

// When user enters "search name"
// Print a list of similar names to the user input name,
// Then print a chosen user's name, ssn, and date opened
void searchCommand(bank::AccountList &account_list) {
	cout << "name > ";
	string nameSearch;
	getline(cin, nameSearch);
	deque<bank::Account> matches;

	for(int i = 0; i < account_list.accounts_size(); ++i) {
		const bank::Account& account = account_list.accounts(i);
		int similarity = stringSimilarity(nameSearch, account.name());

		if(similarity == 0) {
			matches.push_front(account);
		}
		else if(similarity < 4) {
			matches.push_back(account);
		}
	}

	cout << endl;
	for(int i = 0; i < matches.size(); ++i) {
		cout << i+1 << ". " << matches[i].name() << endl;
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
		logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
			"Admin searched for user with id " << matches[chosen-1].id() << endl;
		displayAccount(&matches[chosen-1]);
	}

}

// This function fills in an Account message based on user input.
void addAccount(bank::Account* account, int ssn, const string &name, int &count) {
	account->set_id(count);
	account->set_ssn(ssn);
	*account->mutable_name() = name;
	*account->mutable_date_opened() = TimeUtil::SecondsToTimestamp(time(NULL));
	logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
		"Admin successfully created user account with ssn " << ssn << endl;
}

// When user enters "new account"
// Create a new account with given information if the SSN doesn't already exist
void newCommand(bank::AccountList &account_list, int &count) {
	cout << "Customer Name > ";
	string newName;
	getline(cin, newName);

	cout << "SSN > ";
	int newSSN;
	cin >> newSSN;

	bool exists = false;
	for (int i = 0; i < account_list.accounts_size(); i++) {
		const bank::Account& account = account_list.accounts(i);

		if (account.ssn() == newSSN) {
			exists = true;
			break;
		}

	}

	if (exists) {
		cout << endl << "Account exists" << endl << endl;
		logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
			"Admin failed to add account with ssn" << newSSN << endl;
	}
	else {
		addAccount(account_list.add_accounts(), newSSN, newName, count);
		++count;
		cout << endl << "Account created" << endl << endl;
	}
}

// When user enters "close account"
// Close an account, with confirmation
void closeCommand(bank::AccountList &account_list) {
	cout << "account > ";
	int toDelete;
	cin >> toDelete;

	bool found = false;
	int index;
	for (index = 0; index < account_list.accounts_size(); ++index) {
		const bank::Account& account = account_list.accounts(index);
		if(account.id() == toDelete) {
			found = true;
			break;
		}
	}

	if (found) {
		cout << endl << "Are you sure you want to delete this account? (yes/no)" << endl;
		string confirmation;
		cin >> confirmation;

		if (confirmation.compare("yes") == 0 || confirmation.compare("y") == 0) {
			account_list.mutable_accounts()->DeleteSubrange(index, 1);
			cout << endl << "Account deleted" << endl << endl;
			logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
				"Admin deleted user" << toDelete << endl;
		}
	}
	else {
		cout << endl << "Invalid account" << endl << endl;
		logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
			"Admin attempted to delete invalid user " << toDelete << endl;
	}

}

// Authorize admin login atempt
// Return true if login is successful
bool authorize(const bank::AdminList &admin_list) {
	string username;
	cout << "Username: ";
	getline(cin, username);

	string password;
	cout << "Password: ";
	getline(cin, password);

	if(inAdminList(admin_list, username, password)) {
		cout << endl << "Login successful" << endl << endl;
			logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
				"Successful login, username: " << username << endl;
		return true;
	}

	cout << endl << "Incorrect user" << endl << endl;
	logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
		"Failed login attempt, username: " << username << endl;
	return false;
}

int main() {

	cout << endl;

	logfile.open("logfile.txt", ios_base::app);
	if (!logfile.is_open()) {
			cerr << "Failed to open log file." << endl;
			return -1;
	}

	// Verify that the version of the library that we linked against is
	// compatible with the version of the headers we compiled against.
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	bank::AdminList admin_list;

	{
		// Read the existing admin list.
		fstream input("admin_list", ios::in | ios::binary);
		if (!admin_list.ParseFromIstream(&input)) {
			cerr << "Failed to parse admin list." << endl;
			return -1;
		}
	}

	bank::AccountList account_list;

	{
		// Read the existing account list.
		fstream input("account_list", ios::in | ios::binary);
		if (!input) {
			cout << "account_list: File not found.  Creating a new file." << endl;
		} else if (!account_list.ParseFromIstream(&input)) {
			cerr << "Failed to parse address book." << endl;
			return -1;
		}
	}

	int totalAccs = account_list.accounts_size();

	// Admin authentication
	bool authorized = false;
	while (!authorized) {
		authorized = authorize(admin_list);
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
			showCommand(account_list);
		}

		else if (commInput.compare("display account") == 0) {
			displayCommand(account_list);
		}

		else if (commInput.compare("search name") == 0) {
			searchCommand(account_list);
		}

		else if (commInput.compare("new account") == 0) {
			newCommand(account_list, totalAccs);
		}

		else if (commInput.compare("close account") == 0) {
			closeCommand(account_list);
		}

		else if (commInput.compare("quit") == 0) {
			logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
				"Admin quit the program" << endl;
  			break;	
		}

		// When user enters none of the above commands
		else {
			cout << endl << "Invalid command: " << commInput << endl << endl;
			logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | "
				<< "Admin input invalid command: " << commInput << endl;
		}
	}

  {
    // Write the new account list back to disk.
    fstream output("account_list", ios::out | ios::trunc | ios::binary);
    if (!account_list.SerializeToOstream(&output)) {
      cerr << "Failed to write account list." << endl;
      return -1;
    }
  }

	// Optional:  Delete all global objects allocated by libprotobuf.
	google::protobuf::ShutdownProtobufLibrary();

	logfile.close();

	return 0;

}
