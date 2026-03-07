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

#ifndef INC_SH__FLAT_ALGORITHM_HPP
#define INC_SH__FLAT_ALGORITHM_HPP

#include <cassert>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

#if !defined(SH_FLAT_ASSERT)
	/**	Transparently wraps assert to allow asserts to be turned off for the sh flat_map/set-style containers in one location, if too costly.
	 */
	#define SH_FLAT_ASSERT(CONDITION, ...) \
		/* Comment: __VA_ARGS__ */ \
		assert(CONDITION)
#endif // !SH_FLAT_ASSERT

namespace sh
{

	/**	A disambiguation tag for sh::flat_map and sh::flat_set.
	 *	Denotes that provided arguments are both sorted and unique according to the associated comparator.
	 */
	constexpr struct sorted_unique_t {} sorted_unique;

	/**	A disambiguation tag for sh::unordered_flat_map and sh::unordered_flat_set.
	 *	Denotes that provided arguments are unique (but not necessarily sorted) according to the associated comparator.
	 */
	constexpr struct unsorted_unique_t {} unsorted_unique;

} // namespace sh

namespace sh::flat
{
	/**	Check if a given type has a reserve function like std::vector's.
	 *	@tparam T The type to check for a callable T::reserve(size_type) member function.
	 */
	template <typename T, typename IsVoid = void> struct has_reserve : std::false_type {};
	template <typename T> struct has_reserve<T,
		std::void_t<
			decltype(
				std::declval<T>().reserve(0u)
			)
		>
	> : std::true_type {};
	/**	True if a given type has a reserve function like std::vector's. False otherwise.
	 *	@tparam T The type to check for a callable T::reserve(size_type) member function.
	 */
	template <typename T> constexpr bool has_reserve_v = has_reserve<T>::value;

	/**	Like std::adjacent_find but accepts a less-than predicate in the place of an is-equal predicate and expects the range to be sorted.
	 *	@param first The first iterator in a sorted range [first, last).
	 *	@param last The last iterator in a sorted range [first, last).
	 *	@param less A less-than predicate.
	 *	@return The resulting iterator in a range [result, last) that should be erased to effect uniqueness.
	 */
	template <typename Iterator, typename Compare>
	Iterator less_adjacent_find(Iterator first, const Iterator last, Compare&& less)
	{
		if (first != last)
		{
			using std::next;
			for (Iterator ahead = next(first); ahead != last; ++first, ++ahead)
			{
				// Always order (first, ahead) such that the iterators are (earlier, later).
				// As that's the expected order, less will return false only for equal values.
				if (false == less(*first, *ahead))
				{
					return first;
				}
			}
		}
		return last;
	}

	/**	Like std::unique but accepts a less-than predicate in the place of an is-equal predicate and expects the range to be sorted.
	 *	@param first The first iterator in a sorted range [first, last).
	 *	@param last The last iterator in a sorted range [first, last).
	 *	@param less A less-than predicate.
	 *	@return The resulting iterator in a range [result, last) that should be erased to effect uniqueness.
	 */
	template <typename Iterator, typename Compare>
	Iterator less_unique(Iterator first, const Iterator last, Compare&& less)
	{
		if (first != last)
		{
			Iterator result = first;
			while (++first != last)
			{
				// Always order (result, first) such that the iterators are (earlier, later).
				// As that's the expected order, less will return false only for equal values.
				if (less(*result, *first) && ++result != first)
				{
					*result = std::move(*first);
				}
			}
			return ++result;
		}
		return last;
	}

	template <typename T1, typename T2>
	class reference_pair final : public std::pair<std::add_lvalue_reference_t<T1>, std::add_lvalue_reference_t<T2>>
	{
	public:
		using pair_type = std::pair<std::add_lvalue_reference_t<T1>, std::add_lvalue_reference_t<T2>>;

		template <typename... Args>
		explicit reference_pair(Args&... args)
			: pair_type{ args... }
		{ }

		template <typename U,
			typename IsPairLike = std::enable_if_t<std::tuple_size_v<pair_type> == std::tuple_size_v<std::decay_t<U>>>
		>
		reference_pair& operator=(U&& other) noexcept
		{
			using std::get;
			get<0>(*this) = get<0>(std::forward<U>(other));
			get<1>(*this) = get<1>(std::forward<U>(other));
			return *this;
		}
		template <typename U1, typename U2>
		void swap(reference_pair<U1, U2>& other)
		{
			using std::get;
			using std::swap;
			swap(get<0>(*this), get<0>(other));
			swap(get<1>(*this), get<1>(other));
		}
		template <typename U1, typename U2>
		void swap(reference_pair<U1, U2>&& other)
		{
			this->swap(static_cast<reference_pair<U1, U2>&>(other));
		}

		friend void swap(reference_pair& lhs, reference_pair& rhs)
		{
			lhs.swap(rhs);
		}
		template <typename U1, typename U2>
		friend void swap(reference_pair& lhs, reference_pair<U1, U2>& rhs)
		{
			lhs.swap(rhs);
		}
		template <typename U1, typename U2>
		friend void swap(reference_pair&& lhs, reference_pair<U1, U2>& rhs)
		{
			std::move(lhs).swap(rhs);
		}
		template <typename U1, typename U2>
		friend void swap(reference_pair& lhs, reference_pair<U1, U2>&& rhs)
		{
			lhs.swap(std::move(rhs));
		}
		template <typename U1, typename U2>
		friend void swap(reference_pair&& lhs, reference_pair<U1, U2>&& rhs)
		{
			std::move(lhs).swap(std::move(rhs));
		}
	};

	template <typename T1, typename T2>
	reference_pair(T1&, T2&) -> reference_pair<T1, T2>;

	template <typename I1, typename I2>
	class iterator_pair final
	{
	public:
		using iterator_pair_type = std::pair<I1, I2>;
		using difference_type = std::ptrdiff_t;
		using value_type = std::pair<
			typename std::iterator_traits<I1>::value_type,
			typename std::iterator_traits<I2>::value_type>;
		using reference = reference_pair<
			typename std::iterator_traits<I1>::reference,
			typename std::iterator_traits<I2>::reference>;
		struct pointer final
		{
			reference m_reference;
			reference* operator->() noexcept
			{
				return std::addressof(m_reference);
			}
		};
		using iterator_category = typename std::iterator_traits<I1>::iterator_category;
		static_assert(
			std::is_same_v<iterator_category, typename std::iterator_traits<I2>::iterator_category>
			, "All iterator types expected to have the same iterator_category."
		);

		iterator_pair() = default;
		iterator_pair(const iterator_pair& other) = default;
		iterator_pair(iterator_pair&& other) = default;
		template <typename U1, typename U2>
		iterator_pair(const iterator_pair<U1, U2>& other)
			: m_iterators{ other.m_iterators }
		{ }
		template <typename... Args,
			typename IsConstructible = std::enable_if_t<std::is_constructible_v<iterator_pair_type, Args...>>
		>
		explicit iterator_pair(Args&&... args)
			: m_iterators{ std::forward<Args>(args)... }
		{ }
		iterator_pair& operator=(const iterator_pair&) = default;
		iterator_pair& operator=(iterator_pair&&) = default;

		pointer operator->() const
		{
			return pointer{ operator*() };
		}
		reference operator*() const
		{
			using std::get;
			return reference{ *get<0>(m_iterators), *get<1>(m_iterators) };
		}
		reference operator[](const difference_type offset) const
		{
			using std::get;
			return reference{ get<0>(m_iterators)[offset], get<1>(m_iterators)[offset] };
		}
		iterator_pair& operator++()
		{
			using std::get;
			++get<0>(m_iterators);
			++get<1>(m_iterators);
			return *this;
		}
		iterator_pair& operator--()
		{
			using std::get;
			--get<0>(m_iterators);
			--get<1>(m_iterators);
			return *this;
		}
		iterator_pair operator++(int)
		{
			iterator_pair result{ *this };
			this->operator++();
			return result;
		}
		iterator_pair operator--(int)
		{
			iterator_pair result{ *this };
			this->operator--();
			return result;
		}
		iterator_pair& operator+=(const difference_type offset)
		{
			using std::get;
			get<0>(m_iterators) += offset;
			get<1>(m_iterators) += offset;
			return *this;
		}
		iterator_pair& operator-=(const difference_type offset)
		{
			using std::get;
			get<0>(m_iterators) -= offset;
			get<1>(m_iterators) -= offset;
			return *this;
		}
		iterator_pair operator+(const difference_type offset) const
		{
			return iterator_pair{ *this } += offset;
		}
		iterator_pair operator-(const difference_type offset) const
		{
			return iterator_pair{ *this } -= offset;
		}

		friend difference_type operator-(const iterator_pair& lhs, const iterator_pair& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) - get<0>(rhs.m_iterators);
		}
		friend bool operator==(const iterator_pair& lhs, const iterator_pair& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) == get<0>(rhs.m_iterators);
		}
		friend bool operator!=(const iterator_pair& lhs, const iterator_pair& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) != get<0>(rhs.m_iterators);
		}
		friend bool operator<(const iterator_pair& lhs, const iterator_pair& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) < get<0>(rhs.m_iterators);
		}
		friend bool operator>(const iterator_pair& lhs, const iterator_pair& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) > get<0>(rhs.m_iterators);
		}
		friend bool operator<=(const iterator_pair& lhs, const iterator_pair& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) <= get<0>(rhs.m_iterators);
		}
		friend bool operator>=(const iterator_pair& lhs, const iterator_pair& rhs)
		{
			using std::get;
			return get<0>(lhs.m_iterators) >= get<0>(rhs.m_iterators);
		}
		friend void swap(iterator_pair& lhs, iterator_pair& rhs)
		{
			using std::swap;
			swap(lhs.m_iterators, rhs.m_iterators);
		}
		template <std::size_t I, typename J1, typename J2>
		friend std::tuple_element_t<I, iterator_pair<J1, J2>>& get(iterator_pair<J1, J2>&);
		template <std::size_t I, typename J1, typename J2>
		friend std::tuple_element_t<I, iterator_pair<J1, J2>>&& get(iterator_pair<J1, J2>&&);
		template <std::size_t I, typename J1, typename J2>
		friend const std::tuple_element_t<I, iterator_pair<J1, J2>>& get(const iterator_pair<J1, J2>&);

	private:
		template <typename J1, typename J2>
		friend class iterator_pair;

		iterator_pair_type m_iterators;
	};

	template <typename I1, typename I2>
	iterator_pair(iterator_pair<I1, I2>) -> iterator_pair<I1, I2>;

	template <typename I1, typename I2>
	iterator_pair(I1, I2) -> iterator_pair<I1, I2>;

	template <std::size_t I, typename I1, typename I2>
	std::tuple_element_t<I, iterator_pair<I1, I2>>& get(iterator_pair<I1, I2>& iter)
	{
		using std::get;
		return get<I>(iter.m_iterators);
	}
	template <std::size_t I, typename I1, typename I2>
	std::tuple_element_t<I, iterator_pair<I1, I2>>&& get(iterator_pair<I1, I2>&& iter)
	{
		using std::get;
		return get<I>(std::move(iter.m_iterators));
	}
	template <std::size_t I, typename I1, typename I2>
	const std::tuple_element_t<I, iterator_pair<I1, I2>>& get(const iterator_pair<I1, I2>& iter)
	{
		using std::get;
		return get<I>(iter.m_iterators);
	}

	template <typename I,
		typename V = typename std::iterator_traits<I>::value_type,
		typename R = typename std::iterator_traits<I>::reference
	>
	class iterator_wrapper final
	{
	public:
		using iterator_type = I;
		using difference_type = std::ptrdiff_t;
		using value_type = V;
		using reference = R;
		struct pointer final
		{
			reference m_reference;
			reference* operator->() noexcept
			{
				return std::addressof(m_reference);
			}
		};
		using iterator_category = typename std::iterator_traits<iterator_type>::iterator_category;

		iterator_wrapper() = default;
		iterator_wrapper(const iterator_wrapper& other) = default;
		iterator_wrapper(iterator_wrapper&& other) = default;
		template <typename I2, typename V2, typename R2>
		iterator_wrapper(const iterator_wrapper<I2, V2, R2>& other)
			: m_iterator{ other.m_iterator }
		{ }
		template <typename... Args,
			typename IsConstructible = std::enable_if_t<std::is_constructible_v<iterator_type, Args...>>
		>
		explicit iterator_wrapper(Args&&... args)
			: m_iterator{ std::forward<Args>(args)... }
		{ }
		iterator_wrapper& operator=(const iterator_wrapper&) = default;
		iterator_wrapper& operator=(iterator_wrapper&&) = default;

		pointer operator->() const
		{
			return pointer{ operator*() };
		}
		reference operator*() const
		{
			using std::get;
			return reference{ get<0>(*m_iterator), get<1>(*m_iterator) };
		}
		reference operator[](const difference_type offset) const
		{
			using std::get;
			return reference{ get<0>(m_iterator[offset]), get<1>(m_iterator[offset]) };
		}
		iterator_wrapper& operator++()
		{
			++m_iterator;
			return *this;
		}
		iterator_wrapper& operator--()
		{
			--m_iterator;
			return *this;
		}
		iterator_wrapper operator++(int)
		{
			iterator_wrapper result{ *this };
			this->operator++();
			return result;
		}
		iterator_wrapper operator--(int)
		{
			iterator_wrapper result{ *this };
			this->operator--();
			return result;
		}
		iterator_wrapper& operator+=(const difference_type offset)
		{
			m_iterator += offset;
			return *this;
		}
		iterator_wrapper& operator-=(const difference_type offset)
		{
			m_iterator -= offset;
			return *this;
		}
		iterator_wrapper operator+(const difference_type offset) const
		{
			return iterator_wrapper{ *this } += offset;
		}
		iterator_wrapper operator-(const difference_type offset) const
		{
			return iterator_wrapper{ *this } -= offset;
		}

		friend difference_type operator-(const iterator_wrapper& lhs, const iterator_wrapper& rhs)
		{
			return lhs.m_iterator - rhs.m_iterator;
		}
		friend bool operator==(const iterator_wrapper& lhs, const iterator_wrapper& rhs)
		{
			return lhs.m_iterator == rhs.m_iterator;
		}
		friend bool operator!=(const iterator_wrapper& lhs, const iterator_wrapper& rhs)
		{
			return lhs.m_iterator != rhs.m_iterator;
		}
		friend bool operator<(const iterator_wrapper& lhs, const iterator_wrapper& rhs)
		{
			return lhs.m_iterator < rhs.m_iterator;
		}
		friend bool operator>(const iterator_wrapper& lhs, const iterator_wrapper& rhs)
		{
			return lhs.m_iterator > rhs.m_iterator;
		}
		friend bool operator<=(const iterator_wrapper& lhs, const iterator_wrapper& rhs)
		{
			return lhs.m_iterator <= rhs.m_iterator;
		}
		friend bool operator>=(const iterator_wrapper& lhs, const iterator_wrapper& rhs)
		{
			return lhs.m_iterator >= rhs.m_iterator;
		}
		friend void swap(iterator_wrapper& lhs, iterator_wrapper& rhs)
		{
			using std::swap;
			swap(lhs.m_iterator, rhs.m_iterator);
		}

		constexpr const iterator_type& get() const
		{
			return m_iterator;
		}

	private:
		template <typename I2, typename V2, typename R2>
		friend class iterator_wrapper;

		iterator_type m_iterator;
	};

} // sh::flat

namespace std
{
	template <std::size_t I, typename J1, typename J2>
	struct tuple_element<I, sh::flat::reference_pair<J1, J2>>
		: tuple_element<I, typename sh::flat::reference_pair<J1, J2>::pair_type>
	{ };
	template <std::size_t I, typename J1, typename J2>
	struct tuple_element<I, sh::flat::iterator_pair<J1, J2>>
		: tuple_element<I, typename sh::flat::iterator_pair<J1, J2>::iterator_pair_type>
	{ };

	template <typename J1, typename J2>
	struct tuple_size<sh::flat::reference_pair<J1, J2>>
		: tuple_size<typename sh::flat::reference_pair<J1, J2>::pair_type>
	{ };
	template <typename J1, typename J2>
	struct tuple_size<sh::flat::iterator_pair<J1, J2>>
		: tuple_size<typename sh::flat::iterator_pair<J1, J2>::iterator_pair_type>
	{ };
} // namespace std

#endif
