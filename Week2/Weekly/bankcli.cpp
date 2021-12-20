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
#include "bankcli.h"

using namespace std;

using google::protobuf::util::TimeUtil;

int main() {

	cout << endl;

	ofstream logfile;

	// Open log file
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
		authorized = authorize(admin_list, logfile, cout, cin);
	}

	// Console prompt
	printCommands(cout);

	while(true) {
		string commInput = "";
		cout << "> ";
		while (commInput.compare("") == 0) {
			getline(cin, commInput);
		}

		if (commInput.compare("show accounts") == 0) {
			showCommand(account_list, logfile, cout);
		}

		else if (commInput.compare("display account") == 0) {
			displayCommand(account_list, logfile, cout, cin);
		}

		else if (commInput.compare("search name") == 0) {
			searchCommand(account_list, logfile, cout, cin);
		}

		else if (commInput.compare("new account") == 0) {
			newCommand(account_list, totalAccs, logfile, cout, cin);
		}

		else if (commInput.compare("close account") == 0) {
			closeCommand(account_list, logfile, cout, cin);
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
