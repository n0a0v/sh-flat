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

#ifndef INC_SH__BENCH_UTILS_HPP
#define INC_SH__BENCH_UTILS_HPP

#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <type_traits>
#include <typeinfo>
#include <unordered_set>
#include <vector>

// Permutations
#include <map>
#include <sh/flat_map.hpp>
#include <sh/unordered_flat_map.hpp>
#include <unordered_map>

#if defined(__GNUC__) || defined(__llvm__)
	#ifndef NDEBUG
	#pragma GCC warning "Debugging enabled."
	#endif // !NDEBUG

	#if SH_ROBINHOOD_DEBUG_ITERATOR != 0
	#pragma GCC warning "Debug iterator enabled."
	#endif // SH_ROBINHOOD_DEBUG_ITERATOR
#endif // __GNUC__ || __llvm__

namespace bench
{
	using clock = std::chrono::steady_clock;

	class random final
	{
	public:
		static constexpr std::uint64_t default_seed = 0x2a4e'fd97'acc0'935b;

		explicit random(std::uint64_t seed = default_seed) noexcept;
		random(const random&) noexcept = default;
		random& operator=(const random&) noexcept = default;

		void seed(const std::uint64_t value) noexcept;
		std::uint64_t generate() noexcept;

	private:
		std::array<std::uint64_t, 4> m_state;
	};

	template <typename T, typename = void> struct has_reserve : std::false_type {};
	template <typename T> struct has_reserve<T,
		std::void_t<
			decltype(
				std::declval<T>().reserve(
					std::size_t{}
				)
			)
		>
	> : std::true_type {};
	template <typename T> constexpr bool has_reserve_v = has_reserve<T>::value;

	template <typename T, typename = void>
	constexpr bool is_transparent_v = false;

	template <typename T>
	constexpr bool is_transparent_v<T, std::void_t<typename T::is_transparent>> = true;

	template <typename T, std::size_t N>
	class big final
	{
	public:
		using value_type = T;
		static_assert(N > 0);

		big() = default;
		big(const big&) noexcept = default;
		big(big&&) noexcept = default;
		big& operator=(const big&) noexcept = default;
		big& operator=(big&&) noexcept = default;

		/* implicit */ big(const value_type& value) noexcept
		{
			m_values.fill(value);
		}
		constexpr /* implicit */ operator const value_type& () const
		{
			return m_values[0];
		}
		constexpr bool operator==(const value_type& value) const
		{
			return m_values[0] == value;
		}

	private:
		std::array<value_type, N> m_values;
	};

	template <typename T>
	struct is_unsigned : std::is_unsigned<T>
	{ };
	template <typename T>
	constexpr bool is_unsigned_v = is_unsigned<T>::value;

	template <typename T>
	struct numeric_limits : std::numeric_limits<T>
	{ };

	template <typename T, std::size_t N>
	struct is_unsigned <bench::big<T, N>> : is_unsigned<T>
	{ };
	template <typename T, std::size_t N>
	struct numeric_limits <bench::big<T, N>> : std::numeric_limits<T>
	{ };

	template <typename T>
	struct type_name
	{
		const char* operator()() const noexcept
		{
			return typeid(T).name();
		}
	};
	template <>
	struct type_name <std::string>
	{
		const char* operator()() const noexcept;
	};
	template <>
	struct type_name <std::uint16_t>
	{
		const char* operator()() const noexcept;
	};
	template <>
	struct type_name <std::uint32_t>
	{
		const char* operator()() const noexcept;
	};
	template <>
	struct type_name <std::uint64_t>
	{
		const char* operator()() const noexcept;
	};
	template <typename T, std::size_t N>
	struct type_name <big<T, N>>
	{
		const char* operator()() const
		{
			static std::string instance =
				(std::ostringstream{} << "big w/" << type_name<T>{}() << "[" << N << ']').str();
			return instance.c_str();
		}
	};
	template <typename Key, typename Mapped, typename Compare, typename Allocator>
	struct type_name <std::map<Key, Mapped, Compare, Allocator>>
	{
		const char* operator()() const
		{
			static const std::string instance = 
				(std::ostringstream{} << "std::map<" << type_name<Key>{}() << ", " << type_name<Mapped>{}() << '>').str();
			return instance.c_str();
		}
	};
	template <typename Key, typename Mapped, typename Hash, typename KeyEqual, typename Allocator>
	struct type_name <std::unordered_map<Key, Mapped, Hash, KeyEqual, Allocator>>
	{
		const char* operator()() const
		{
			static const std::string instance =
				(std::ostringstream{} << "std::unordered_map<" << type_name<Key>{}() << ", " << type_name<Mapped>{}() << '>').str();
			return instance.c_str();
		}
	};
	template <typename Key, typename Mapped, typename Compare, typename Allocator>
	struct type_name <sh::flat_map<Key, Mapped, Compare, Allocator>>
	{
		const char* operator()() const
		{
			static const std::string instance =
				(std::ostringstream{} << "sh::flat_map<" << type_name<Key>{}() << ", " << type_name<Mapped>{}() << '>').str();
			return instance.c_str();
		}
	};
	template <typename Key, typename Mapped, typename KeyEqual, typename Allocator>
	struct type_name <sh::unordered_flat_map<Key, Mapped, KeyEqual, Allocator>>
	{
		const char* operator()() const
		{
			static const std::string instance =
				(std::ostringstream{} << "sh::unordered_flat_map<" << type_name<Key>{}() << ", " << type_name<Mapped>{}() << '>').str();
			return instance.c_str();
		}
	};

	template <typename Map, typename KeyType = typename Map::key_type, typename SizeType = typename Map::size_type>
	void reserve_map(Map& map, const std::size_t elements)
	{
		if constexpr (has_reserve_v<Map>)
		{
			if constexpr (is_unsigned_v<KeyType>)
			{
				if (numeric_limits<KeyType>::max() < elements)
				{
					map.reserve(SizeType(numeric_limits<KeyType>::max()));
				}
				else
				{
					map.reserve(SizeType(elements));
				}
			}
			else
			{
				map.reserve(SizeType(elements));
			}
		}
	}

	template <typename T, typename U>
	T key_value_cast(U&& value)
	{
		if constexpr (std::is_same_v<T, std::string>)
		{
			return std::to_string(std::forward<U>(value));
		}
		else
		{
			return T(std::forward<U>(value));
		}
	}

	template <typename Map, typename KeyType = typename Map::key_type>
	KeyType random_key(random& r, const std::size_t key_modulo)
	{
		std::uint64_t key = r.generate();
		if (key_modulo != 0)
		{
			key %= key_modulo;
		}
		return key_value_cast<KeyType>(key);
	}

	template <typename Map, typename KeyType = typename Map::key_type, typename MappedType = typename Map::mapped_type>
	std::pair<KeyType, MappedType> random_key_value_pair(random& r, const std::size_t key_modulo)
	{
		std::uint64_t key = r.generate();
		if (key_modulo != 0)
		{
			key %= key_modulo;
		}
		const std::uint64_t value = key * 100;
		return {
			key_value_cast<KeyType>(key),
			key_value_cast<MappedType>(value)
		};
	}

	struct test_parameters
	{
		const std::size_t m_repetitions;
		const std::size_t m_operations;

		friend std::ostream& operator<<(std::ostream& ostr, const test_parameters& param);
	};
	struct map_parameters : test_parameters
	{
		const std::size_t m_reserve;
		const std::size_t m_key_modulo;
		const std::size_t m_fill_size;
		const std::size_t m_fill_skip;

		friend std::ostream& operator<<(std::ostream& ostr, const map_parameters& params);
	};

	template <typename Map>
	void fill_map(random& r, Map& map, const map_parameters& param)
	{
		for (std::size_t index = 0; index < param.m_fill_size; ++index)
		{
			if (param.m_fill_skip == 0 || (index % param.m_fill_skip) != 0)
			{
				auto [key, value] = random_key_value_pair<Map>(r, param.m_key_modulo);
				map.try_emplace(std::move(key), std::move(value));
			}
		}
	}

	class map_result final
	{
	public:
		using value_type = std::size_t;

		value_type merge() const noexcept;
		map_result& size(const std::size_t size) noexcept;
		map_result& insert(const bool inserted) noexcept;
		map_result& find(const bool found) noexcept;
		map_result& erase(const std::size_t count) noexcept;

		friend bool operator==(const map_result& lhs, const map_result& rhs) noexcept;
		friend std::ostream& operator<<(std::ostream& ostr, const map_result& result);

	private:
		value_type m_size_call{ 0 };
		value_type m_size_sum{ 0 };
		value_type m_insert_call{ 0 };
		value_type m_inserted{ 0 };
		value_type m_find_call{ 0 };
		value_type m_found{ 0 };
		value_type m_erase_call{ 0 };
		value_type m_erased{ 0 };
	};

	struct thousands final
	{
		const std::size_t m_value;

		friend std::ostream& operator<<(std::ostream& ostr, const thousands& t);
	};
	struct nanoseconds final
	{
		const std::chrono::nanoseconds m_value;

		friend std::ostream& operator<<(std::ostream& ostr, const nanoseconds& ns);
	};

	template <typename Result>
	class test_entry final
	{
	public:
		using result_type = Result;

		template <typename Tester, typename... Args>
		explicit test_entry(const std::size_t repetitions, const Tester& tester, const Args&... args)
			: m_name{ tester.name() }
		{
			for (std::size_t r = 0; r < repetitions; ++r)
			{
				auto run = tester.prepare(args...);

				const clock::time_point before = clock::now();
				result_type result{ run() };
				const clock::time_point after = clock::now();

				m_results.emplace(std::move(result));

				const auto duration = (after - before);
				m_total_duration += duration;
				if (m_count == 0)
				{
					m_min_duration = duration;
					m_max_duration = duration;
				}
				else
				{
					m_min_duration = std::min(duration, m_min_duration);
					m_max_duration = std::max(duration, m_max_duration);
				}
				++m_count;
			}
		}

		void report(std::optional<result_type>& result_check) const
		{
			std::cout << '\t' << m_name << '\n';

			if (m_results.size() > 1
				|| (m_results.size() == 1 && result_check.has_value() && m_results.find(result_check.value()) == m_results.end()))
			{
				std::cout << "\tresult check failed:";
				const char* sep = " ";
				for (const result_type& r : m_results)
				{
					std::cout << sep << r << '\n';
					sep = ", ";
				}
				std::cout << std::endl;
			}

			std::cout << "\t\tavg: " << nanoseconds{ m_total_duration / m_count } << '\n';
			std::cout << "\t\tmin: " << nanoseconds{ m_min_duration }<< '\n';
			std::cout << "\t\tmax: " << nanoseconds{ m_max_duration } << '\n';
		}

	private:
		const char* m_name;
		std::unordered_set<result_type> m_results;
		std::size_t m_count{ 0 };
		std::chrono::nanoseconds m_total_duration{};
		std::chrono::nanoseconds m_min_duration{};
		std::chrono::nanoseconds m_max_duration{};
	}; 

	template <typename Result, typename Parameters>
	class test_group final
	{
	public:
		explicit test_group(const Parameters& parameters)
			: m_parameters{ parameters }
		{ }

		template <typename Tester>
		void add_permutation()
		{
			m_entries.emplace_back(m_parameters.m_repetitions, Tester{ m_parameters }, m_parameters);
		}
		void clear()
		{
			m_entries.clear();
		}
		void report()
		{
			std::optional<Result> result_check;
			std::cout << (m_parameters) << '\n';
			for (const test_entry<Result>& p : m_entries)
			{
				p.report(result_check);
			}
			std::cout << std::flush;
		}

	private:
		Parameters m_parameters;
		std::vector<test_entry<Result>> m_entries;
	};

	using map_test_group = test_group<map_result, map_parameters>;

	template <
		template <typename Map> class Tester,
		typename Result, typename Parameters
	>
	void test_map_permutations(const bench::test_group<Result, Parameters>& g)
	{ }

	template <
		template <typename Map> class Tester,
		typename KeyType,
		typename MappedType,
		typename... KeyMappedTypes,
		typename Result, typename Parameters
	>
	void test_map_permutations(bench::test_group<Result, Parameters>& g)
	{
		constexpr bool std_hash_transparent = is_transparent_v<std::hash<KeyType>>;
		using equal_to_counterpart = std::equal_to<
			std::conditional_t<std_hash_transparent, void, KeyType>
		>;
		using less_counterpart = std::less<
			std::conditional_t<std_hash_transparent, void, KeyType>
		>;

		g.template add_permutation<
			Tester<
				std::map<KeyType, MappedType, std::less<>>
			>
		>();
		g.template add_permutation<
			Tester<
				std::unordered_map<KeyType, MappedType, std::hash<KeyType>, equal_to_counterpart>
			>
		>();
		g.template add_permutation<
			Tester<
				sh::unordered_flat_map<KeyType, MappedType, equal_to_counterpart>
			>
		>();
		g.template add_permutation<
			Tester<
				sh::flat_map<KeyType, MappedType, less_counterpart>
			>
		>();
		g.report();
		g.clear();

		test_map_permutations<Tester, KeyMappedTypes...>(g);
	}

	template <
		template <typename Map> class Tester,
		typename Result, typename Parameters
	>
	void test_common_map_permutations(bench::test_group<Result, Parameters>& g)
	{
		using big_type = bench::big<std::uint64_t, 12>;
		test_map_permutations<
			Tester,
//			std::uint16_t, std::uint16_t
//			std::uint32_t, std::uint32_t
			std::uint64_t, std::uint64_t
//			std::uint32_t, big_type
		>(g);
	}
} // namespace bench

namespace std
{
	template <typename T, size_t N>
	struct hash<bench::big<T,N>> : private hash<T>
	{
		using is_transparent = void;

		using hash<T>::operator();
		constexpr size_t operator()(const bench::big<T,N>& value) const
		{
			return this->hash<T>::operator()(static_cast<const T&>(value));
		}
	};

	template <>
	struct hash<bench::map_result> : private hash<size_t>
	{
		size_t operator()(const bench::map_result& value) const
		{
			return this->hash<size_t>::operator()(value.merge());
		}
	};
} // namespace std

#endif
