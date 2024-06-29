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

using find_parameters = bench::map_parameters;

template <typename Map>
class find_tester final
{
public:
	using map_type = Map;
	using parameters = find_parameters;

	static const char* name()
	{
		static const std::string instance =
			(std::ostringstream{} << "find " << bench::type_name<Map>{}()).str();
		return instance.c_str();
	}

	explicit find_tester(const parameters& param)
	{
		bench::reserve_map(m_template, param.m_reserve);

		bench::random r{ m_random };
		bench::fill_map(r, m_template, param);
	}
	auto prepare(const parameters& param) const
	{
		return [this, param = param, r = m_random]() mutable -> bench::map_result
		{
			bench::map_result result;

			result.size(this->m_template.size());
			for (std::size_t i = 0; i < param.m_operations; ++i)
			{
				result.find(this->m_template.find(bench::random_key<map_type>(r, param.m_key_modulo)) != this->m_template.end());
			}

			return result;
		};
	}

private:
	map_type m_template;
	bench::random m_random;
};

struct find_caller final
{
	template <typename KeyType, typename MappedType>
	void operator()() const
	{
		std::size_t index = 0;
		for (std::size_t size = 1; size <= 1024; size += std::min<std::size_t>(4u, size))
		{
			const find_parameters param
			{
				/* repetitions: */ 16,
				/* operations:  */ 16'000,
				/* reserve:     */ size,
				/* key modulo:  */ 0,
				/* fill size:   */ size,
				/* fill skip:   */ 0
			};
			std::cout << '#' << index++ << ": ";
			bench::map_test_group g{ param };
			bench::test_map_permutations<find_tester, KeyType, MappedType>(g);
		}
	}
};

int main()
{
	bench::for_each_common_map_permutation(find_caller{});
}
