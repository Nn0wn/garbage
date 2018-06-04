#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "kmp.h"

#include "gtest/gtest.h"

using namespace std;

typedef struct PrefixTestType {
	const char* input;
	vector<int> output;
}PrefixTestType;

typedef struct KMPTestType {
	const char* image;
	const char* input_str;
	vector<int> vals;
	unsigned int multiplier;
}KMPTestType;

PrefixTestType PrefixTest1 = { "aabaaab", vector<int> {0, 1, 0, 1, 2, 2, 3} };
PrefixTestType PrefixTest2 = { "abcabcd",
vector<int> {0, 0, 0, 1, 2, 3, 0} };
PrefixTestType PrefixTest3 = { "sdasdsadsssdsadas",
vector<int> {0, 0, 0, 1, 2, 1, 0, 0, 1, 1, 1, 2, 1, 0, 0, 0, 1} };
PrefixTestType PrefixTest4 = { "afder", vector<int> {0, 0, 0, 0, 0} };

KMPTestType KMPTest1 = { "ab", "abab", vector<int> {0, 2}, 1 };
KMPTestType KMPTest2 = { "aa", "aaaaa", vector<int> {0, 1, 2, 3}, 1 };

KMPTestType KMPShiftTest = { "abcdef", "defabc", vector<int> {3}, 2 };
KMPTestType KMPShiftTest2 = { "aaaaab", "baaaaa", vector<int> {1}, 2 };


class PrefixTest : public ::testing::TestWithParam<PrefixTestType> {
public:
	virtual void SetUp() { }
	virtual void TearDown() { }

};

class KMPTest : public ::testing::TestWithParam<KMPTestType> {
public:
	virtual void SetUp() { }
	virtual void TearDown() { }
};

class NaiveTest : public ::testing::TestWithParam<KMPTestType> {
public:
	virtual void SetUp() { }
	virtual void TearDown() { }
};

INSTANTIATE_TEST_CASE_P(PrefixTestInstantiation, PrefixTest, ::testing::Values(PrefixTest1,
	PrefixTest2, PrefixTest3, PrefixTest4));

INSTANTIATE_TEST_CASE_P(KMPTestInstantiation, KMPTest, ::testing::Values(KMPTest1, KMPTest2));

INSTANTIATE_TEST_CASE_P(NaiveTestInstantiation, NaiveTest, ::testing::Values(KMPTest1, KMPTest2));

INSTANTIATE_TEST_CASE_P(KMPShiftTestInstantiation, KMPTest, ::testing::Values(KMPShiftTest, KMPShiftTest2));

TEST_P(PrefixTest, PrefixTestTrue)
{
	ASSERT_TRUE(prefix_vec(GetParam().input) == GetParam().output);
}

TEST_P(KMPTest, KMPTestTrue) {
	ASSERT_TRUE(kmp(GetParam().image, GetParam().input_str, GetParam().multiplier) == GetParam().vals);
}

TEST_P(NaiveTest, NaiveTestTrue) {
	ASSERT_TRUE(naive(GetParam().image, GetParam().input_str) == GetParam().vals);

}


int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
