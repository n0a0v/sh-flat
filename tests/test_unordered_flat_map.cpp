/*	BSD 3-Clause License

	Copyright (c) 2024, Paul Varga
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions are met:

	1. Redistributions of source code must retain the above copyright notice, this
	   list of conditions and the following disclaimer.

	2. Redistributions in binary form must reproduce the above copyright notice,
	   this list of conditions and the following disclaimer in the documentation
	   and/or other materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	   contributors may be used to endorse or promote products derived from
	   this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <gtest/gtest.h>

#include <sh/unordered_flat_map.hpp>

#include <climits>
#include <string>
#include <type_traits>

using sh::unordered_flat_map;
using sh::unsorted_unique;

namespace
{
	template <typename T>
	class equal_to_transparent
	{
	public:
		using is_transparent = void;

		explicit equal_to_transparent(std::uint32_t& compared_normally, std::uint32_t& compared_transparently)
			: m_compared_normally{ &compared_normally }
			, m_compared_transparently{ &compared_transparently }
		{ }
		bool operator()(const T& lhs, const T& rhs) const
		{
			++(*m_compared_normally);
			return lhs == rhs;
		}
		template <typename U>
		bool operator()(const T& lhs, const U& rhs) const
		{
			++(*m_compared_transparently);
			return lhs == rhs;
		}
		template <typename U>
		bool operator()(const U& lhs, const T& rhs) const
		{
			++(*m_compared_transparently);
			return lhs == rhs;
		}
	private:
		std::uint32_t* m_compared_normally;
		std::uint32_t* m_compared_transparently;
	};
} // anonymous namespace

TEST(sh_unordered_flat_map, ctor_default)
{
	unordered_flat_map<int, std::string> x;
	EXPECT_TRUE(x.empty());
	EXPECT_EQ(x.size(), 0u);
}
TEST(sh_unordered_flat_map, ctor_copy)
{
	unordered_flat_map<int, std::string> x;
	x.try_emplace(1, "one");
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 1u);
	ASSERT_TRUE(x.contains(1));

	unordered_flat_map<int, std::string> y{ x };
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 1u);
	EXPECT_EQ(y.at(1), "one");
}
TEST(sh_unordered_flat_map, ctor_copy_alloc)
{
	unordered_flat_map<int, std::string> x;
	x.try_emplace(1, "one");
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 1u);
	ASSERT_TRUE(x.contains(1));

	unordered_flat_map<int, std::string> y{ x, decltype(x)::key_container_type::allocator_type{} };
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 1u);
	EXPECT_EQ(y.at(1), "one");
}
TEST(sh_unordered_flat_map, ctor_move)
{
	unordered_flat_map<int, std::string> x;
	x.try_emplace(1, "one");
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 1u);
	ASSERT_TRUE(x.contains(1));

	unordered_flat_map<int, std::string> y{ std::move(x) };
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 1u);
	EXPECT_EQ(y.at(1), "one");
}
TEST(sh_unordered_flat_map, ctor_move_alloc)
{
	unordered_flat_map<int, std::string> x;
	x.try_emplace(1, "one");
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 1u);
	ASSERT_TRUE(x.contains(1));

	unordered_flat_map<int, std::string> y{ std::move(x), decltype(x)::key_container_type::allocator_type{} };
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 1u);
	EXPECT_EQ(y.at(1), "one");
}
TEST(sh_unordered_flat_map, ctor_keys_values)
{
	unordered_flat_map<int, std::string> x{
		std::vector<int>{ 1, 2, 3, 1 },
		std::vector<std::string>{ "one", "two", "three", "ONE" },
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_keys_values_alloc)
{
	unordered_flat_map<int, std::string> x{
		std::vector<int>{ 1, 2, 3, 1 },
		std::vector<std::string>{ "one", "two", "three", "ONE" },
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_keys_values_eq_alloc)
{
	unordered_flat_map<int, std::string> x{
		std::vector<int>{ 1, 2, 3, 1 },
		std::vector<std::string>{ "one", "two", "three", "ONE" },
		std::equal_to<int>{},
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_unsorted_unique_keys_values)
{
	unordered_flat_map<int, std::string> x{
		unsorted_unique,
		std::vector<int>{ 3, 1, 2 },
		std::vector<std::string>{ "three", "one", "two" }
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_unsorted_unique_keys_values_alloc)
{
	unordered_flat_map<int, std::string> x{
		unsorted_unique,
		std::vector<int>{ 3, 1, 2 },
		std::vector<std::string>{ "three", "one", "two" },
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_unsorted_unique_keys_values_eq_alloc)
{
	unordered_flat_map<int, std::string> x{
		unsorted_unique,
		std::vector<int>{ 3, 1, 2 },
		std::vector<std::string>{ "three", "one", "two" },
		std::equal_to<int>{},
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_eq)
{
	unordered_flat_map<int, std::string> x{
		std::equal_to<int>{}
	};
	EXPECT_TRUE(x.empty());
}
TEST(sh_unordered_flat_map, ctor_eq_alloc)
{
	unordered_flat_map<int, std::string> x{
		std::equal_to<int>{},
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_TRUE(x.empty());
}
TEST(sh_unordered_flat_map, ctor_alloc)
{
	unordered_flat_map<int, std::string> x{
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_TRUE(x.empty());
}
TEST(sh_unordered_flat_map, ctor_first_last)
{
	const std::vector<std::pair<int, std::string>> values = {
		{ 2, "two" },
		{ 3, "three" },
		{ 1, "one" },
		{ 1, "ONE" }
	};
	unordered_flat_map<int, std::string> x{ values.begin(), values.end() };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_first_last_eq_alloc)
{
	const std::vector<std::pair<int, std::string>> values = {
		{ 2, "two" },
		{ 3, "three" },
		{ 1, "one" },
		{ 1, "ONE" }
	};
	unordered_flat_map<int, std::string> x{
		values.begin(),
		values.end(),
		std::equal_to<int>{},
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_first_last_alloc)
{
	const std::vector<std::pair<int, std::string>> values = {
		{ 2, "two" },
		{ 3, "three" },
		{ 1, "one" },
		{ 1, "ONE" }
	};
	unordered_flat_map<int, std::string> x{
		values.begin(),
		values.end(),
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_unsorted_unique_first_last)
{
	const std::vector<std::pair<int, std::string>> values = {
		{ 2, "two" },
		{ 3, "three" },
		{ 1, "one" },
	};
	unordered_flat_map<int, std::string> x{
		unsorted_unique,
		values.begin(),
		values.end()
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_unsorted_unique_first_last_eq_alloc)
{
	const std::vector<std::pair<int, std::string>> values = {
		{ 2, "two" },
		{ 3, "three" },
		{ 1, "one" },
	};
	unordered_flat_map<int, std::string> x{
		unsorted_unique,
		values.begin(),
		values.end(),
		std::equal_to<int>{},
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_unsorted_unique_first_last_alloc)
{
	const std::vector<std::pair<int, std::string>> values = {
		{ 2, "two" },
		{ 3, "three" },
		{ 1, "one" },
	};
	unordered_flat_map<int, std::string> x{
		unsorted_unique,
		values.begin(),
		values.end(),
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_initializer_list)
{
	const std::initializer_list<std::pair<int, std::string>> values = {
		{ 2, "two" },
		{ 3, "three" },
		{ 1, "one" },
		{ 1, "ONE" },
	};
	unordered_flat_map<int, std::string> x{
		values
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_initializer_list_eq_alloc)
{
	const std::initializer_list<std::pair<int, std::string>> values = {
		{ 2, "two" },
		{ 3, "three" },
		{ 1, "one" },
		{ 1, "ONE" },
	};
	unordered_flat_map<int, std::string> x{
		values,
		std::equal_to<int>{},
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_initializer_list_alloc)
{
	const std::initializer_list<std::pair<int, std::string>> values = {
		{ 2, "two" },
		{ 3, "three" },
		{ 1, "one" },
		{ 1, "ONE" },
	};
	unordered_flat_map<int, std::string> x{
		values,
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_unsorted_unique_initializer_list)
{
	const std::initializer_list<std::pair<int, std::string>> values = {
		{ 2, "two" },
		{ 3, "three" },
		{ 1, "one" },
	};
	unordered_flat_map<int, std::string> x{
		unsorted_unique,
		values
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_unsorted_unique_initializer_list_eq_alloc)
{
	const std::initializer_list<std::pair<int, std::string>> values = {
		{ 2, "two" },
		{ 3, "three" },
		{ 1, "one" },
	};
	unordered_flat_map<int, std::string> x{
		unsorted_unique,
		values,
		std::equal_to<int>{},
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, ctor_unsorted_unique_initializer_list_alloc)
{
	const std::initializer_list<std::pair<int, std::string>> values = {
		{ 2, "two" },
		{ 3, "three" },
		{ 1, "one" },
	};
	unordered_flat_map<int, std::string> x{
		unsorted_unique,
		values,
		unordered_flat_map<int, std::string>::key_container_type::allocator_type{}
	};
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(x.size(), 3u);
	EXPECT_EQ(x.at(1), "one");
	EXPECT_EQ(x.at(2), "two");
	EXPECT_EQ(x.at(3), "three");
}
TEST(sh_unordered_flat_map, operator_assign)
{
	{
		using map_type = unordered_flat_map<int, std::string>;
		map_type x;
		x.try_emplace(1, "one");
		ASSERT_FALSE(x.empty());
		ASSERT_EQ(x.size(), 1u);
		ASSERT_TRUE(x.contains(1));

		map_type y;
		y = x;
		EXPECT_FALSE(y.empty());
		EXPECT_EQ(y.size(), 1u);
		EXPECT_EQ(y.at(1), "one");
	}
	{
		using map_type = unordered_flat_map<int, int>;
		map_type x;
		x.try_emplace(1, 1111);
		ASSERT_FALSE(x.empty());
		ASSERT_EQ(x.size(), 1u);
		ASSERT_TRUE(x.contains(1));

		map_type y;
		y = x;
		EXPECT_FALSE(y.empty());
		EXPECT_EQ(y.size(), 1u);
		EXPECT_EQ(y.at(1), 1111);
	}
	{
		using map_type = unordered_flat_map<int, std::string>;
		map_type x;
		x.try_emplace(1, "one");
		ASSERT_FALSE(x.empty());
		ASSERT_EQ(x.size(), 1u);
		ASSERT_TRUE(x.contains(1));

		x = x;
		EXPECT_FALSE(x.empty());
		EXPECT_EQ(x.size(), 1u);
		EXPECT_TRUE(x.contains(1));
	}
}

TEST(sh_unordered_flat_map, operator_assign_move)
{
	unordered_flat_map<int, std::string> x;
	x.try_emplace(1, "one");
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 1u);
	ASSERT_TRUE(x.contains(1));

	unordered_flat_map<int, std::string> y;
	y = std::move(x);
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 1u);
	EXPECT_EQ(y.at(1), "one");

	y = std::move(y);
}
TEST(sh_unordered_flat_map, key_eq)
{
	unordered_flat_map<int, std::string> x;
	EXPECT_TRUE(x.key_eq()(1, 1));
	EXPECT_FALSE(x.key_eq()(1, 2));
}
TEST(sh_unordered_flat_map, begin_end)
{
	unordered_flat_map<int, std::string> x = {
		{ 1, "one" },
		{ 2, "two" },
		{ 3, "three" },
		{ 4, "four" },
	};

	int key_sum = 0;
	int value_sum = 0;
	for (auto it = x.begin(); it != x.end(); ++it)
	{
		static_assert(std::is_const_v<std::remove_reference_t<decltype(it->first)>>, "iterator key expected to be read-only");
		static_assert(false == std::is_const_v<std::remove_reference_t<decltype(it->second)>>, "iterator value expected to be mutable");
		key_sum += it->first;
		for (const char c : it->second)
		{
			value_sum += c - 'a';
		}
	}
	EXPECT_EQ(key_sum, 10);
	EXPECT_EQ(value_sum, 193);
}
TEST(sh_unordered_flat_map, cbegin_cend)
{
	const unordered_flat_map<int, std::string> x = {
		{ 1, "one" },
		{ 2, "two" },
		{ 3, "three" },
		{ 4, "four" },
	};

	int key_sum = 0;
	int value_sum = 0;
	for (auto it = x.cbegin(); it != x.cend(); ++it)
	{
		static_assert(std::is_const_v<std::remove_reference_t<decltype(it->second)>>, "dereferenced const_iterator value expected to be read-only");
		key_sum += it->first;
		for (const char c : it->second)
		{
			value_sum += c - 'a';
		}
	}
	EXPECT_EQ(key_sum, 10);
	EXPECT_EQ(value_sum, 193);
}
TEST(sh_unordered_flat_map, clear)
{
	unordered_flat_map<int, std::string> x = {
		{ 1, "one" },
		{ 2, "two" },
		{ 3, "three" },
		{ 4, "four" },
	};
	ASSERT_FALSE(x.empty());
	for (int i = 1; i <= 4; ++i)
	{
		ASSERT_TRUE(x.contains(i));
	}
	ASSERT_EQ(x.size(), 4u);

	x.clear();
	EXPECT_TRUE(x.empty());
	for (int i = 1; i <= 4; ++i)
	{
		EXPECT_FALSE(x.contains(i));
	}
	EXPECT_EQ(x.size(), 0u);
}
TEST(sh_unordered_flat_map, swap)
{
	unordered_flat_map<int, std::string> y, x = {
		{ 1, "one" },
		{ 2, "two" },
		{ 3, "three" },
		{ 4, "four" },
	};
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 4u);
	ASSERT_TRUE(y.empty());
	ASSERT_EQ(y.size(), 0u);

	x.swap(y);
	EXPECT_TRUE(x.empty());
	EXPECT_EQ(x.size(), 0u);
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 4u);

	y.swap(y);
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 4u);
}
TEST(sh_unordered_flat_map, insert_copy)
{
	unordered_flat_map<int, std::string> x;
	{
		const std::pair<int, std::string> value(1, "one");
		const auto it = x.insert(value);
		EXPECT_TRUE(it.second);
		EXPECT_EQ(it.first->first, 1);
		EXPECT_EQ(it.first->second, "one");
	}
	{
		const std::pair<int, std::string> value(1, "ONE");
		const auto it = x.insert(value);
		EXPECT_FALSE(it.second);
		EXPECT_EQ(it.first->first, 1);
		EXPECT_EQ(it.first->second, "one");
	}
}
TEST(sh_unordered_flat_map, insert_hint_copy)
{
	unordered_flat_map<int, std::string> x;
	auto hint = x.insert({ 0, "hint" }).first;
	{
		const std::pair<int, std::string> value(1, "one");
		const auto it = hint = x.insert(hint, value);
		EXPECT_EQ(it->first, 1);
		EXPECT_EQ(it->second, "one");
	}
	{
		const std::pair<int, std::string> value(1, "ONE");
		const auto it = x.insert(hint, value);
		EXPECT_EQ(it->first, 1);
		EXPECT_EQ(it->second, "one");
	}
}
TEST(sh_unordered_flat_map, insert_move)
{
	unordered_flat_map<int, std::string> x;
	{
		std::pair<int, std::string> value(1, "one");
		const auto it = x.insert(std::move(value));
		EXPECT_TRUE(it.second);
		EXPECT_EQ(it.first->first, 1);
		EXPECT_EQ(it.first->second, "one");
	}
	{
		std::pair<int, std::string> value(1, "ONE");
		const auto it = x.insert(std::move(value));
		EXPECT_FALSE(it.second);
		EXPECT_EQ(it.first->first, 1);
		EXPECT_EQ(it.first->second, "one");
	}
}
TEST(sh_unordered_flat_map, insert_hint_move)
{
	unordered_flat_map<int, std::string> x;
	auto hint = x.insert({ 0, "hint" }).first;
	{
		std::pair<int, std::string> value(1, "one");
		const auto it = hint = x.insert(hint, std::move(value));
		EXPECT_EQ(it->first, 1);
		EXPECT_EQ(it->second, "one");
	}
	{
		std::pair<int, std::string> value(1, "ONE");
		const auto it = x.insert(hint, std::move(value));
		EXPECT_EQ(it->first, 1);
		EXPECT_EQ(it->second, "one");
	}
}
TEST(sh_unordered_flat_map, insert_first_last)
{
	unordered_flat_map<int, std::string> x;
	{
		const std::vector<std::pair<int, std::string>> values = {
			{ 2, "two" }, { 1, "one" }, { 3, "three" }, { 1, "ONE" }
		};
		x.insert(values.begin(), values.end());
		EXPECT_EQ(x.at(1), "one");
		EXPECT_EQ(x.at(2), "two");
		EXPECT_EQ(x.at(3), "three");
	}
	{
		const std::vector<std::pair<int, std::string>> values = {
			{ 1, "one" }, { 2, "two" }, { 3, "three" }, { 4, "four" }, { 1, "ONE" }
		};
		x.insert(values.begin(), values.end());
		EXPECT_EQ(x.at(1), "one");
		EXPECT_EQ(x.at(2), "two");
		EXPECT_EQ(x.at(3), "three");
		EXPECT_EQ(x.at(4), "four");
	}
}
TEST(sh_unordered_flat_map, insert_sorted_unique_first_last)
{
	unordered_flat_map<int, std::string> x;
	{
		const std::vector<std::pair<int, std::string>> values = {
			{ 1, "one" }, { 2, "two" }, { 3, "three" }
		};
		x.insert(unsorted_unique, values.begin(), values.end());
		EXPECT_EQ(x.at(1), "one");
		EXPECT_EQ(x.at(2), "two");
		EXPECT_EQ(x.at(3), "three");
	}
	{
		const std::vector<std::pair<int, std::string>> values = {
			{ 1, "one" }, { 2, "two" }, { 3, "three" }, { 4, "four" }
		};
		x.insert(unsorted_unique, values.begin(), values.end());
		EXPECT_EQ(x.at(1), "one");
		EXPECT_EQ(x.at(2), "two");
		EXPECT_EQ(x.at(3), "three");
		EXPECT_EQ(x.at(4), "four");
	}
}
TEST(sh_unordered_flat_map, insert_initializer_list)
{
	unordered_flat_map<int, std::string> x;
	{
		const std::initializer_list<std::pair<int, std::string>> values = {
			{ 2, "two" }, { 1, "one" }, { 3, "three" }, { 1, "ONE" }
		};
		x.insert(values);
		EXPECT_EQ(x.at(1), "one");
		EXPECT_EQ(x.at(2), "two");
		EXPECT_EQ(x.at(3), "three");
	}
	{
		const std::initializer_list<std::pair<int, std::string>> values = {
			{ 1, "one" }, { 2, "two" }, { 3, "three" }, { 4, "four" }, { 1, "ONE" }
		};
		x.insert(values);
		EXPECT_EQ(x.at(1), "one");
		EXPECT_EQ(x.at(2), "two");
		EXPECT_EQ(x.at(3), "three");
		EXPECT_EQ(x.at(4), "four");
	}
}
TEST(sh_unordered_flat_map, insert_sorted_unique_initializer_list)
{
	unordered_flat_map<int, std::string> x;
	{
		const std::initializer_list<std::pair<int, std::string>> values = {
			{ 1, "one" }, { 2, "two" }, { 3, "three" }
		};
		x.insert(unsorted_unique, values);
		EXPECT_EQ(x.at(1), "one");
		EXPECT_EQ(x.at(2), "two");
		EXPECT_EQ(x.at(3), "three");
	}
	{
		const std::initializer_list<std::pair<int, std::string>> values = {
			{ 1, "one" }, { 2, "two" }, { 3, "three" }, { 4, "four" }
		};
		x.insert(unsorted_unique, values);
		EXPECT_EQ(x.at(1), "one");
		EXPECT_EQ(x.at(2), "two");
		EXPECT_EQ(x.at(3), "three");
		EXPECT_EQ(x.at(4), "four");
	}
}
TEST(sh_unordered_flat_map, insert_or_assign)
{
	unordered_flat_map<int, std::string> x;
	{
		const auto it = x.insert_or_assign(1, "one");
		EXPECT_TRUE(it.second);
		EXPECT_EQ(it.first->first, 1);
		EXPECT_EQ(it.first->second, "one");
	}
	{
		const auto it = x.insert_or_assign(1, "one!");
		EXPECT_FALSE(it.second);
		EXPECT_EQ(it.first->first, 1);
		EXPECT_EQ(it.first->second, "one!");
	}

	const auto hint = x.insert_or_assign(2, "two");
	{
		const auto it = x.insert_or_assign(hint.first, 3, "three");
		EXPECT_EQ(it->first, 3);
		EXPECT_EQ(it->second, "three");
	}
	{
		const auto it = x.insert_or_assign(hint.first, 3, "three!");
		EXPECT_EQ(it->first, 3);
		EXPECT_EQ(it->second, "three!");
	}
	EXPECT_EQ(x.size(), 3u);
}
TEST(sh_unordered_flat_map, insert_or_assign_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	unordered_flat_map<int, std::string, equal_to_transparent<int>> x
	{
		equal_to_transparent<int>{ compared_normally, compared_transparently }
	};
	{
		const auto it = x.insert_or_assign(1, "one");
		EXPECT_TRUE(it.second);
		EXPECT_EQ(it.first->first, 1);
		EXPECT_EQ(it.first->second, "one");
	}
	{
		const auto it = x.insert_or_assign(static_cast<unsigned short>(1), "one!");
		EXPECT_FALSE(it.second);
		EXPECT_EQ(it.first->first, 1);
		EXPECT_EQ(it.first->second, "one!");
	}
	ASSERT_EQ(0u, compared_normally);
	ASSERT_LE(1u, compared_transparently);
}
TEST(sh_unordered_flat_map, insert_or_assign_hint_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	unordered_flat_map<int, std::string, equal_to_transparent<int>> x
	{
		equal_to_transparent<int>{ compared_normally, compared_transparently }
	};
	const auto hint = x.insert_or_assign(2, "two");
	{
		const auto it = x.insert_or_assign(hint.first, 3, "three");
		EXPECT_EQ(it->first, 3);
		EXPECT_EQ(it->second, "three");
	}
	{
		const auto it = x.insert_or_assign(hint.first, static_cast<unsigned short>(3), "three!");
		EXPECT_EQ(it->first, 3);
		EXPECT_EQ(it->second, "three!");
	}
	ASSERT_LE(1u, compared_transparently);
}
TEST(sh_unordered_flat_map, emplace)
{
	unordered_flat_map<int, std::string> x;
	{
		const auto it = x.emplace(1, "one");
		EXPECT_TRUE(it.second);
		EXPECT_EQ(it.first->first, 1);
		EXPECT_EQ(it.first->second, "one");
	}
	{
		const auto it = x.emplace(1, "one!");
		EXPECT_FALSE(it.second);
		EXPECT_EQ(it.first->first, 1);
		EXPECT_EQ(it.first->second, "one");
	}
	ASSERT_EQ(x.size(), 1u);
}
TEST(sh_unordered_flat_map, emplace_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	unordered_flat_map<int, int, equal_to_transparent<int>> x
	{
		equal_to_transparent<int>{ compared_normally, compared_transparently }
	};

	x.emplace(1, 100);
	EXPECT_EQ(compared_normally, 0u);
	EXPECT_EQ(compared_transparently, 0u);

	x.emplace(1, 100);
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x.emplace(static_cast<unsigned short>(2), 200);
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_unordered_flat_map, emplace_hint)
{
	unordered_flat_map<int, std::string> x;

	const auto hint = x.try_emplace(1, "one");
	ASSERT_TRUE(hint.second);
	EXPECT_EQ(hint.first->first, 1);
	EXPECT_EQ(hint.first->second, "one");

	{
		const auto it = x.emplace_hint(hint.first, 2, "two");
		EXPECT_EQ(it->first, 2);
		EXPECT_EQ(it->second, "two");
	}
	ASSERT_EQ(x.size(), 2u);
}
TEST(sh_unordered_flat_map, try_emplace)
{
	unordered_flat_map<int, std::string> x;
	{
		const auto it = x.try_emplace(1, "one");
		EXPECT_TRUE(it.second);
		EXPECT_EQ(it.first->first, 1);
		EXPECT_EQ(it.first->second, "one");
	}
	{
		const auto it = x.try_emplace(1, "one!");
		EXPECT_FALSE(it.second);
		EXPECT_EQ(it.first->first, 1);
		EXPECT_EQ(it.first->second, "one");
	}
	ASSERT_EQ(x.size(), 1u);
}
TEST(sh_unordered_flat_map, try_emplace_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	unordered_flat_map<int, int, equal_to_transparent<int>> x
	{
		equal_to_transparent<int>{ compared_normally, compared_transparently }
	};

	x.try_emplace(1, 100);
	EXPECT_EQ(compared_normally, 0u);
	EXPECT_EQ(compared_transparently, 0u);

	x.try_emplace(1, 100);
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x.try_emplace(static_cast<unsigned short>(2), 200);
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_unordered_flat_map, try_emplace_hint)
{
	unordered_flat_map<int, std::string> x;

	const auto hint = x.try_emplace(1, "one");
	ASSERT_TRUE(hint.second);
	EXPECT_EQ(hint.first->first, 1);
	EXPECT_EQ(hint.first->second, "one");

	{
		const auto it = x.try_emplace(hint.first, 2, "two");
		EXPECT_EQ(it->first, 2);
		EXPECT_EQ(it->second, "two");
	}
	ASSERT_EQ(x.size(), 2u);
}
TEST(sh_unordered_flat_map, erase)
{
	unordered_flat_map<int, std::string> x = {
		{ 1, "one" },
		{ 2, "two" },
		{ 3, "three" },
		{ 4, "four" },
	};
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 4u);

	std::ptrdiff_t sum = 0;
	std::size_t count = 0;

	auto it = std::next(x.begin());
	ASSERT_NE(it, x.end());
	while (it != x.end())
	{
		sum += it->first;
		++count;
		it = x.erase(it);

		ASSERT_EQ(x.size(), 4u - count);
	}
	ASSERT_EQ(count, 3u);
	ASSERT_EQ(x.size(), 1u);
	ASSERT_FALSE(x.empty());

	it = x.begin();
	sum += it->first;
	++count;
	it = x.erase(it);
	ASSERT_EQ(it, x.end());

	EXPECT_EQ(sum, 10);
	EXPECT_EQ(count, 4u);
	EXPECT_EQ(x.size(), 0u);
	EXPECT_TRUE(x.empty());
}
TEST(sh_unordered_flat_map, erase_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	unordered_flat_map<int, int, equal_to_transparent<int>> x
	{
		unsorted_unique,
		{ { 1, 100 }, { 2, 200 } },
		equal_to_transparent<int>{ compared_normally, compared_transparently }
	};
	ASSERT_EQ(2u, x.size());

	x.erase(1);
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x.erase(static_cast<unsigned short>(2));
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_unordered_flat_map, erase_range)
{
	unordered_flat_map<std::size_t, std::size_t> x;

	constexpr std::size_t min = 47;
	constexpr std::size_t size = 149;
	for (std::size_t a = min; a < min + size; ++a)
	{
		x.try_emplace(a, a * 100);
	}
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), size);

	const auto first = std::next(x.begin());
	const auto front_key = x.begin()->first;
	const auto last = std::prev(x.end());
	const auto back_key = last->first;
	auto result = x.erase(first, last);

	ASSERT_FALSE(x.empty());
	EXPECT_EQ(result, std::prev(x.end()));
	EXPECT_EQ(x.size(), 2u);
	EXPECT_TRUE(x.contains(front_key));
	EXPECT_TRUE(x.contains(back_key));

	result = x.erase(x.begin(), x.end());
	EXPECT_TRUE(x.empty());
	EXPECT_EQ(result, x.end());
	EXPECT_EQ(x.size(), 0u);
	EXPECT_FALSE(x.contains(front_key));
	EXPECT_FALSE(x.contains(back_key));
}
TEST(sh_unordered_flat_map, operator_index)
{
	unordered_flat_map<int, std::string> x;
	ASSERT_TRUE(x.empty());
	ASSERT_EQ(x.size(), 0u);

	x[1];
	ASSERT_EQ(x.size(), 1u);
	EXPECT_TRUE(x.contains(1));
	EXPECT_EQ(x.at(1), "");

	x[1] = "one";
	ASSERT_EQ(x.size(), 1u);
	EXPECT_TRUE(x.contains(1));
	EXPECT_EQ(x.at(1), "one");
}
TEST(sh_unordered_flat_map, operator_index_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	unordered_flat_map<int, int, equal_to_transparent<int>> x
	{
		unsorted_unique,
		{ { 1, 100 }, { 2, 200 } },
		equal_to_transparent<int>{ compared_normally, compared_transparently }
	};

	x[1];
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x[static_cast<unsigned short>(2)];
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_unordered_flat_map, at)
{
	unordered_flat_map<int, std::string> x = {
		{ 1, "one" },
		{ 2, "two" },
	};
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 2u);

	EXPECT_EQ(x.at(1), "one");
	EXPECT_THROW(x.at(0), std::out_of_range);
}
TEST(sh_unordered_flat_map, at_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	unordered_flat_map<int, int, equal_to_transparent<int>> x
	{
		unsorted_unique,
		{ { 1, 100 }, { 2, 200 } },
		equal_to_transparent<int>{ compared_normally, compared_transparently }
	};

	x.at(1);
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x.at(static_cast<unsigned short>(2));
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_unordered_flat_map, count)
{
	unordered_flat_map<int, std::string> x = {
		{ 1, "one" },
		{ 2, "two" },
		{ 3, "three" },
		{ 4, "four" },
	};
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 4u);

	EXPECT_EQ(x.count(0), 0u);
	for (int i = 1; i <= 4; ++i)
	{
		EXPECT_EQ(x.count(i), 1u);
	}
}
TEST(sh_unordered_flat_map, count_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	unordered_flat_map<int, int, equal_to_transparent<int>> x
	{
		unsorted_unique,
		{ { 1, 100 }, { 2, 200 } },
		equal_to_transparent<int>{ compared_normally, compared_transparently }
	};

	x.count(1);
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x.count(static_cast<unsigned short>(2));
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_unordered_flat_map, contains)
{
	unordered_flat_map<int, std::string> x = {
		{ 1, "one" },
		{ 2, "two" },
		{ 3, "three" },
		{ 4, "four" },
	};
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 4u);

	for (int i = 1; i <= 4; ++i)
	{
		EXPECT_TRUE(x.contains(i));
	}
}
TEST(sh_unordered_flat_map, contains_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	unordered_flat_map<int, int, equal_to_transparent<int>> x
	{
		unsorted_unique,
		{ { 1, 100 }, { 2, 200 } },
		equal_to_transparent<int>{ compared_normally, compared_transparently }
	};

	x.contains(1);
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x.contains(static_cast<unsigned short>(2));
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_unordered_flat_map, find)
{
	unordered_flat_map<int, std::string> x = {
		{ 1, "one" },
		{ 2, "two" },
		{ 3, "three" },
		{ 4, "four" },
	};
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 4u);

	EXPECT_EQ(x.find(0), x.end());
	for (int i = 1; i <= 4; ++i)
	{
		EXPECT_NE(x.find(i), x.end());
	}
}
TEST(sh_unordered_flat_map, find_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	unordered_flat_map<int, int, equal_to_transparent<int>> x
	{
		unsorted_unique,
		{ { 1, 100 }, { 2, 200 } },
		equal_to_transparent<int>{ compared_normally, compared_transparently }
	};

	x.find(1);
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x.find(static_cast<unsigned short>(2));
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_unordered_flat_map, equal_range)
{
	unordered_flat_map<int, std::string> x = {
		{ 1, "one" },
		{ 2, "two" },
		{ 3, "three" },
		{ 4, "four" },
	};
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 4u);

	{
		const auto range = x.equal_range(0);
		EXPECT_EQ(range.first, range.second);
		EXPECT_EQ(range.first, x.end());
		EXPECT_EQ(range.second, x.end());
	}
	{
		const auto range = x.equal_range(2);
		EXPECT_NE(range.first, range.second);
		EXPECT_NE(range.first, x.end());
		ASSERT_NE(range.first, x.end());
		EXPECT_EQ(range.first->first, 2);
		EXPECT_EQ(range.first->second, "two");
	}
}
