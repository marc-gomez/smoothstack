#include <string>
#include "BigNumber.h"
#include "unit_test_framework.h"

using namespace std;

TEST(test_basic_multiply) {
	BigNumber A("2");
	BigNumber B("3");
	BigNumber C;
	A.multiply(B, C);
	ASSERT_EQUAL(C.number(), to_string(2*3));
}

TEST(test_basic_simulate_multiply) {
	BigNumber A("2");
	BigNumber B("3");
	BigNumber C;
	A.simulate_multiply(B, C);
	ASSERT_EQUAL(C.number(), to_string(2*3));
}

TEST(test_number) {
	BigNumber A("2");
	BigNumber B("3");
	BigNumber C;
	ASSERT_EQUAL(A.number(), "2");
	ASSERT_EQUAL(B.number(), "3");
	ASSERT_EQUAL(C.number(), "0");
}

TEST(test_simulate_A_0) {
	BigNumber A("0");
	BigNumber B("3");
	BigNumber C;
	A.simulate_multiply(B, C);
	ASSERT_EQUAL(C.number(), "0");
}

TEST(test_simulate_B_0) {
	BigNumber A("2");
	BigNumber B("0");
	BigNumber C;
	A.simulate_multiply(B, C);
	ASSERT_EQUAL(C.number(), "0");
}

TEST(test_set_string) {
	BigNumber A("2");

	A.set("5");
	ASSERT_EQUAL(A.number(), "5");
}

TEST(test_set_long) {
	BigNumber A("2");

	A.set(5);
	ASSERT_EQUAL(A.number(), "5");
}

TEST(test_overflow) {
	BigNumber A("999999999999999999");
	BigNumber B("888888888888888888");
	BigNumber C;
	A.multiply(B, C);
	ASSERT_EQUAL(C.number(), to_string(LONG_MIN));
}

TEST(test_multiply) {
	BigNumber A("124");
	BigNumber B("456");
	BigNumber C;

	numvec out = A.simulate_multiply(B, C);
	ASSERT_EQUAL(out.size(), 6);

	ASSERT_EQUAL(out[0].size(), 3);
	ASSERT_EQUAL(out[0][0], '1');
	ASSERT_EQUAL(out[0][1], '2');
	ASSERT_EQUAL(out[0][2], '4');

	ASSERT_EQUAL(out[1].size(), 3);
	ASSERT_EQUAL(out[1][0], '4');
	ASSERT_EQUAL(out[1][1], '5');
	ASSERT_EQUAL(out[1][2], '6');

	ASSERT_EQUAL(out[2].size(), 3);
	ASSERT_EQUAL(out[2][0], '7');
	ASSERT_EQUAL(out[2][1], '4');
	ASSERT_EQUAL(out[2][2], '4');

	ASSERT_EQUAL(out[3].size(), 3);
	ASSERT_EQUAL(out[3][0], '6');
	ASSERT_EQUAL(out[3][1], '2');
	ASSERT_EQUAL(out[3][2], '0');

	ASSERT_EQUAL(out[4].size(), 3);
	ASSERT_EQUAL(out[4][0], '4');
	ASSERT_EQUAL(out[4][1], '9');
	ASSERT_EQUAL(out[4][2], '6');

	ASSERT_EQUAL(out[5].size(), 5);
	ASSERT_EQUAL(out[5][0], '5');
	ASSERT_EQUAL(out[5][1], '6');
	ASSERT_EQUAL(out[5][2], '5');
	ASSERT_EQUAL(out[5][3], '4');
	ASSERT_EQUAL(out[5][4], '4');
}

TEST_MAIN()
