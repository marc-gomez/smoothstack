#include <string>
#include <google/protobuf/util/time_util.h>
#include "bankaccounts.pb.h"
#include "bankcli.h"
#include "unit_test_framework.h"
#include <sstream>

using namespace std;

TEST(test_account_size) {
	bank::AccountList account_list;
	ASSERT_EQUAL(account_list.accounts_size(), 0);
}

TEST(test_add_account) {
	ostringstream logfile;
	int count = 0;

	bank::AccountList account_list;
	ASSERT_EQUAL(account_list.accounts_size(), 0);
	addAccount(account_list.add_accounts(), 123450000, "Jane Doe", count, logfile);
    ASSERT_EQUAL(account_list.accounts_size(), 1);
    ASSERT_EQUAL(account_list.accounts(0).id(), 0);
    ASSERT_EQUAL(account_list.accounts(0).name(), "Jane Doe");
    ASSERT_EQUAL(account_list.accounts(0).ssn(), 123450000);
}

TEST(test_authorize_false) {
	ostringstream logfile;
	ostringstream os;
	istringstream is("demo\npassword");

	bank::AdminList admin_list;
	{
		// Read the existing admin list.
		fstream input("admin_list", ios::in | ios::binary);
		if (!admin_list.ParseFromIstream(&input)) {
			ASSERT_TRUE(false);
		}
	}

	ASSERT_FALSE(authorize(admin_list, logfile, os, is));
}

TEST(test_authorize_true) {
	ostringstream logfile;
	ostringstream os;
	istringstream is("demo\ndemo");

	bank::AdminList admin_list;
	{
		// Read the existing admin list.
		fstream input("admin_list", ios::in | ios::binary);
		if (!admin_list.ParseFromIstream(&input)) {
			ASSERT_TRUE(false);
		}
	}

	ASSERT_TRUE(authorize(admin_list, logfile, os, is));
}

TEST(test_string_similarity_same) {
	ASSERT_EQUAL(stringSimilarity("hello", "hello"), 0);
}

TEST(test_string_similarity_diff_1) {
	ASSERT_EQUAL(stringSimilarity("hello", "hello."), 1);
}

TEST(test_string_similarity_diff_2) {
	ASSERT_EQUAL(stringSimilarity("hello", "ello"), 1);
}

TEST(test_show_empty) {
	ostringstream logfile;
	ostringstream os;

	bank::AccountList account_list;
	showCommand(account_list, logfile, os);

	ASSERT_EQUAL(os.str(), "\n");
}

TEST(test_show_1) {
	ostringstream logfile;
	ostringstream os;
	int count = 0;
	auto ts = google::protobuf::util::TimeUtil::SecondsToTimestamp(time(NULL));

	bank::AccountList account_list;
	addAccount(account_list.add_accounts(), 123450000, "Jane Doe", count, logfile);
	showCommand(account_list, logfile, os);

	string correct = (string("\n") + 
						string("Customer name: Jane Doe\n") +
						string("Account: 0\n") +
						string("Date Opened: ") +
						google::protobuf::util::TimeUtil::ToString(ts) + 
						string("\n\n"));

	ASSERT_EQUAL(os.str(), correct);
}

TEST(test_display) {
	ostringstream logfile;
	ostringstream os;
	int count = 0;
	auto ts = google::protobuf::util::TimeUtil::SecondsToTimestamp(time(NULL));

	bank::AccountList account_list;
	addAccount(account_list.add_accounts(), 123450000, "Jane Doe", count, logfile);
	displayAccount(&account_list.accounts(0), logfile, os);

	string correct = (string("Customer name: Jane Doe\n") +
						string("Balance: $0.00\n") +
						string("SSN: XXX-XX-0000\n") +
						string("Date Opened: ") +
						google::protobuf::util::TimeUtil::ToString(ts) + 
						string("\n\n"));

	ASSERT_EQUAL(os.str(), correct);
}

TEST(test_refresh) {
	ostringstream logfile;
	ostringstream os;
	bank::AccountList account_list;
	int count = 0;
	auto ts = google::protobuf::util::TimeUtil::SecondsToTimestamp(time(NULL));

	addAccount(account_list.add_accounts(), 123450000, "Jane Doe", count, logfile);
	ASSERT_EQUAL(account_list.accounts_size(), 1);
	int id = account_list.accounts(0).id();

	account_list.mutable_accounts()->DeleteSubrange(0, 1);
	ASSERT_EQUAL(account_list.accounts_size(), 0);

	refreshAccount(account_list.add_accounts(), 123451111, "John Smith", id, 0.0, ts);
	ASSERT_EQUAL(account_list.accounts_size(), 1);

	displayAccount(&account_list.accounts(0), logfile, os);

	string correct = (string("Customer name: John Smith\n") +
						string("Balance: $0.00\n") +
						string("SSN: XXX-XX-1111\n") +
						string("Date Opened: ") +
						google::protobuf::util::TimeUtil::ToString(ts) + 
						string("\n\n"));

	ASSERT_EQUAL(os.str(), correct);
}

TEST(test_display_command_empty) {
	ostringstream logfile;
	ostringstream os;
	istringstream is;
	bank::AccountList account_list;

	displayCommand(account_list, logfile, os, is);

	string correct = string("No accounts in the system\n");

	ASSERT_EQUAL(os.str(), correct);
}

TEST(test_display_command_invalid) {
	ostringstream logfile;
	ostringstream os;
	istringstream is("1");
	int count = 0;
	bank::AccountList account_list;

	addAccount(account_list.add_accounts(), 123450000, "Jane Doe", count, logfile);

	displayCommand(account_list, logfile, os, is);

	string correct = string("account > \nInvalid account\n\n");

	ASSERT_EQUAL(os.str(), correct);
}

TEST(test_display_command_credit) {
	ostringstream logfile;
	ostringstream os;
	istringstream is("0\n1\ncredit\n50");
	int count = 0;
	bank::AccountList account_list;
	auto ts = google::protobuf::util::TimeUtil::SecondsToTimestamp(time(NULL));

	addAccount(account_list.add_accounts(), 123450000, "Jane Doe", count, logfile);

	displayCommand(account_list, logfile, os, is);

	string correct = (string("account > \n") +
						string("Customer name: Jane Doe\n") +
						string("Balance: $0.00\n") +
						string("SSN: XXX-XX-0000\n") +
						string("Date Opened: ") +
						google::protobuf::util::TimeUtil::ToString(ts) + 
						string("\n\n") +
						string("1. Add Transaction\n") +
						string("2. Home\n\n") +
						string("Select number > \nDebit or credit > Amount > Transaction complete\n\n"));

	ASSERT_EQUAL(os.str(), correct);
}

TEST(test_search_command) {
	ostringstream logfile;
	ostringstream os;
	istringstream is("Jan Do\n1");
	int count = 0;
	bank::AccountList account_list;
	auto ts = google::protobuf::util::TimeUtil::SecondsToTimestamp(time(NULL));

	addAccount(account_list.add_accounts(), 123450000, "Jane Doe", count, logfile);
	++count;
	addAccount(account_list.add_accounts(), 123451111, "Bob Tanner", count, logfile);

	searchCommand(account_list, logfile, os, is);

	string correct = (string("name > \n") +
						string("1. Jane Doe\n") +
						string("2. Home\n\n") +
						string("Select number > \n") +
						string("Customer name: Jane Doe\n") +
						string("Balance: $0.00\n") +
						string("SSN: XXX-XX-0000\n") +
						string("Date Opened: ") +
						google::protobuf::util::TimeUtil::ToString(ts) + 
						string("\n\n"));

	ASSERT_EQUAL(os.str(), correct);
}

TEST(test_new_command) {
	ostringstream logfile;
	ostringstream os;
	istringstream is("Jane Doe\n123450000");
	int count = 0;
	bank::AccountList account_list;

	newCommand(account_list, count, logfile, os, is);

	ASSERT_EQUAL(count, 1);
	ASSERT_EQUAL(account_list.accounts_size(), 1);

	string correct = (string("Customer Name > SSN > \nAccount created\n\n"));

	ASSERT_EQUAL(os.str(), correct);
}

TEST(test_close_command) {
	ostringstream logfile;
	ostringstream os;
	istringstream is("0\nyes");
	int count = 0;
	bank::AccountList account_list;

	addAccount(account_list.add_accounts(), 123450000, "Jane Doe", count, logfile);
	ASSERT_EQUAL(account_list.accounts_size(), 1);

	closeCommand(account_list, logfile, os, is);
	ASSERT_EQUAL(account_list.accounts_size(), 0);

	string correct = (string("account > \n") +
						string("Are you sure you want to delete this account? (yes/no)\n") +
						string("\nAccount deleted\n\n"));

	ASSERT_EQUAL(os.str(), correct);
}

TEST_MAIN()
