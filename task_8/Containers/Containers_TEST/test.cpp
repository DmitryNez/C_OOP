#include "pch.h"
#include "../Containers/reference.h"
#include "../Containers/reference.cpp"
#include "../Containers/RNA.h"
#include "../Containers/RNA.cpp"
#include "../Containers/RNA.h"

TEST(TEST_ALLOC, TRUE_ALLOC_TEST) {
	RNA rna;
	ASSERT_EQ(rna.length(),0);
	rna.push_back(A);
	ASSERT_EQ(rna.length(), 1);
	ASSERT_EQ(rna.get_capacity(), 16);
	ASSERT_EQ(rna.get_real_size(), ceil(1.0 * rna.length()/rna.Size_tPerNucl) * sizeof(size_t));
	RNA rna1(A, 120);
	ASSERT_EQ(rna1.get_real_size(), ceil(1.0 * rna1.length() / rna1.Size_tPerNucl) * sizeof(size_t));
}

TEST(TEST_OPERATORS, TEST_SQUARE_BRACKETS) {
	RNA rna(U,16);
	const RNA rna1(G, 17);

	ASSERT_THROW(rna[16], std::out_of_range);
	ASSERT_NO_THROW(rna[0]);

	ASSERT_TRUE(rna[0] == U);
	ASSERT_TRUE(rna1[16] == G);

	rna[15] = A;
	ASSERT_TRUE(rna[15] == A);

	rna[14] = rna1[0];
	ASSERT_TRUE(rna[14] == G);
}

TEST(TEST_OPERATORS, TEST_EQUAL_OPERATOR) {
	RNA rna;
	const RNA rna1(C, 17);

	rna = rna1;
	ASSERT_EQ(rna.length(), rna1.length());
	ASSERT_EQ(rna[0], rna1[0]);
	ASSERT_EQ(rna[16], rna1[16]);
}

TEST(TEST_OPERATORS, TEST_PLUS_OPERATOR) {
	const RNA rna(G, 17);
	const RNA rna1(C, 17);
	
	const RNA rnaSUM(rna + rna1);
	ASSERT_TRUE(rnaSUM[0] == G);
	ASSERT_TRUE(rnaSUM[33] == C);

	const RNA rnaSUMMA(rnaSUM + rna1);
	ASSERT_EQ(rnaSUMMA.length(),0);
}

TEST(TEST_OPERATORS, TEST_IS_EQUAL_OPERATOR) {
	const RNA rna(G, 16);
	const RNA rna1(G, 16);
	const RNA rna2(C, 16);
	const RNA rna3(C, 14);

	ASSERT_TRUE(rna == rna1);
	ASSERT_FALSE(rna2 == rna3);
}

TEST(TEST_OPERATORS, TEST_IS_NOT_EQUAL_OPERATOR) {
	const RNA rna(G, 16);
	const RNA rna1(G, 16);
	const RNA rna2(C, 16);
	const RNA rna3(C, 14);

	ASSERT_FALSE(rna != rna1);
	ASSERT_TRUE(rna2 != rna3);
}

TEST(TEST_OPERATORS, TEST_NEGATION_OPERATOR) {
	RNA rna(G, 16);
	RNA rna1(C, 16);

	ASSERT_TRUE(rna != rna1);
	!rna;
	ASSERT_TRUE(rna == rna1);
}

TEST(TEST_METHODS, TEST_PUSH_BACK) {
	RNA rna;
	ASSERT_TRUE(rna.get_capacity() >= rna.length());
	for (int i = 0; i < 16; i++) {
		rna.push_back(U);
	}
	ASSERT_TRUE(rna.get_capacity() == rna.length());
	rna.push_back(U);
	ASSERT_TRUE(rna.get_capacity() >= rna.length());
	for (int i = 0; i < 1000000; i++) {
		rna.push_back(U);
	}
	ASSERT_EQ(rna.length(), 1000017);
	ASSERT_TRUE(rna.get_capacity() >= rna.length());
}

TEST(TEST_METHODS, TEST_IS_COMPLEMENTARY) {
	RNA rna(G, 16);
	RNA rna1(C, 16);
	RNA rna2(A, 16);

	ASSERT_TRUE(rna.isComplementary(rna1));
	ASSERT_FALSE(rna1.isComplementary(rna2));
}

TEST(TEST_METHODS, TEST_SPLIT) {
	RNA rna(U, 16);
	std::pair<RNA, RNA> two_rna = rna.split(7);
	RNA r(U, 8);
	ASSERT_TRUE(two_rna.first == r);
	ASSERT_TRUE(two_rna.second == r);

	RNA rna1(U, 16);
	ASSERT_ANY_THROW(rna1.split(16));
}

TEST(TEST_METHODS, TEST_CARDINALITY) {
	RNA rna(U, 16);
	ASSERT_EQ(rna.length(), rna.cardinality(U));
	for (int i = 0; i < 10; i++) {
		rna.push_back(C);
	}
	ASSERT_EQ(10, rna.cardinality(C));
	ASSERT_EQ(16, rna.cardinality(U));

	for (int i = 0; i < 20; i++) {
		rna.push_back(A);
	}

	std::unordered_map<Nucleotide, int, std::hash<int>> m = rna.cardinality();

	ASSERT_EQ(20, m[A]);
	ASSERT_EQ(10, m[C]);
	ASSERT_EQ(16, m[U]);
}

TEST(TEST_METHODS, TEST_TRIM) {
	RNA rna(A,10);
	for (int i = 0; i < 1000; i++) {
		rna.push_back(U);
	}
	rna.trim(0);
	EXPECT_EQ(rna.get_capacity(), 0);
}