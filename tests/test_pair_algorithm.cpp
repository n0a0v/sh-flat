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

#include <sh/pair_algorithm.hpp>

#include <algorithm>
#include <map>
#include <string>
#include <utility>
#include <vector>

using sh::pair_algorithm::iterator_pair;
using sh::pair_algorithm::reference_pair;
using sh::pair_algorithm::get;

constexpr bool DEBUG_VERBOSE = true;

namespace
{
	enum class color
	{
		red, blue, green
	};

	std::ostream& operator<<(std::ostream& ostr, const color& c)
	{
		switch (c)
		{
		case color::red: return ostr << "red";
		case color::blue: return ostr << "blue";
		case color::green: return ostr << "green";
		}
		return ostr << "unknown";
	}
} // anonymous namespace

TEST(sh_pair_algorithm, iterator_pair_ctor)
{
	std::vector<color> colors = { color::red, color::blue };
	std::vector<std::string> words = { "one", "two" };
	using std::get;
	{
		const auto iter = iterator_pair{ words.begin(), colors.begin() };
		EXPECT_EQ(get<0>(iter), words.begin());
		EXPECT_EQ(get<1>(iter), colors.begin());
	}
	{
		const auto iter = iterator_pair{ words.cbegin(), colors.cbegin() };
		EXPECT_EQ(get<0>(iter), words.begin());
		EXPECT_EQ(get<1>(iter), colors.begin());
	}
}
TEST(sh_pair_algorithm, iterator_pair_ctor_pair)
{
	std::vector<std::string> words = { "one", "two" };
	std::vector<int> numbers = { 1, 2 };
	using std::get;
	{
		const auto iter = iterator_pair{ words.begin(), numbers.begin() };
		EXPECT_EQ(get<0>(iter), words.begin());
		EXPECT_EQ(get<1>(iter), numbers.begin());
	}
	{
		const auto iter = iterator_pair{ words.cbegin(), numbers.cbegin() };
		EXPECT_EQ(get<0>(iter), words.begin());
		EXPECT_EQ(get<1>(iter), numbers.begin());
	}
}
TEST(sh_pair_algorithm, iterator_pair_sort)
{
	const std::map<int, std::string> number_to_words =
	{
		{ 1, "one" },
		{ 2, "two" },
		{ 3, "three" },
		{ 4, "four" },
 		{ 5, "five" },
 		{ 6, "six" }
	};
	std::vector<std::string> words;
	std::vector<int> numbers;

	words.reserve(number_to_words.size());
	numbers.reserve(number_to_words.size());
	for (const auto& [number, word] : number_to_words)
	{
		if constexpr (DEBUG_VERBOSE)
		{
			std::cout << word << ": " << number << '\n';
		}

		numbers.emplace_back(number);
		words.emplace_back(word);
	}

	std::sort(
		iterator_pair{ words.begin(), numbers.begin() },
		iterator_pair{ words.end(), numbers.end() },
		[](const auto& lhs, const auto& rhs)
	{
		using std::get;
		return get<0>(lhs) < get<0>(rhs);
	});

	if constexpr (DEBUG_VERBOSE)
	{
		std::cout << '\n';
	}

	int sum = 0;
	{
		auto number_iter = numbers.cbegin();
		for (const auto& word : words)
		{
			if constexpr (DEBUG_VERBOSE)
			{
				std::cout << word << ": " << *number_iter << '\n';
			}

			sum += *number_iter;
			ASSERT_NE(number_to_words.find(*number_iter), number_to_words.end());
			EXPECT_EQ(number_to_words.find(*number_iter)->second, word);
			++number_iter;
		}
	}
	EXPECT_EQ(21, sum);
}
TEST(sh_pair_algorithm, reference_pair_ctor)
{
	using std::get;
	{
		color c = color::red;
		std::string word = "one";
		const auto ref = reference_pair{ word, c };
		EXPECT_EQ(get<0>(ref), word);
		EXPECT_EQ(get<1>(ref), c);
	}
	{
		const color c = color::red;
		const std::string word = "one";
		const auto ref = reference_pair{ word, c };
		EXPECT_EQ(get<0>(ref), word);
		EXPECT_EQ(get<1>(ref), c);
	}
}
TEST(sh_pair_algorithm, reference_pair_ctor_pair)
{
	using std::get;
	{
		std::string word = "one";
		int number = 1;
		const auto ref = reference_pair{ word, number };
		//EXPECT_EQ(ref.first, word);
		EXPECT_EQ(get<0>(ref), word);
		//EXPECT_EQ(ref.second, number);
		EXPECT_EQ(get<1>(ref), number);
	}
	{
		const std::string word = "one";
		const int number = 1;
		const auto ref = reference_pair{ word, number };
		//EXPECT_EQ(ref.first, word);
		EXPECT_EQ(get<0>(ref), word);
		//EXPECT_EQ(ref.second, number);
		EXPECT_EQ(get<1>(ref), number);
	}
}
TEST(sh_pair_algorithm, reference_pair_assign_operator)
{
	color c = color::red;
	std::string word = "one";
	auto ref = reference_pair{ word, c };
	{
		const auto value = std::make_pair("two", color::blue);
		ref = value;
		EXPECT_EQ(c, color::blue);
		EXPECT_EQ(word, "two");
	}
	{
		ref = std::make_pair("three", color::green);
		EXPECT_EQ(c, color::green);
		EXPECT_EQ(word, "three");
	}
}
TEST(sh_pair_algorithm, reference_pair_assign_operator_pair)
{
	std::string word = "one";
	int number = 1;
	auto ref = reference_pair{ word, number };
	{
		const auto value = std::make_pair("two", 2);
		ref = value;
		EXPECT_EQ(word, "two");
		EXPECT_EQ(number, 2);
	}
	{
		ref = std::make_pair("three", 3);
		EXPECT_EQ(word, "three");
		EXPECT_EQ(number, 3);
	}
}
TEST(sh_pair_algorithm, reference_pair_swap)
{
	// rvalue
	{
		color x_c = color::red;
		std::string x_word = "one";

		color y_c = color::blue;
		std::string y_word = "two";

		using std::swap;
		swap(
			reference_pair{ x_word, x_c },
			reference_pair{ y_word, y_c }
		);

		EXPECT_EQ(x_c, color::blue);
		EXPECT_EQ(x_word, "two");

		EXPECT_EQ(y_c, color::red);
		EXPECT_EQ(y_word, "one");
	}
	// lvalue
	{
		color x_c = color::red;
		std::string x_word = "one";
		auto x = reference_pair{ x_word, x_c };

		color y_c = color::blue;
		std::string y_word = "two";
		auto y = reference_pair{ y_word, y_c };

		using std::swap;
		swap(x, y);

		EXPECT_EQ(x_c, color::blue);
		EXPECT_EQ(x_word, "two");

		EXPECT_EQ(y_c, color::red);
		EXPECT_EQ(y_word, "one");
	}
}
TEST(sh_pair_algorithm, reference_pair_swap_pair)
{
	// rvalue
	{
		std::string x_word = "one";
		int x_number = 1;

		std::string y_word = "two";
		int y_number = 2;

		using std::swap;
		swap(
			reference_pair{ x_word, x_number },
			reference_pair{ y_word, y_number }
		);

		EXPECT_EQ(x_word, "two");
		EXPECT_EQ(x_number, 2);

		EXPECT_EQ(y_word, "one");
		EXPECT_EQ(y_number, 1);
	}
	// lvalue
	{
		std::string x_word = "one";
		int x_number = 1;
		auto x = reference_pair{ x_word, x_number };

		std::string y_word = "two";
		int y_number = 2;
		auto y = reference_pair{ y_word, y_number };

		using std::swap;
		swap(x, y);

		EXPECT_EQ(x_word, "two");
		EXPECT_EQ(x_number, 2);

		EXPECT_EQ(y_word, "one");
		EXPECT_EQ(y_number, 1);
	}
}
