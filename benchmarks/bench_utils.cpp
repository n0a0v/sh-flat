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

#include <climits>
#include <iomanip>

namespace splitmix
{
	/*  Written in 2015 by Sebastiano Vigna (vigna@acm.org)

	To the extent possible under law, the author has dedicated all copyright
	and related and neighboring rights to this software to the public domain
	worldwide. This software is distributed without any warranty.

	See <http://creativecommons.org/publicdomain/zero/1.0/>. */
	// https://xorshift.di.unimi.it/splitmix64.c
	std::uint64_t generate_64(std::uint64_t& state) noexcept
	{
		std::uint64_t result{ state += 0x9e3779b97f4a7c15 };
		result = (result ^ (result >> 30)) * 0xbf58476d1ce4e5b9;
		result = (result ^ (result >> 27)) * 0x94d049bb133111eb;
		return result ^ (result >> 31);
	}
} // namespace splitmix

namespace xoshiro
{
	/*  Written in 2018 by David Blackman and Sebastiano Vigna (vigna@acm.org)

	To the extent possible under law, the author has dedicated all copyright
	and related and neighboring rights to this software to the public domain
	worldwide. This software is distributed without any warranty.

	See <http://creativecommons.org/publicdomain/zero/1.0/>. */
	// https://prng.di.unimi.it/xoshiro256starstar.c
	void seed_256ss(std::uint64_t seed, std::array<std::uint64_t, 4>& state) noexcept
	{
		for (std::uint64_t& element : state)
		{
			element = splitmix::generate_64(seed);
		}
	}
	template <std::uint8_t Bits, typename T>
	constexpr T rotate_left(const T input) noexcept
	{
		static_assert(std::is_unsigned_v<T>);
		return (input << Bits) | (input >> (sizeof(input)*CHAR_BIT - Bits));
	}
	std::uint64_t generate_256ss(std::array<std::uint64_t, 4>& state) noexcept
	{
		const std::uint64_t result{ rotate_left<7>(state[1] * 5) * 9 };
		const std::uint64_t t{ state[1] << 17 };

		state[2] ^= state[0];
		state[3] ^= state[1];
		state[1] ^= state[2];
		state[0] ^= state[3];
		state[2] ^= t;
		state[3] = rotate_left<45>(state[3]);

		return result;
	}
} // namespace xoshiro

namespace bench
{
	random::random(const std::uint64_t seed) noexcept
	{
		this->seed(seed);
	}
	void random::seed(const std::uint64_t seed) noexcept
	{
		xoshiro::seed_256ss(seed, m_state);
	}
	std::uint64_t random::generate() noexcept
	{
		return xoshiro::generate_256ss(m_state);
	}

	const char* type_name<std::string>::operator()() const noexcept
	{
		return "std::string";
	}
	const char* type_name<std::uint16_t>::operator()() const noexcept
	{
		return "std::uint16_t";
	}
	const char* type_name<std::uint32_t>::operator()() const noexcept
	{
		return "std::uint32_t";
	}
	const char* type_name<std::uint64_t>::operator()() const noexcept
	{
		return "std::uint64_t";
	}

	std::ostream& operator<<(std::ostream& ostr, const test_parameters& param)
	{
		return ostr << param.m_repetitions << " repetitions, " << param.m_operations << " operations";
	}
	std::ostream& operator<<(std::ostream& ostr, const map_parameters& param)
	{
		ostr << static_cast<const test_parameters&>(param);
		if (param.m_reserve != 0)
		{
			ostr << ", " << param.m_reserve << " reserved";
		}
		if (param.m_key_modulo != 0)
		{
			ostr << ", " << param.m_key_modulo << " key modulo";
		}
		if (param.m_fill_size != 0)
		{
			const std::size_t skipped = param.m_fill_skip == 0 ? 0 : param.m_fill_size / param.m_fill_skip;
			ostr << ", " << (param.m_fill_size - skipped) << " pre-filled";
		}
		return ostr;
	}
	auto map_result::merge() const noexcept
		-> value_type
	{
		return m_size_call + m_size_sum +
			m_insert_call + m_inserted +
			m_find_call + m_found +
			m_erase_call + m_erased;
	}
	map_result& map_result::size(const std::size_t size) noexcept
	{
		++m_size_call;
		m_size_sum += size;
		return *this;
	}
	map_result& map_result::insert(const bool inserted) noexcept
	{
		++m_insert_call;
		m_inserted += inserted ? 1 : 0;
		return *this;
	}
	map_result& map_result::find(const bool found) noexcept
	{
		++m_find_call;
		m_found += found ? 1 : 0;
		return *this;
	}
	map_result& map_result::erase(const std::size_t count) noexcept
	{
		++m_erase_call;
		m_erased += count;
		return *this;
	}
	bool operator==(const map_result& lhs, const map_result& rhs) noexcept
	{
		return lhs.m_size_call == rhs.m_size_call
			&& lhs.m_size_sum == rhs.m_size_sum
			&& lhs.m_insert_call == rhs.m_insert_call
			&& lhs.m_inserted == rhs.m_inserted
			&& lhs.m_find_call == rhs.m_find_call
			&& lhs.m_found == rhs.m_found
			&& lhs.m_erase_call == rhs.m_erase_call
			&& lhs.m_erased == rhs.m_erased;
	}
	std::ostream& operator<<(std::ostream& ostr, const map_result& result)
	{
		return ostr << '{'
			<< result.m_size_call << ", "
			<< result.m_size_sum << ", "
			<< result.m_insert_call << ", "
			<< result.m_inserted << ", "
			<< result.m_find_call << ", "
			<< result.m_found << ", "
			<< result.m_erase_call << ", "
			<< result.m_erased << "}";
	}
	std::ostream& operator<<(std::ostream& ostr, const thousands& t)
	{
		if (t.m_value == 0)
		{
			return ostr << '0';
		}
		const auto print_1000 = [&ostr](auto& print_1000, const std::size_t value) -> std::ostream&
		{
			if (value >= 1'000)
			{
				return print_1000(print_1000, value / 1'000)
					<< ',' << std::setw(3) << std::setfill('0') << (value % 1000);
			}
			else
			{
				return ostr << value;
			}
		};
		return print_1000(print_1000, t.m_value);
	}
	std::ostream& operator<<(std::ostream& ostr, const nanoseconds& ns)
	{
		const std::size_t count = ns.m_value / std::chrono::nanoseconds(1);
		return ostr << thousands{ count } << " ns";
	}

} // namespace bench
