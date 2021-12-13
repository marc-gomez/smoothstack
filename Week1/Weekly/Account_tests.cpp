#include "Account.h"
#include "unit_test_framework.h"

using namespace std;

TEST(test_getID) {
    Account acc("Jane Doe", 1, 123450000);
    ASSERT_EQUAL(acc.getID(), 1);
}

TEST(test_getName) {
    Account acc("Jane Doe", 1, 123450000);
    ASSERT_EQUAL(acc.getName(), "Jane Doe");
}

TEST(test_getSSN) {
    Account acc("Jane Doe", 1, 123450000);
    ASSERT_EQUAL(acc.getSSN(), 123450000);
}

TEST_MAIN()
