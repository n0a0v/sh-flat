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

#ifndef INC_SH__TUPLE_ALGORITHM_HPP
#define INC_SH__TUPLE_ALGORITHM_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

namespace sh::tuple_algorithm
{
	template <typename... Types> class reference_tuple;
	template <typename Iterator, typename... Iterators> class iterator_tuple;
} // namespace sh::tuple_algorithm

namespace std
{
	template <std::size_t I, typename... Types>
	struct tuple_element<I, sh::tuple_algorithm::reference_tuple<Types...>>
		: tuple_element<I, typename sh::tuple_algorithm::reference_tuple<Types...>::underlying_type>
	{ };
	template <std::size_t I, typename... Iterators>
	struct tuple_element<I, sh::tuple_algorithm::iterator_tuple<Iterators...>>
		: tuple_element<I, typename sh::tuple_algorithm::iterator_tuple<Iterators...>::underlying_type>
	{ };

	template <typename... Types>
	struct tuple_size<sh::tuple_algorithm::reference_tuple<Types...>>
		: tuple_size<typename sh::tuple_algorithm::reference_tuple<Types...>::underlying_type>
	{ };
	template <typename... Iterators>
	struct tuple_size<sh::tuple_algorithm::iterator_tuple<Iterators...>>
		: tuple_size<typename sh::tuple_algorithm::iterator_tuple<Iterators...>::underlying_type>
	{ };
} // namespace std

namespace sh::tuple_algorithm
{
	template <std::size_t Index = 0, typename F, typename Tuple, typename... Tuples>
	void for_each_element(F&& func, Tuple&& first, Tuples&&... others)
	{
		using std::tuple_size_v;
		if constexpr (Index < tuple_size_v<std::decay_t<Tuple>>)
		{
			using std::get;
			func(
				get<Index>(first),
				get<Index>(others)...);
			for_each_element<Index + 1>(
				std::forward<F>(func),
				std::forward<Tuple>(first),
				std::forward<Tuples>(others)...);
		}
	}

	template <typename... Types>
	struct tuple
	{
		using type = std::tuple<Types...>;
	};
	template <typename First, typename Second>
	struct tuple<First, Second>
	{
		using type = std::pair<First, Second>;
	};
	template <typename... Types>
	using tuple_t = typename tuple<Types...>::type;

	template <typename Tuple, template <typename> class Apply>
	struct for_each_type;
	template <typename Tuple, template <typename> class Apply>
	using for_each_type_t = typename for_each_type<Tuple, Apply>::type;

	template <template <typename> class Apply, typename First, typename Second>
	struct for_each_type <std::pair<First, Second>, Apply>
	{
		using type = std::pair<typename Apply<First>::type, typename Apply<Second>::type>;
	};
	template <template <typename> class Apply, typename... Types>
	struct for_each_type <std::tuple<Types...>, Apply>
	{
		using type = std::tuple<typename Apply<Types>::type...>;
	};

	template <typename... Types>
	class reference_tuple final : public tuple_t<std::add_lvalue_reference_t<Types>...>
	{
	public:
		using underlying_type = tuple_t<std::add_lvalue_reference_t<Types>...>;

		template <typename... Args>
		explicit reference_tuple(Args&... args)
			: underlying_type{ args... }
		{ }

		template <typename T,
			typename = std::enable_if_t<std::tuple_size_v<underlying_type> == std::tuple_size_v<std::decay_t<T>>>
		>
		reference_tuple& operator=(T&& other) noexcept
		{
			sh::tuple_algorithm::for_each_element([](auto& lhs, auto&& rhs)
			{
				lhs = std::forward<decltype(rhs)>(rhs);
			}, *this, std::forward<T>(other));
			return *this;
		}
		friend void swap(reference_tuple& lhs, reference_tuple& rhs)
		{
			sh::tuple_algorithm::for_each_element([](auto& lhs, auto& rhs)
			{
				using std::swap;
				swap(lhs, rhs);
			}, lhs, rhs);
		}
		friend void swap(reference_tuple&& lhs, reference_tuple&& rhs)
		{
			swap(static_cast<reference_tuple&>(lhs), static_cast<reference_tuple&>(rhs));
		}
	};

	template <typename... Types>
	reference_tuple(Types&...) -> reference_tuple<Types...>;

	template <typename Iterator, typename... Iterators>
	class iterator_tuple final
	{
	public:
		using underlying_type = tuple_t<Iterator, Iterators...>;
		using difference_type = std::ptrdiff_t;
		using value_type = tuple_t<
			typename std::iterator_traits<Iterator>::value_type,
			typename std::iterator_traits<Iterators>::value_type...>;
		using reference = reference_tuple<
			typename std::iterator_traits<Iterator>::reference,
			typename std::iterator_traits<Iterators>::reference...>;
		struct pointer final
		{
			reference m_reference;
			reference* operator->()
			{
				return &m_reference;
			}
		};
		using iterator_category = typename std::iterator_traits<Iterator>::iterator_category;
		static_assert(
			(std::is_same_v<iterator_category, typename std::iterator_traits<Iterators>::iterator_category> && ...)
			, "All iterator types expected to have the same iterator_category."
		);

		iterator_tuple() = default;
		iterator_tuple(const iterator_tuple& other) = default;
		iterator_tuple(iterator_tuple&& other) = default;
		template <typename U0, typename... U1>
		iterator_tuple(const iterator_tuple<U0, U1...>& other)
			: m_iterators{ other.m_iterators }
		{ }
		template <typename... Args,
			typename = std::enable_if_t<std::is_constructible_v<underlying_type, Args...>>
		>
		explicit iterator_tuple(Args&&... args)
			: m_iterators{ std::forward<Args>(args)... }
		{ }
		iterator_tuple& operator=(const iterator_tuple&) = default;
		iterator_tuple& operator=(iterator_tuple&&) = default;

		pointer operator->() const
		{
			return pointer{ operator*() };
		}
		reference operator*() const
		{
			using std::apply;
			return reference{ apply(
				[](const auto&... iterator)
				{
					return reference{ (*iterator)... };
				},
				m_iterators
			) };
		}
		reference operator[](const difference_type offset) const
		{
			using std::apply;
			return reference{ apply(
				[offset](const auto&... iterator)
				{
					return reference{ iterator[offset]... };
				},
				m_iterators
			) };
		}
		iterator_tuple& operator++()
		{
			using std::apply;
			apply([](auto&... iterator)
			{
				(++iterator, ...);
			}, m_iterators);
			return *this;
		}
		iterator_tuple& operator--()
		{
			using std::apply;
			apply([](auto&... iterator)
			{
				(--iterator, ...);
			}, m_iterators);
			return *this;
		}
		iterator_tuple operator++(int)
		{
			iterator_tuple result{ *this };
			this->operator++();
			return result;
		}
		iterator_tuple operator--(int)
		{
			iterator_tuple result{ *this };
			this->operator--();
			return result;
		}
		iterator_tuple& operator+=(const difference_type offset)
		{
			using std::apply;
			apply([offset](auto&... iterator)
			{
				const auto plus_equal = [offset](auto& iterator)
				{
					return iterator += offset;
				};
				(plus_equal(iterator), ...);
			}, m_iterators);
			return *this;
		}
		iterator_tuple& operator-=(const difference_type offset)
		{
			using std::apply;
			apply([offset](auto&... iterator)
			{
				const auto minus_equal = [offset](auto& iterator)
				{
					return iterator -= offset;
				};
				(minus_equal(iterator), ...);
			}, m_iterators);
			return *this;
		}
		iterator_tuple operator+(const difference_type offset) const
		{
			return iterator_tuple{ *this } += offset;
		}
		iterator_tuple operator-(const difference_type offset) const
		{
			return iterator_tuple{ *this } -= offset;
		}
		friend difference_type operator-(const iterator_tuple& lhs, const iterator_tuple& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) - get<0>(rhs.m_iterators);
		}
		friend bool operator==(const iterator_tuple& lhs, const iterator_tuple& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) == get<0>(rhs.m_iterators);
		}
		friend bool operator!=(const iterator_tuple& lhs, const iterator_tuple& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) != get<0>(rhs.m_iterators);
		}
		friend bool operator<(const iterator_tuple& lhs, const iterator_tuple& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) < get<0>(rhs.m_iterators);
		}
		friend bool operator>(const iterator_tuple& lhs, const iterator_tuple& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) > get<0>(rhs.m_iterators);
		}
		friend bool operator<=(const iterator_tuple& lhs, const iterator_tuple& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) <= get<0>(rhs.m_iterators);
		}
		friend bool operator>=(const iterator_tuple& lhs, const iterator_tuple& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) >= get<0>(rhs.m_iterators);
		}
		friend void swap(iterator_tuple& lhs, iterator_tuple& rhs)
		{
			using std::swap;
			swap(lhs.m_iterators, rhs.m_iterators);
		}
		template <std::size_t I>
		friend std::tuple_element_t<I, iterator_tuple>& get(iterator_tuple& iter)
		{
			using std::get;
			return get<I>(iter.m_iterators);
		}
		template <std::size_t I>
		friend std::tuple_element_t<I, iterator_tuple>&& get(iterator_tuple&& iter)
		{
			using std::get;
			return get<I>(std::move(iter.m_iterators));
		}
		template <std::size_t I>
		friend const std::tuple_element_t<I, iterator_tuple>& get(const iterator_tuple& iter)
		{
			using std::get;
			return get<I>(iter.m_iterators);
		}

	private:
		template <typename U, typename... Us>
		friend class iterator_tuple;

		underlying_type m_iterators;
	};

	template <typename Iterator, typename... Iterators>
	iterator_tuple(iterator_tuple<Iterator, Iterators...>) -> iterator_tuple<Iterator, Iterators...>;

	template <typename... Iterators>
	iterator_tuple(Iterators...) -> iterator_tuple<Iterators...>;
} // sh::tuple_algorithm

#endif
