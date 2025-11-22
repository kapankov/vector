#include <gtest/gtest.h>
//#include <gmock/gmock.h>
#include "vector.h"

TEST(Vector, SizeOf)
{
	diy::vector<int> vec;
	ASSERT_EQ(sizeof(vec), 3 * sizeof(void*));
}

TEST(Vector, Construct)
{
	// Test default constructor
	diy::vector<int> vec1;
	ASSERT_EQ(vec1.size(), 0);
	ASSERT_EQ(vec1.capacity(), 0);

	// Test constructor with count
	diy::vector<int> vec2(5);
	ASSERT_EQ(vec2.size(), 5);
	ASSERT_EQ(vec2.capacity(), 5);

	// Test constructor with count and value
	diy::vector<int> vec3(3, 42);
	ASSERT_EQ(vec3.size(), 3);
	ASSERT_EQ(vec3.capacity(), 3);
	ASSERT_EQ(vec3[0], 42);
	ASSERT_EQ(vec3[1], 42);
	ASSERT_EQ(vec3[2], 42);

	// Test constructor with iterators (range)
	std::vector<int> std_vec{1, 2, 3, 4, 5};
	diy::vector<int> vec4(std_vec.begin(), std_vec.end());
	ASSERT_EQ(vec4.size(), 5);
	// Do not check capacity as it may be larger due to allocation strategy
	ASSERT_EQ(vec4[0], 1);
	ASSERT_EQ(vec4[1], 2);
	ASSERT_EQ(vec4[2], 3);
	ASSERT_EQ(vec4[3], 4);
	ASSERT_EQ(vec4[4], 5);

	// Test copy constructor
	diy::vector<int> vec5(3, 10);
	diy::vector<int> vec6(vec5);
	ASSERT_EQ(vec6.size(), 3);
	ASSERT_EQ(vec6.capacity(), 3);
	ASSERT_EQ(vec6[0], 10);
	ASSERT_EQ(vec6[1], 10);
	ASSERT_EQ(vec6[2], 10);
}

TEST(Vector, Assign)
{
	diy::vector<int> vec(5, 10);  // {10, 10, 10, 10, 10}
	ASSERT_EQ(vec.size(), 5);
	ASSERT_EQ(vec[0], 10);
	ASSERT_EQ(vec[1], 10);
	ASSERT_EQ(vec[2], 10);
	ASSERT_EQ(vec[3], 10);
	ASSERT_EQ(vec[4], 10);
	
	vec.assign(3, 20);  // {20, 20, 20}
	ASSERT_EQ(vec.size(), 3);
	ASSERT_EQ(vec[0], 20);
	ASSERT_EQ(vec[1], 20);
	ASSERT_EQ(vec[2], 20);
	
	vec.assign(0, 30);  // {}
	ASSERT_EQ(vec.size(), 0);
	
	diy::vector<int> vec2(10, 5);  // {5, 5, 5, 5, 5, 5, 5, 5, 5, 5}
	ASSERT_EQ(vec2.size(), 10);
	vec2.assign(15, 15);  // {15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15}
	ASSERT_EQ(vec2.size(), 15);
	ASSERT_EQ(vec2[0], 15);
	ASSERT_EQ(vec2[14], 15);
}

TEST(Vector, Copy)
{
	diy::vector<int> vec_empty;
	diy::vector<int> vec_copy(vec_empty);
	ASSERT_EQ(vec_copy.size(), 0);
	ASSERT_EQ(vec_copy.capacity(), 0);

	diy::vector<int> vec1(10);
	diy::vector<int> vec2(vec1);
	ASSERT_EQ(vec1.size(), vec2.size());
	ASSERT_EQ(vec1.capacity(), vec2.capacity());

	diy::vector<int> vec3;
	vec3 = vec1;
	ASSERT_EQ(vec1.size(), vec3.size());
	ASSERT_EQ(vec1.capacity(), vec3.capacity());
}

TEST(Vector, Move)
{
	diy::vector<int> vec1(3);
	diy::vector<int> vec2(std::move(vec1));
	ASSERT_EQ(vec1.size(), 0);
	ASSERT_EQ(vec2.size(), 3);
}

TEST(Vector, At)
{
	diy::vector<int> vec(10);
	vec.at(0) = 42;
	EXPECT_THROW(vec.at(11), std::out_of_range);
	ASSERT_EQ(vec.at(0), 42);
}

TEST(Vector, Subscript)
{
	diy::vector<int> vec(3);
	for (size_t i = 0; i < 3; ++i)
		vec[i] = i + 1;
	for (size_t i = 0; i < 3; ++i)
		ASSERT_EQ(vec.at(i), i + 1);
}

TEST(Vector, Front)
{
	diy::vector<int> vec(3, 42);
	ASSERT_EQ(vec.front(), 42);
}

TEST(Vector, Back)
{
	diy::vector<int> vec(3, 42);
	ASSERT_EQ(vec.back(), 42);
}

TEST(Vector, Data)
{
	diy::vector<int> vec_empty;
	ASSERT_EQ(vec_empty.data(), nullptr);

	diy::vector<int> vec(3, 42);
	ASSERT_NE(vec.data(), nullptr);
}

TEST(Vector, Iterators)
{
	diy::vector<int> vec(3);
	ASSERT_EQ(&*vec.begin(), vec.data());
	ASSERT_EQ(&*vec.end(), vec.data() + vec.size());

	diy::vector<int> vec1(3, 42);
	for (auto it = vec1.begin(); it != vec1.end(); ++it)
		ASSERT_EQ(*it, 42);

	std::for_each(vec1.begin(), vec1.end(), [](int &elem) { elem *= 2; });
	for (auto& it: vec1)
		ASSERT_EQ(it, 84);
}

TEST(Vector, ReverseIterators)
{
	diy::vector<int> vec(3);
	ASSERT_EQ(&*vec.rbegin(), vec.data() + vec.size() - 1);
	ASSERT_EQ(&*vec.rend(), vec.data() - 1);

	diy::vector<int> vec1(3, 42);
	for (auto rit = vec1.rbegin(); rit != vec1.rend(); ++rit)
		ASSERT_EQ(*rit, 42);
	
	std::for_each(vec1.rbegin(), vec1.rend(), [](int &elem) { elem *= 2; });
	for (auto& rit: vec1)
		ASSERT_EQ(rit, 84);
}

TEST(Vector, Empty)
{
	diy::vector<int> vec;
	ASSERT_TRUE(vec.empty());

	vec.emplace_back(42);
	ASSERT_FALSE(vec.empty());
}

TEST(Vector, Size)
{
	diy::vector<int> vec1;
	ASSERT_EQ(vec1.size(), 0);

	diy::vector<int> vec2(42);
	ASSERT_EQ(vec2.size(), 42);
}

TEST(Vector, MaxSize)
{
	diy::vector<int> vec;
	ASSERT_EQ(vec.max_size(), std::allocator<int>().max_size());
}

TEST(Vector, Reserve)
{
	diy::vector<int> vec;
	vec.reserve(10);
	ASSERT_EQ(vec.capacity(), 10);
}

TEST(Vector, Capacity)
{
	diy::vector<int> vec;
   ASSERT_EQ(vec.capacity(), 0);

   vec.reserve(10);
   ASSERT_EQ(vec.capacity(), 10);
   
   vec.shrink_to_fit();
   ASSERT_EQ(vec.capacity(), 0);

   vec.resize(10);
   ASSERT_EQ(vec.capacity(), 10);
}

TEST(Vector, ShrinkToFit)
{
	diy::vector<int> vec;
	vec.push_back(1);
	vec.push_back(2);
	vec.push_back(3);
	vec.push_back(4);
	vec.push_back(5);
	ASSERT_EQ(vec.capacity(), 8);

	vec.shrink_to_fit();
	ASSERT_EQ(vec.capacity(), 5);

	vec.resize(10);
	vec.shrink_to_fit();
	ASSERT_EQ(vec.capacity(), 10);
}

TEST(Vector, Clear)
{
	diy::vector<int> vec(10);
	ASSERT_EQ(vec.size(), 10);

	vec.clear();
	ASSERT_TRUE(vec.empty());
}

TEST(Vector, Insert)
{
	diy::vector<int> vec(3);
	vec.insert(vec.begin(), 42);

	ASSERT_EQ(vec.size(), 4);
	ASSERT_EQ(vec.capacity(), 6);
	ASSERT_EQ(vec[0], 42);
	ASSERT_EQ(vec[1], 0);
	ASSERT_EQ(vec[2], 0);
	ASSERT_EQ(vec[3], 0);
}

TEST(Vector, InsertValue)
{
	diy::vector<int> vec(3, 10);  // {10, 10, 10}
	vec.insert(vec.begin(), 5);
	ASSERT_EQ(vec.size(), 4);
	ASSERT_EQ(vec[0], 5);
	ASSERT_EQ(vec[1], 10);
	ASSERT_EQ(vec[2], 10);
	ASSERT_EQ(vec[3], 10);
}

TEST(Vector, InsertCount)
{
	diy::vector<int> vec(2, 10);  // {10, 10}
	vec.insert(vec.begin() + 1, 3, 5);
	ASSERT_EQ(vec.size(), 5);
	ASSERT_EQ(vec[0], 10);
	ASSERT_EQ(vec[1], 5);
	ASSERT_EQ(vec[2], 5);
	ASSERT_EQ(vec[3], 5);
	ASSERT_EQ(vec[4], 10);

	diy::vector<int> vec2(2, 10);  // {10, 10}
	vec2.reserve(6);
	vec2.insert(vec2.begin(), 3, 5);
	ASSERT_EQ(vec2.size(), 5);
	ASSERT_EQ(vec2[0], 5);
	ASSERT_EQ(vec2[1], 5);
	ASSERT_EQ(vec2[2], 5);
	ASSERT_EQ(vec2[3], 10);
	ASSERT_EQ(vec2[4], 10);
}

TEST(Vector, InsertIteratorRange)
{
	diy::vector<int> vec1(3, 10);  // {10, 10, 10}
	diy::vector<int> vec2(2, 5);   // {5, 5}
	vec1.insert(vec1.begin() + 1, vec2.begin(), vec2.end());
	ASSERT_EQ(vec1.size(), 5);
	ASSERT_EQ(vec1[0], 10);
	ASSERT_EQ(vec1[1], 5);
	ASSERT_EQ(vec1[2], 5);
	ASSERT_EQ(vec1[3], 10);
	ASSERT_EQ(vec1[4], 10);
}

TEST(Vector, InsertRvalue)
{
	diy::vector<int> vec(2, 10);  // {10, 10}
	vec.insert(vec.begin(), 20);
	ASSERT_EQ(vec.size(), 3);
	ASSERT_EQ(vec[0], 20);
	ASSERT_EQ(vec[1], 10);
	ASSERT_EQ(vec[2], 10);
}

TEST(Vector, InsertInitializerList)
{
	diy::vector<int> vec(2, 10);  // {10, 10}
	vec.insert(vec.end(), {5, 15, 25});
	ASSERT_EQ(vec.size(), 5);
	ASSERT_EQ(vec[0], 10);
	ASSERT_EQ(vec[1], 10);
	ASSERT_EQ(vec[2], 5);
	ASSERT_EQ(vec[3], 15);
	ASSERT_EQ(vec[4], 25);
}

TEST(Vector, Emplace)
{
	diy::vector<int> vec;
	vec.emplace_back(42);
	ASSERT_EQ(vec.size(), 1);
	ASSERT_EQ(vec[0], 42);
}

TEST(Vector, Erase)
{
	diy::vector<int> vec(5, 42);
	vec.erase(vec.begin());
	ASSERT_EQ(vec.size(), 4);
	ASSERT_EQ(vec[0], 42);
	ASSERT_EQ(vec[1], 42);
	ASSERT_EQ(vec[2], 42);
	ASSERT_EQ(vec[3], 42);
}

TEST(Vector, PushBack)
{
	diy::vector<int> vec;
	vec.push_back(42);
	ASSERT_EQ(vec.size(), 1);

	ASSERT_EQ(vec.back(), 42); 
}

TEST(Vector, EmplaceBack)
{
	diy::vector<int> vec;
	vec.emplace_back(42);
	ASSERT_EQ(vec.size(), 1);

	ASSERT_EQ(vec.back(), 42); 
}

TEST(Vector, PopBack)
{
	diy::vector<int> vec(10);
	vec.pop_back();
	ASSERT_EQ(vec.size(), 9);
}

TEST(Vector, Resize)
{
	diy::vector<int> vec;
	vec.resize(10);
	ASSERT_EQ(vec.size(), 10);
	
	vec.resize(5);
	ASSERT_EQ(vec.size(), 5);
	
	vec.resize(15, 42);
	ASSERT_EQ(vec.size(), 15);
	ASSERT_EQ(vec[14], 42);
}

TEST(Vector, Swap)
{
    diy::vector<int> vec1(3, 10);  // {10, 10, 10}
    diy::vector<int> vec2(2, 20);  // {20, 20}
    
    ASSERT_EQ(vec1.size(), 3);
    ASSERT_EQ(vec2.size(), 2);
    ASSERT_EQ(vec1[0], 10);
    ASSERT_EQ(vec1[1], 10);
    ASSERT_EQ(vec1[2], 10);
    ASSERT_EQ(vec2[0], 20);
    ASSERT_EQ(vec2[1], 20);
    
    vec1.swap(vec2);
    
    ASSERT_EQ(vec1.size(), 2);
    ASSERT_EQ(vec2.size(), 3);
    ASSERT_EQ(vec1[0], 20);
    ASSERT_EQ(vec1[1], 20);
    ASSERT_EQ(vec2[0], 10);
    ASSERT_EQ(vec2[1], 10);
    ASSERT_EQ(vec2[2], 10);
}

TEST(Vector, Compare)
{
    diy::vector<int> vec1(3, 10);  // {10, 10, 10}
    diy::vector<int> vec2(3, 10);  // {10, 10, 10}
    diy::vector<int> vec3(2, 20);  // {20, 20}
    diy::vector<int> vec4(4, 5);  // {5, 5, 5, 5}
    
    // Test operator ==
    ASSERT_TRUE(vec1 == vec2);  // equal vectors
    ASSERT_FALSE(vec1 == vec3);    // unequal vectors
    
    // Test operator !=
    ASSERT_FALSE(vec1 != vec2);  // equal vectors
    ASSERT_TRUE(vec1 != vec3);   // unequal vectors
    
    // Test operator <
    ASSERT_FALSE(vec1 < vec2);  // equal vectors
    ASSERT_TRUE(vec4 < vec1);   // vec4 less than vec1 (5 < 10)
    
    // Test operator >
    ASSERT_FALSE(vec1 > vec2);  // equal vectors
    ASSERT_TRUE(vec3 > vec1);  // vec3 greater than vec1 (20 > 10)
    ASSERT_FALSE(vec1 > vec3);  // vec1 not greater than vec3 (10 < 20)
    
    diy::vector<int> vec5(2, 5);  // {5, 5}
    diy::vector<int> vec6(3, 10);  // {10, 10, 10}
    
    // vec5 < vec6? Yes, because 5 < 10 (first element)
    ASSERT_TRUE(vec5 < vec6);
    
    // vec6 > vec5? Yes, because 10 > 5
    ASSERT_TRUE(vec6 > vec5);
    
    // vec6 <= vec5? No, because 10 > 5
    ASSERT_FALSE(vec6 <= vec5);
    
    // vec5 <= vec6? Yes, because 5 <= 10
    ASSERT_TRUE(vec5 <= vec6);
    
    // vec6 >= vec5? Yes, because 10 >= 5
    ASSERT_TRUE(vec6 >= vec5);
    
    // vec5 >= vec6? No, because 5 < 10
    ASSERT_FALSE(vec5 >= vec6);
}
