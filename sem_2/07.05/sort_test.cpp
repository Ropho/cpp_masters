////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cstddef>
#include <random>
#include <vector>

#include <gtest/gtest.h>

#include "sort.hpp"

////////////////////////////////////////////////////////////////////////////////

TEST(Sort, EmptyVector)
{
	std::vector < int > vec;

	sort(vec);

	EXPECT_TRUE(vec.empty());
	EXPECT_TRUE(std::ranges::is_sorted(vec));
}

////////////////////////////////////////////////////////////////////////////////

TEST(Sort, SingleElement)
{
	std::vector < int > vec = { 42 };

	sort(vec);

	ASSERT_EQ(vec.size(), 1u);
	EXPECT_EQ(vec[0], 42);
	EXPECT_TRUE(std::ranges::is_sorted(vec));
}

////////////////////////////////////////////////////////////////////////////////

TEST(Sort, TwoElementsSorted)
{
	std::vector < int > vec = { 1, 2 };

	sort(vec);

	EXPECT_EQ(vec[0], 1);
	EXPECT_EQ(vec[1], 2);
	EXPECT_TRUE(std::ranges::is_sorted(vec));
}

////////////////////////////////////////////////////////////////////////////////

TEST(Sort, TwoElementsReverse)
{
	std::vector < int > vec = { 2, 1 };

	sort(vec);

	EXPECT_EQ(vec[0], 1);
	EXPECT_EQ(vec[1], 2);
	EXPECT_TRUE(std::ranges::is_sorted(vec));
}

////////////////////////////////////////////////////////////////////////////////

TEST(Sort, AlreadySorted)
{
	std::vector < int > vec = { 1, 2, 3, 4, 5 };

	sort(vec);

	EXPECT_TRUE(std::ranges::is_sorted(vec));
	EXPECT_EQ(vec, (std::vector < int > { 1, 2, 3, 4, 5 }));
}

////////////////////////////////////////////////////////////////////////////////

TEST(Sort, ReverseOrder)
{
	std::vector < int > vec = { 5, 4, 3, 2, 1 };

	sort(vec);

	EXPECT_TRUE(std::ranges::is_sorted(vec));
	EXPECT_EQ(vec, (std::vector < int > { 1, 2, 3, 4, 5 }));
}

////////////////////////////////////////////////////////////////////////////////

TEST(Sort, AllEqual)
{
	std::vector < int > vec = { 7, 7, 7, 7 };

	sort(vec);

	EXPECT_TRUE(std::ranges::is_sorted(vec));
	EXPECT_EQ(vec, (std::vector < int > { 7, 7, 7, 7 }));
}

////////////////////////////////////////////////////////////////////////////////

TEST(Sort, RandomSmall)
{
	std::vector < int > vec = { 3, 1, 4, 1, 5, 9, 2, 6 };

	sort(vec);

	EXPECT_TRUE(std::ranges::is_sorted(vec));
	EXPECT_EQ(vec, (std::vector < int > { 1, 1, 2, 3, 4, 5, 6, 9 }));
}

////////////////////////////////////////////////////////////////////////////////

TEST(Sort, RandomLarge)
{
	std::mt19937 gen(42);
	std::uniform_int_distribution < int > dist(-1000, 1000);

	std::vector < int > vec(1000);

	for (auto & x : vec)
	{
		x = dist(gen);
	}

	sort(vec);

	EXPECT_TRUE(std::ranges::is_sorted(vec));
}

////////////////////////////////////////////////////////////////////////////////

TEST(Sort, DescendingSequence)
{
	std::vector < int > vec(500);

	for (std::size_t i = 0; i < vec.size(); ++i)
	{
		vec[i] = static_cast < int > (vec.size() - i);
	}

	sort(vec);

	EXPECT_TRUE(std::ranges::is_sorted(vec));
}

////////////////////////////////////////////////////////////////////////////////

TEST(Sort, TemplateDouble)
{
	std::vector < double > vec = { 3.1, 1.4, 2.7, 0.5, 2.0 };

	sort(vec);

	EXPECT_TRUE(std::ranges::is_sorted(vec));
	EXPECT_DOUBLE_EQ(vec[0], 0.5);
	EXPECT_DOUBLE_EQ(vec.back(), 3.1);
}

////////////////////////////////////////////////////////////////////////////////

TEST(Sort, WithThreshold)
{
	std::vector < int > vec = { 5, 4, 3, 2, 1 };

	sort(vec, static_cast < std::size_t > (3));

	EXPECT_TRUE(std::ranges::is_sorted(vec));
	EXPECT_EQ(vec, (std::vector < int > { 1, 2, 3, 4, 5 }));
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char ** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

////////////////////////////////////////////////////////////////////////////////
