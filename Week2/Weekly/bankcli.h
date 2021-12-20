#ifndef BANKCLI_H
#define BANKCLI_H

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
void printCommands(ostream &outstream) {
	outstream << "Console commands:" << endl << endl;
	outstream << "1. \"show accounts\"" << endl;
	outstream << "2. \"display account\"" << endl;
	outstream << "3. \"search name\"" << endl;
	outstream << "4. \"new account\"" << endl;
	outstream << "5. \"close account\"" << endl;
	outstream << "6. \"quit\"" << endl << endl;
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
void showCommand(bank::AccountList &account_list, ostream &logfile, ostream &outstream) {
	outstream << endl;

	for (int i = 0; i < account_list.accounts_size(); i++) {
		const bank::Account& account = account_list.accounts(i);

		outstream << "Customer name: " << account.name() << endl;
		outstream << "Account: " << account.id() << endl;
		outstream << "Date Opened: " << account.date_opened() << endl << endl;

	}

	logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
		"Admin viewed the list of all accounts" << endl;
}

// This function fills in an Account message based on user input.
void displayAccount(const bank::Account* account, ostream &logfile, ostream &outstream) {
	outstream << "Customer name: " << account->name() << endl;
	outstream << "Balance: $" << fixed << setprecision(2) << account->balance() << endl;
	outstream << "SSN: " << "XXX-XX-" << setw(4) << setfill('0') << account->ssn() % 10000 << endl;
	outstream << "Date Opened: " << account->date_opened() << endl << endl;
	logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
		"Admin displayed account info for user with id " << account->id() << endl;
}

// This function fills in an Account message based on an existing Account message.
void refreshAccount(bank::Account* account, int ssn, const string &name,
					int id, double bal, google::protobuf::Timestamp ts) {
	account->set_id(id);
	account->set_ssn(ssn);
	account->set_balance(bal);
	*account->mutable_name() = name;
	*account->mutable_date_opened() = ts;
}

// When user enters "display account"
// Print a specific user's name, ssn, and date opened
// Then allow a transaction for that account
void displayCommand(bank::AccountList &account_list, ostream &logfile,
					ostream &outstream, istream &instream) {
	if (account_list.accounts_size() == 0) {
		outstream << "No accounts in the system" << endl;
		return;
	}

	outstream << "account > ";
	int accountNum;
	instream >> accountNum;
	bool found = false;

	bank::Account account = account_list.accounts(0);
	int index;
	for (index = 0; index < account_list.accounts_size(); index++) {
		account = account_list.accounts(index);

		if (account.id() == accountNum) {
			outstream << endl;
			displayAccount(&account, logfile, outstream);
			found = true;
			break;
		}

	}

	if (!found) {
		outstream << endl << "Invalid account" << endl << endl;
		logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
			"Admin attempted to display account info for user with id " << accountNum << endl;
	}

	else {
		outstream << "1. Add Transaction" << endl;
		outstream << "2. Home" << endl << endl;

		int chosen = -1;
		while (chosen <= 0 || chosen > 2) {
			outstream << "Select number > ";
			instream >> chosen;
			if (chosen <= 0 || chosen > 2) {
				outstream << endl << "Invalid choice" << endl << endl;
			}
		}

		if (chosen == 1) {
			outstream << endl << "Debit or credit > ";
			instream.ignore(256,'\n');
			string dc;
			getline(instream, dc);

			outstream << "Amount > ";
			double amt;
			instream >> amt;

			double bal = account.balance();
			int id = account.id();
			int ssn = account.ssn();
			auto date = account.date_opened();
			string name = account.name();

			if (dc.compare("debit") == 0 || dc.compare("d") == 0 || dc.compare("Debit") == 0) {
				if (amt > bal) {
					outstream << endl << "Insufficient balance" << endl;
					logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
						"Admin attempted to withdraw $" << fixed << setprecision(2) << amt <<
						" from account with id " << account.id() << endl;
				}
				else {
					account_list.mutable_accounts()->DeleteSubrange(index, 1);
					refreshAccount(account_list.add_accounts(), ssn, name, id, bal-amt, date);
					outstream << endl << "Transaction complete" << endl;
					logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
						"Admin withdrew $" << fixed << setprecision(2) <<
						amt << " from account with id " << account.id() << endl;
					}
			}
			else if (dc.compare("credit") == 0 || dc.compare("c") == 0 || dc.compare("Credit") == 0) {
				account_list.mutable_accounts()->DeleteSubrange(index, 1);
				refreshAccount(account_list.add_accounts(), ssn, name, id, bal+amt, date);
				outstream << "Transaction complete" << endl;
				logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
					"Admin added $" << fixed << setprecision(2) << amt <<
					" to account with id " << account.id() << endl;
			}
			else {
				outstream << "Invalid type: " << dc << endl;
			}
		}
		outstream << endl;
	}
}

// When user enters "search name"
// Print a list of similar names to the user input name,
// Then print a chosen user's name, ssn, and date opened
void searchCommand(bank::AccountList &account_list, ostream &logfile,
					ostream &outstream, istream &instream) {
	outstream << "name > ";
	string nameSearch;
	getline(instream, nameSearch);
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

	outstream << endl;
	for(int i = 0; i < matches.size(); ++i) {
		outstream << i+1 << ". " << matches[i].name() << endl;
	}
	outstream << matches.size()+1 << ". Home" << endl << endl;
			
	int chosen = -1;
	while (chosen <= 0 || chosen > matches.size()+1) {
		outstream << "Select number > ";
		instream >> chosen;
		if (chosen <= 0 || chosen > matches.size()+1) {
			outstream << endl << "Invalid choice" << endl << endl;
		}
	}

	outstream << endl;
	if(chosen != matches.size()+1) {
		logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
			"Admin searched for user with id " << matches[chosen-1].id() << endl;
		displayAccount(&matches[chosen-1], logfile, outstream);
	}

}

// This function fills in an Account message based on user input.
void addAccount(bank::Account* account, int ssn, const string &name, int &count, ostream &logfile) {
	account->set_id(count);
	account->set_ssn(ssn);
	account->set_balance(0.0);
	*account->mutable_name() = name;
	*account->mutable_date_opened() = TimeUtil::SecondsToTimestamp(time(NULL));
	logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
		"Admin successfully created user account with ssn " << ssn << endl;
}

// When user enters "new account"
// Create a new account with given information if the SSN doesn't already exist
void newCommand(bank::AccountList &account_list, int &count,
				ostream &logfile, ostream &outstream, istream &instream) {
	outstream << "Customer Name > ";
	string newName;
	getline(instream, newName);

	outstream << "SSN > ";
	int newSSN;
	instream >> newSSN;

	bool exists = false;
	for (int i = 0; i < account_list.accounts_size(); i++) {
		const bank::Account& account = account_list.accounts(i);

		if (account.ssn() == newSSN) {
			exists = true;
			break;
		}

	}

	if (exists) {
		outstream << endl << "Account exists" << endl << endl;
		logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
			"Admin failed to add account with ssn" << newSSN << endl;
	}
	else {
		addAccount(account_list.add_accounts(), newSSN, newName, count, logfile);
		++count;
		outstream << endl << "Account created" << endl << endl;
	}
}

// When user enters "close account"
// Close an account, with confirmation
void closeCommand(bank::AccountList &account_list, ostream &logfile,
					ostream &outstream, istream &instream) {
	outstream << "account > ";
	int toDelete;
	instream >> toDelete;

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
		outstream << endl << "Are you sure you want to delete this account? (yes/no)" << endl;
		string confirmation;
		instream >> confirmation;

		if (confirmation.compare("yes") == 0 || confirmation.compare("y") == 0) {
			account_list.mutable_accounts()->DeleteSubrange(index, 1);
			outstream << endl << "Account deleted" << endl << endl;
			logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
				"Admin deleted user" << toDelete << endl;
		}
	}
	else {
		outstream << endl << "Invalid account" << endl << endl;
		logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
			"Admin attempted to delete invalid user " << toDelete << endl;
	}

}

// Authorize admin login atempt
// Return true if login is successful
bool authorize(const bank::AdminList &admin_list, ostream &logfile,
				ostream &outstream, istream &instream) {
	string username;
	outstream << "Username: ";
	getline(instream, username);

	string password;
	outstream << "Password: ";
	getline(instream, password);

	if(inAdminList(admin_list, username, password)) {
		outstream << endl << "Login successful" << endl << endl;
			logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
				"Successful login, username: " << username << endl;
		return true;
	}

	outstream << endl << "Incorrect user" << endl << endl;
	logfile << TimeUtil::SecondsToTimestamp(time(NULL)) << " | " <<
		"Failed login attempt, username: " << username << endl;
	return false;
}

#endif
