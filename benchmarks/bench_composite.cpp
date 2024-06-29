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

#include "bench_utils.hpp"

#include <map>
#include <sstream>

struct composite_parameters
	: bench::map_parameters
{
	const unsigned int m_insert{ 1 };
	const unsigned int m_find{ 1 };
	const unsigned int m_erase{ 1 };

	constexpr unsigned int total() const noexcept
	{
		return m_insert + m_find + m_erase;
	}

	friend std::ostream& operator<<(std::ostream& ostr, const composite_parameters& param)
	{
		return ostr << static_cast<const bench::map_parameters&>(param)
			<< ", insert " << param.m_insert << '/' << param.total()
			<< ", find " << param.m_find << '/' << param.total()
			<< ", erase " << param.m_erase << '/' << param.total();
	}
};

template <typename Map>
class composite_tester final
{
public:
	using map_type = Map;
	using parameters = composite_parameters;

	static const char* name()
	{
		static const std::string instance =
			(std::ostringstream{} << "composite " << bench::type_name<Map>{}()).str();
		return instance.c_str();
	}

	explicit composite_tester(const parameters& param)
	{
		bench::reserve_map(m_template, param.m_reserve);
		bench::fill_map(m_random, m_template, param);
	}
	auto prepare(const parameters& param) const
	{
		return [map = m_template, param,
			action_r = m_random,
			r = m_random]
			() mutable -> bench::map_result
		{
			bench::map_result result;

			result.size(map.size());
			for (std::size_t i = 0; i < param.m_operations; ++i)
			{
				std::uint64_t action{ action_r.generate() % param.total() };

				if (action < param.m_insert)
				{
					auto [key, value] = bench::random_key_value_pair<map_type>(r, param.m_key_modulo);
					result.insert(map.emplace(std::move(key), std::move(value)).second);
					continue;
				}
				action -= param.m_insert;

				if (action < param.m_find)
				{
					result.find(map.find(bench::random_key<map_type>(r, param.m_key_modulo)) != map.end());
					continue;
				}
				action -= param.m_find;

				assert(action < param.m_erase);
				result.erase(map.erase(bench::random_key<map_type>(r, param.m_key_modulo)));
			}

			return result;
		};
	}

private:
	bench::random m_random;
	map_type m_template;
};

struct composite_caller final
{
	template <typename KeyType, typename MappedType>
	void operator()() const
	{
		std::size_t index = 0;
		for (std::size_t size = 1; size <= 1024; size += std::min<std::size_t>(4u, size))
		{
			const composite_parameters param
			{
				/* repetitions: */ 16,
				/* operations:  */ 16'000,
				/* reserve:     */ size,
				/* key modulo:  */ 0,
				/* fill size:   */ size,
				/* fill skip:   */ 0,
				/* insert: */      2,
				/* find:   */      12,
				/* erase:  */      2,
			};
			std::cout << '#' << index++ << ": ";
			bench::test_group<bench::map_result, composite_parameters> g{ param };
			bench::test_map_permutations<composite_tester, KeyType, MappedType>(g);
		}
	}
};

int main()
{
	bench::for_each_common_map_permutation(composite_caller{});
}
