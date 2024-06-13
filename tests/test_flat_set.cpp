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

#include <sh/flat_set.hpp>

#include <climits>
#include <string>
#include <type_traits>

using sh::flat_set;
using sh::sorted_unique;

namespace
{
	template <typename T>
	class less_transparent
	{
	public:
		using is_transparent = void;

		explicit less_transparent(std::uint32_t& compared_normally, std::uint32_t& compared_transparently)
			: m_compared_normally{ &compared_normally }
			, m_compared_transparently{ &compared_transparently }
		{ }
		bool operator()(const T& lhs, const T& rhs) const
		{
			++(*m_compared_normally);
			return lhs < rhs;
		}
		template <typename U>
		bool operator()(const T& lhs, const U& rhs) const
		{
			++(*m_compared_transparently);
			return lhs < rhs;
		}
		template <typename U>
		bool operator()(const U& lhs, const T& rhs) const
		{
			++(*m_compared_transparently);
			return lhs < rhs;
		}
	private:
		std::uint32_t* m_compared_normally;
		std::uint32_t* m_compared_transparently;
	};
} // anonymous namespace

TEST(sh_flat_set, ctor_default)
{
	flat_set<int> x;
	EXPECT_TRUE(x.empty());
	EXPECT_EQ(x.size(), 0u);
}
TEST(sh_flat_set, ctor_copy)
{
	flat_set<int> x;
	x.emplace(1);
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 1u);
	ASSERT_TRUE(x.contains(1));

	flat_set<int> y{ x };
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 1u);
	EXPECT_TRUE(y.contains(1));
}
TEST(sh_flat_set, ctor_copy_alloc)
{
	flat_set<int> x;
	x.emplace(1);
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 1u);
	ASSERT_TRUE(x.contains(1));

	flat_set<int> y{ x, decltype(x)::container_type::allocator_type{} };
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 1u);
	EXPECT_TRUE(y.contains(1));
}
TEST(sh_flat_set, ctor_move)
{
	flat_set<int> x;
	x.emplace(1);
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 1u);
	ASSERT_TRUE(x.contains(1));

	flat_set<int> y{ std::move(x) };
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 1u);
	EXPECT_TRUE(y.contains(1));

	EXPECT_TRUE(x.empty());
}
TEST(sh_flat_set, ctor_move_alloc)
{
	flat_set<int> x;
	x.emplace(1);
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 1u);
	ASSERT_TRUE(x.contains(1));

	flat_set<int> y{ std::move(x), decltype(x)::container_type::allocator_type{} };
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 1u);
	EXPECT_TRUE(y.contains(1));

	EXPECT_TRUE(x.empty());
}
TEST(sh_flat_set, ctor_container)
{
	const flat_set<int>::container_type values = { 3, 1, 1, 2 };

	flat_set<int> x{ values };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : values)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_container_alloc)
{
	const flat_set<int>::container_type values = { 3, 1, 1, 2 };

	flat_set<int> x{ values, decltype(values)::allocator_type{} };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : values)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_container_comp_alloc)
{
	const flat_set<int>::container_type values = { 3, 1, 1, 2 };

	flat_set<int> x{ values, std::less<int>{}, decltype(values)::allocator_type{} };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : values)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_sorted_unique_container)
{
	const flat_set<int>::container_type values = { 1, 2, 3 };

	flat_set<int> x{ sorted_unique, values };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : values)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_sorted_unique_container_alloc)
{
	const flat_set<int>::container_type values = { 1, 2, 3 };

	flat_set<int> x{ sorted_unique, values, decltype(values)::allocator_type{} };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : values)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_sorted_unique_container_comp_alloc)
{
	const flat_set<int>::container_type values = { 1, 2, 3 };

	flat_set<int> x{ values, std::less<int>{}, decltype(values)::allocator_type{} };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : values)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_comp)
{
	flat_set<int> x{ std::less<int>{} };
	EXPECT_TRUE(x.empty());
}
TEST(sh_flat_set, ctor_comp_alloc)
{
	flat_set<int> x{ std::less<int>{}, flat_set<int>::container_type::allocator_type{} };
	EXPECT_TRUE(x.empty());
}
TEST(sh_flat_set, ctor_alloc)
{
	flat_set<int> x{ flat_set<int>::container_type::allocator_type{} };
	EXPECT_TRUE(x.empty());
}
TEST(sh_flat_set, ctor_first_last)
{
	const std::vector<int> values = { 3, 1, 1, 2 };

	flat_set<int> x{ values.begin(), values.end() };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : values)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_first_last_comp_alloc)
{
	const std::vector<int> values = { 3, 1, 1, 2 };

	flat_set<int> x{ values.begin(), values.end(), std::less<int>{}, flat_set<int>::container_type::allocator_type{} };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : values)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_first_last_alloc)
{
	const std::vector<int> values = { 3, 1, 1, 2 };

	flat_set<int> x{ values.begin(), values.end(), flat_set<int>::container_type::allocator_type{} };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : values)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_sorted_unique_first_last)
{
	const std::vector<int> values = { 1, 2, 3 };

	flat_set<int> x{ sorted_unique, values.begin(), values.end() };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : values)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_sorted_unique_first_last_comp_alloc)
{
	const std::vector<int> values = { 1, 2, 3 };

	flat_set<int> x{ sorted_unique, values.begin(), values.end(), std::less<int>{}, flat_set<int>::container_type::allocator_type{} };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : values)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_sorted_unique_first_last_alloc)
{
	const std::vector<int> values = { 1, 2, 3 };

	flat_set<int> x{ sorted_unique, values.begin(), values.end(), flat_set<int>::container_type::allocator_type{} };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : values)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_initializer_list)
{
	const std::initializer_list<int> ilist = { 3, 1, 1, 2 };

	flat_set<int> x{ ilist };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : ilist)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_initializer_list_comp_alloc)
{
	const std::initializer_list<int> ilist = { 3, 1, 1, 2 };

	flat_set<int> x{ ilist, std::less<int>{}, flat_set<int>::container_type::allocator_type{}  };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : ilist)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_initializer_list_alloc)
{
	const std::initializer_list<int> ilist = { 3, 1, 1, 2 };

	flat_set<int> x{ ilist, std::less<int>{}, flat_set<int>::container_type::allocator_type{}  };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : ilist)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_sorted_unique_initializer_list)
{
	const std::initializer_list<int> ilist = { 1, 2, 3 };

	flat_set<int> x{ sorted_unique, ilist };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : ilist)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_sorted_unique_initializer_list_comp_alloc)
{
	const std::initializer_list<int> ilist = { 1, 2, 3 };

	flat_set<int> x{ sorted_unique, ilist, std::less<int>{}, flat_set<int>::container_type::allocator_type{}  };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : ilist)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, ctor_sorted_unique_initializer_list_alloc)
{
	const std::initializer_list<int> ilist = { 1, 2, 3 };

	flat_set<int> x{ sorted_unique, ilist, std::less<int>{}, flat_set<int>::container_type::allocator_type{}  };
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	for (const auto& item : ilist)
	{
		EXPECT_TRUE(x.contains(item));
	}
}
TEST(sh_flat_set, operator_assign_copy)
{
	flat_set<int> x;
	x.emplace(1);
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 1u);
	ASSERT_TRUE(x.contains(1));

	flat_set<int> y;
	y = x;
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 1u);
	EXPECT_TRUE(y.contains(1));

	y = y;
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 1u);
	EXPECT_TRUE(y.contains(1));
}
TEST(sh_flat_set, operator_assign_move)
{
	flat_set<std::string> x;
	x.emplace("one");
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 1u);
	ASSERT_TRUE(x.contains("one"));

	flat_set<std::string> y;
	y = std::move(x);
	EXPECT_FALSE(y.empty());
	EXPECT_EQ(y.size(), 1u);
	EXPECT_TRUE(y.contains("one"));

	y = std::move(y);
}
TEST(sh_flat_set, begin_end)
{
	flat_set<int> x = { 1, 2, 3, 4 };
	int key_sum = 0;
	int key_last = 0;
	for (auto it = x.begin(); it != x.end(); ++it)
	{
		static_assert(std::is_const_v<std::remove_reference_t<decltype(*it)>>, "dereferenced const_iterator key expected to be read-only");
		key_sum += *it;
		EXPECT_GT(*it, key_last);
		key_last = *it;
	}
	EXPECT_EQ(key_sum, 10);
}
TEST(sh_flat_set, cbegin_cend)
{
	const flat_set<int> x = { 1, 2, 3, 4 };
	int key_sum = 0;
	int key_last = 0;
	for (auto it = x.cbegin(); it != x.cend(); ++it)
	{
		static_assert(std::is_const_v<std::remove_reference_t<decltype(*it)>>, "dereferenced const_iterator expected to be read-only");
		key_sum += *it;
		EXPECT_GT(*it, key_last);
		key_last = *it;
	}
	EXPECT_EQ(key_sum, 10);
}
TEST(sh_flat_set, rbegin_rend)
{
	flat_set<int> x = { 1, 2, 3, 4 };
	int key_sum = 0;
	int key_last = 5;
	for (auto it = x.rbegin(); it != x.rend(); ++it)
	{
		static_assert(std::is_const_v<std::remove_reference_t<decltype(*it)>>, "dereferenced const_iterator key expected to be read-only");
		key_sum += *it;
		EXPECT_LT(*it, key_last);
		key_last = *it;
	}
	EXPECT_EQ(key_sum, 10);
}
TEST(sh_flat_set, crbegin_crend)
{
	const flat_set<int> x = { 1, 2, 3, 4 };
	int key_sum = 0;
	int key_last = 5;
	for (auto it = x.crbegin(); it != x.crend(); ++it)
	{
		static_assert(std::is_const_v<std::remove_reference_t<decltype(*it)>>, "dereferenced const_iterator expected to be read-only");
		key_sum += *it;
		EXPECT_LT(*it, key_last);
		key_last = *it;
	}
	EXPECT_EQ(key_sum, 10);
}
TEST(sh_flat_set, empty)
{
	flat_set<int> x = {};
	EXPECT_TRUE(x.empty());

	x.emplace(1);
	EXPECT_FALSE(x.empty());
}
TEST(sh_flat_set, size)
{
	flat_set<int> x = {};
	EXPECT_EQ(0u, x.size());

	x.emplace(1);
	EXPECT_EQ(1u, x.size());
}
TEST(sh_flat_set, emplace)
{
	flat_set<std::string> x;
	{
		const auto it = x.emplace("one");
		EXPECT_TRUE(it.second);
		EXPECT_EQ(*it.first, "one");
	}
	{
		const auto it = x.emplace("one");
		EXPECT_FALSE(it.second);
		EXPECT_EQ(*it.first, "one");
	}
	ASSERT_EQ(x.size(), 1u);
}
TEST(sh_flat_set, emplace_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	flat_set<std::string, less_transparent<std::string>> x
	{
		less_transparent<std::string>{ compared_normally, compared_transparently }
	};

	x.emplace(std::string{ "one" });
	ASSERT_EQ(0u, compared_normally);
	ASSERT_EQ(0u, compared_transparently);

	x.emplace(std::string{ "one" });
	ASSERT_LE(1u, compared_normally);
	ASSERT_EQ(0u, compared_transparently);

	const char two[] = "two";
	x.emplace(two);
	EXPECT_LE(1u, compared_normally);
	EXPECT_LE(1u, compared_transparently);
}
TEST(sh_flat_set, emplace_hint)
{
	flat_set<std::string> x;

	const auto hint = x.emplace("one");
	ASSERT_TRUE(hint.second);
	EXPECT_EQ(*hint.first, "one");

	{
		const auto it = x.emplace_hint(hint.first, "two");
		EXPECT_EQ(*it, "two");
	}
	ASSERT_EQ(x.size(), 2u);
}
TEST(sh_flat_set, insert_copy)
{
	flat_set<std::string> x;
	{
		const std::string value("one");
		const auto it_inserted = x.insert(value);
		EXPECT_TRUE(it_inserted.second);
		EXPECT_EQ(*it_inserted.first, "one");
		EXPECT_EQ(1u, x.size());
	}
	{
		const std::string value("one");
		const auto it_inserted = x.insert(value);
		EXPECT_FALSE(it_inserted.second);
		EXPECT_EQ(*it_inserted.first, "one");
		EXPECT_EQ(1u, x.size());
	}
}
TEST(sh_flat_set, insert_hint_copy)
{
	flat_set<std::string> x;
	auto hint = x.insert("hint").first;
	{
		const std::string value("one");
		const auto it = hint = x.insert(hint, value);
		EXPECT_EQ(*it, "one");
		EXPECT_EQ(2u, x.size());
	}
	{
		const std::string value("one");
		const auto it = x.insert(hint, value);
		EXPECT_EQ(*it, "one");
		EXPECT_EQ(2u, x.size());
	}
}
TEST(sh_flat_set, insert_move)
{
	flat_set<std::string> x;
	{
		std::string value("two");
		const auto it_inserted = x.insert(std::move(value));
		EXPECT_TRUE(it_inserted.second);
		EXPECT_EQ(*it_inserted.first, "two");
		EXPECT_EQ(1u, x.size());
	}
	{
		std::string value("two");
		const auto it_inserted = x.insert(std::move(value));
		EXPECT_FALSE(it_inserted.second);
		EXPECT_EQ(*it_inserted.first, "two");
		EXPECT_EQ(1u, x.size());
	}
}
TEST(sh_flat_set, insert_hint_move)
{
	flat_set<std::string> x;
	auto hint = x.insert("hint").first;
	{
		std::string value("two");
		const auto it = hint = x.insert(hint, std::move(value));
		EXPECT_EQ(*it, "two");
		EXPECT_EQ(2u, x.size());
	}
	{
		std::string value("two");
		const auto it = x.insert(hint, std::move(value));
		EXPECT_EQ(*it, "two");
		EXPECT_EQ(2u, x.size());
	}
}
TEST(sh_flat_set, insert_first_last)
{
	flat_set<std::string> x;
	{
		const std::vector<std::string> values = { "one", "two", "three", "one" };
		x.insert(values.begin(), values.end());
		EXPECT_TRUE(x.contains("one"));
		EXPECT_TRUE(x.contains("two"));
		EXPECT_TRUE(x.contains("three"));
	}
	{
		const std::vector<std::string> values = { "one", "two", "three", "four", "one" };
		x.insert(values.begin(), values.end());
		EXPECT_TRUE(x.contains("one"));
		EXPECT_TRUE(x.contains("two"));
		EXPECT_TRUE(x.contains("three"));
		EXPECT_TRUE(x.contains("four"));
	}
}
TEST(sh_flat_set, insert_sorted_unique_first_last)
{
	flat_set<std::string> x;
	{
		const std::vector<std::string> values = { "one", "three", "two" };
		x.insert(sorted_unique, values.begin(), values.end());
		EXPECT_TRUE(x.contains("one"));
		EXPECT_TRUE(x.contains("two"));
		EXPECT_TRUE(x.contains("three"));
	}
	{
		const std::vector<std::string> values = { "four", "one", "three", "two" };
		x.insert(sorted_unique, values.begin(), values.end());
		EXPECT_TRUE(x.contains("one"));
		EXPECT_TRUE(x.contains("two"));
		EXPECT_TRUE(x.contains("three"));
		EXPECT_TRUE(x.contains("four"));
	}
}
TEST(sh_flat_set, insert_initializer_list)
{
	flat_set<std::string> x;
	{
		x.insert({ "one", "two", "three" });
		EXPECT_TRUE(x.contains("one"));
		EXPECT_TRUE(x.contains("two"));
		EXPECT_TRUE(x.contains("three"));
	}
	{
		x.insert({ "one", "two", "three", "four" });
		EXPECT_TRUE(x.contains("one"));
		EXPECT_TRUE(x.contains("two"));
		EXPECT_TRUE(x.contains("three"));
		EXPECT_TRUE(x.contains("four"));
	}
}
TEST(sh_flat_set, insert_sorted_unique_initializer_list)
{
	flat_set<std::string> x;
	{
		x.insert(sorted_unique, { "one", "three", "two" });
		EXPECT_TRUE(x.contains("one"));
		EXPECT_TRUE(x.contains("two"));
		EXPECT_TRUE(x.contains("three"));
	}
	{
		x.insert(sorted_unique, { "four", "one", "three", "two" });
		EXPECT_TRUE(x.contains("one"));
		EXPECT_TRUE(x.contains("two"));
		EXPECT_TRUE(x.contains("three"));
		EXPECT_TRUE(x.contains("four"));
	}
}
TEST(sh_flat_set, extract)
{
	std::vector<int> values{ 1, 2, 3 };
	flat_set<int> x{ sorted_unique, values.begin(), values.end() };
	auto extract = std::move(x).extract();
	EXPECT_EQ(values, extract);
}
TEST(sh_flat_set, replace)
{
	flat_set<int> x;
	std::vector<int> values{ 1, 2, 3 };
	x.replace(std::move(values));
	EXPECT_FALSE(x.empty());
	EXPECT_EQ(3u, x.size());
	EXPECT_TRUE(x.contains(1));
	EXPECT_TRUE(x.contains(2));
	EXPECT_TRUE(x.contains(3));
}
TEST(sh_flat_set, erase_iterator)
{
	{
		flat_set<int> x = { 1, 2, 3, 4 };
		ASSERT_FALSE(x.empty());
		ASSERT_EQ(x.size(), 4u);
		for (int i = 1; i <= 4; ++i)
		{
			auto it = x.find(i);
			ASSERT_TRUE(it != x.end());
			it = x.erase(it);
		}
		EXPECT_EQ(x.size(), 0u);
		EXPECT_TRUE(x.empty());
	}
	{
		flat_set<int> x = { 1, 2, 3, 4 };
		ASSERT_FALSE(x.empty());
		ASSERT_EQ(x.size(), 4u);
		for (auto it = x.begin(); it != x.end(); )
		{
			it = x.erase(it);
		}
		EXPECT_EQ(x.size(), 0u);
		EXPECT_TRUE(x.empty());
	}
}
TEST(sh_flat_set, erase_first_last)
{
	{
		flat_set<int> x = { 1, 2, 3, 4 };
		ASSERT_FALSE(x.empty());
		ASSERT_EQ(x.size(), 4u);
		const auto it = x.erase(x.begin(), x.end());
		EXPECT_EQ(it, x.end());
		EXPECT_EQ(x.size(), 0u);
		EXPECT_TRUE(x.empty());
	}
	{
		flat_set<int> x = { 1, 2, 3, 4 };
		ASSERT_FALSE(x.empty());
		ASSERT_EQ(x.size(), 4u);
		auto it = x.erase(x.begin(), std::next(x.begin()));
		it = x.erase(it, x.end());
		EXPECT_EQ(it, x.end());
		EXPECT_EQ(x.size(), 0u);
		EXPECT_TRUE(x.empty());
	}
}
TEST(sh_flat_set, erase_key)
{
	flat_set<int> x = { 1, 2, 3, 4 };
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 4u);
	for (int i = 1; i <= 4; ++i)
	{
		auto it = x.erase(i);
	}
	EXPECT_EQ(x.size(), 0u);
	EXPECT_TRUE(x.empty());
}
TEST(sh_flat_set, erase_key_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	flat_set<int, less_transparent<int>> x
	{
		sorted_unique,
		{ 1, 2, 3, 4 },
		less_transparent<int>{ compared_normally, compared_transparently }
	};

	x.erase(1);
	EXPECT_LE(1u, compared_normally);
	EXPECT_EQ(0u, compared_transparently);

	x.erase(static_cast<unsigned short>(2));
	EXPECT_LE(1u, compared_normally);
	EXPECT_LE(1u, compared_transparently);
}
TEST(sh_flat_set, swap)
{
	flat_set<int> y, x = { 1, 2, 3, 4 };
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
TEST(sh_flat_set, clear)
{
	flat_set<int> x = { 1, 2, 3, 4 };
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
TEST(sh_flat_set, find)
{
	flat_set<int> x = { 1, 2, 3, 4 };
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 4u);

	EXPECT_EQ(x.find(0), x.end());
	for (int i = 1; i <= 4; ++i)
	{
		EXPECT_NE(x.find(i), x.end());
	}
}
TEST(sh_flat_set, find_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	flat_set<int, less_transparent<int>> x
	{
		sorted_unique,
		{ 1, 2 },
		less_transparent<int>{ compared_normally, compared_transparently }
	};

	x.find(1);
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x.find(static_cast<unsigned short>(2));
	EXPECT_GE(compared_normally, 1u);
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_flat_set, count)
{
	flat_set<int> x = { 1, 2, 3, 4 };
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 4u);

	EXPECT_EQ(x.count(0), 0u);
	for (int i = 1; i <= 4; ++i)
	{
		EXPECT_EQ(x.count(i), 1u);
	}
}
TEST(sh_flat_set, count_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	flat_set<int, less_transparent<int>> x
	{
		sorted_unique,
		{ 1, 2 },
		less_transparent<int>{ compared_normally, compared_transparently }
	};

	ASSERT_EQ(0u, compared_normally);
	ASSERT_EQ(0u, compared_transparently);

	x.count(1);
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x.count(static_cast<unsigned short>(2));
	EXPECT_GE(compared_normally, 1u);
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_flat_set, contains)
{
	flat_set<int> x = { 1, 2, 3, 4 };
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 4u);

	for (int i = 1; i <= 4; ++i)
	{
		EXPECT_TRUE(x.contains(i));
	}
}
TEST(sh_flat_set, contains_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	flat_set<int, less_transparent<int>> x
	{
		{ 1, 2 },
		less_transparent<int>{ compared_normally, compared_transparently }
	};

	x.contains(1);
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x.contains(static_cast<unsigned short>(2));
	EXPECT_GE(compared_normally, 1u);
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_flat_set, lower_bound)
{
	flat_set<int> x = { 1, 2, 3, 4 };
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 4u);
	{
		const auto iter = x.lower_bound(0);
		EXPECT_EQ(iter, x.begin());
	}
	{
		const auto iter = x.lower_bound(5);
		EXPECT_EQ(iter, x.end());
	}
	{
		const auto iter = x.lower_bound(3);
		ASSERT_NE(iter, x.end());
		EXPECT_EQ(*iter, 3);
	}
}
TEST(sh_flat_set, lower_bound_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	flat_set<int, less_transparent<int>> x
	{
		{ 1, 2 },
		less_transparent<int>{ compared_normally, compared_transparently }
	};

	x.lower_bound(1);
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x.lower_bound(static_cast<unsigned short>(2));
	EXPECT_GE(compared_normally, 1u);
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_flat_set, upper_bound)
{
	flat_set<int> x = { 1, 2, 3, 4 };
	ASSERT_FALSE(x.empty());
	ASSERT_EQ(x.size(), 4u);
	{
		const auto iter = x.upper_bound(0);
		EXPECT_EQ(iter, x.begin());
	}
	{
		const auto iter = x.upper_bound(5);
		EXPECT_EQ(iter, x.end());
	}
	{
		const auto iter = x.upper_bound(3);
		ASSERT_NE(iter, x.end());
		EXPECT_EQ(*iter, 4);
	}
}
TEST(sh_flat_set, upper_bound_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	flat_set<int, less_transparent<int>> x
	{
		{ 1, 2 },
		less_transparent<int>{ compared_normally, compared_transparently }
	};

	x.upper_bound(1);
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x.upper_bound(static_cast<unsigned short>(2));
	EXPECT_GE(compared_normally, 1u);
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_flat_set, equal_range)
{
	flat_set<int> x = { 1, 2, 3, 4 };
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
		EXPECT_EQ(*range.first, 2);
	}
}
TEST(sh_flat_set, equal_range_transparent)
{
	std::uint32_t compared_normally = 0, compared_transparently = 0;
	flat_set<int, less_transparent<int>> x
	{
		{ 1, 2 },
		less_transparent<int>{ compared_normally, compared_transparently }
	};

	x.equal_range(1);
	EXPECT_GE(compared_normally, 1u);
	EXPECT_EQ(compared_transparently, 0u);

	x.equal_range(static_cast<unsigned short>(2));
	EXPECT_GE(compared_normally, 1u);
	EXPECT_GE(compared_transparently, 1u);
}
TEST(sh_flat_set, operator_equal)
{
	flat_set<int> x{ sorted_unique, { 1, 2, 3, 4 } };
	{
		flat_set<int> y = { 4, 1, 2, 3 };
		EXPECT_TRUE(x == y);
	}
	{
		flat_set<int> y = { 4, 1, 2 };
		EXPECT_FALSE(x == y);
	}
	{
		flat_set<int> y = { 4, 1, 2, 3, 5 };
		EXPECT_FALSE(x == y);
	}
}
TEST(sh_flat_set, operator_nonequal)
{
	flat_set<int> x{ sorted_unique, { 1, 2, 3, 4 } };
	{
		flat_set<int> y = { 4, 1, 2, 3 };
		EXPECT_FALSE(x != y);
	}
	{
		flat_set<int> y = { 4, 1, 2 };
		EXPECT_TRUE(x != y);
	}
	{
		flat_set<int> y = { 4, 1, 2, 3, 5 };
		EXPECT_TRUE(x != y);
	}
}
