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

#ifndef INC_SH__ADJACENT_FLAT_MAP_HPP
#define INC_SH__ADJACENT_FLAT_MAP_HPP

#include "flat.hpp"
#include "pair_algorithm.hpp"

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

namespace sh
{

template <typename Key,
	typename T,
	typename Compare = std::less<Key>,
	typename Container = std::vector<std::pair<Key, T>>>
class adjacent_flat_map : private Compare
{
public:
	using container_type = Container;
	using key_type = Key;
	using mapped_type = T;
	using value_type = typename container_type::value_type;
	using key_compare = Compare;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using iterator = pair_algorithm::iterator_wrapper<typename container_type::iterator,
		std::pair<key_type, mapped_type>,
		std::pair<const key_type&, mapped_type&>>;
	using const_iterator = pair_algorithm::iterator_wrapper<typename container_type::const_iterator,
		const std::pair<key_type, mapped_type>,
		std::pair<const key_type&, const mapped_type&>>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	static_assert(std::is_same_v<Key, std::tuple_element_t<0, typename container_type::value_type>>);
	static_assert(std::is_same_v<T, std::tuple_element_t<1, typename container_type::value_type>>);
	static_assert(std::is_nothrow_swappable_v<container_type>);

	struct value_compare : private key_compare
	{
		explicit value_compare(const key_compare& comp)
			: key_compare{ comp }
		{ }
		bool operator()(const_reference lhs, const_reference rhs) const
		{
			using std::get;
			return this->key_compare::operator()(get<0>(lhs), get<0>(rhs));
		}
	};

	// Member functions:
	adjacent_flat_map();
	adjacent_flat_map(const adjacent_flat_map& other);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		const adjacent_flat_map& other,
		const Allocator& alloc);
	adjacent_flat_map(adjacent_flat_map&& other);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		adjacent_flat_map&& other,
		const Allocator& alloc);
	adjacent_flat_map(
		container_type cont,
		const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		const container_type& cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		const container_type& cont,
		const key_compare& comp,
		const Allocator& alloc);
	adjacent_flat_map(
		sorted_unique_t,
		const container_type& cont);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		sorted_unique_t,
		const container_type& cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		sorted_unique_t,
		const container_type& cont,
		const key_compare& comp,
		const Allocator& alloc);
	explicit adjacent_flat_map(const key_compare& comp);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(const key_compare& comp, const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	explicit adjacent_flat_map(const Allocator& alloc);
	template <typename InputIterator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category
	>
	adjacent_flat_map(
		InputIterator first,
		InputIterator last,
		const key_compare& comp = key_compare{});
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		InputIterator first,
		InputIterator last,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		InputIterator first,
		InputIterator last,
		const Allocator& alloc);
	template <typename InputIterator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category
	>
	adjacent_flat_map(
		sorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_compare& comp = key_compare{});
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		sorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		sorted_unique_t,
		InputIterator first,
		InputIterator last,
		const Allocator& alloc);
	adjacent_flat_map(
		std::initializer_list<value_type> init,
		 const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		std::initializer_list<value_type> init,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	adjacent_flat_map(sorted_unique_t,
		std::initializer_list<value_type> init,
		const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		sorted_unique_t,
		std::initializer_list<value_type> init,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		sorted_unique_t,
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	~adjacent_flat_map() = default;

	adjacent_flat_map& operator=(const adjacent_flat_map& other);
	adjacent_flat_map& operator=(adjacent_flat_map&& other) noexcept;

	// Element access:
	mapped_type& at(const key_type& key_arg);
	const mapped_type& at(const key_type& key_arg) const;
	mapped_type& operator[](const key_type& key_arg);
	mapped_type& operator[](key_type&& key_arg);

	// Element access (transparent):
	template <typename K, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	mapped_type& at(const K& key_arg);
	template <typename K, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	const mapped_type& at(const K& key_arg) const;
	template <typename K, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	mapped_type& operator[](K&& key_arg);

	// Iterators:
	iterator begin() noexcept;
	iterator end() noexcept;
	const_iterator begin() const noexcept;
	const_iterator end() const noexcept;
	const_iterator cbegin() const noexcept;
	const_iterator cend() const noexcept;

	reverse_iterator rbegin() noexcept;
	reverse_iterator rend() noexcept;
	const_reverse_iterator rbegin() const noexcept;
	const_reverse_iterator rend() const noexcept;
	const_reverse_iterator crbegin() const noexcept;
	const_reverse_iterator crend() const noexcept;

	// Capacity:
	[[nodiscard]] bool empty() const noexcept;
	size_type size() const noexcept;
	size_type max_size() const noexcept;

	// Modifiers:
	template <typename... Args> std::pair<iterator, bool> try_emplace(const key_type& key_arg, Args&&... args);
	template <typename... Args> std::pair<iterator, bool> try_emplace(key_type&& key_arg, Args&&... args);
	template <typename... Args> iterator try_emplace(const_iterator hint, const key_type& key_arg, Args&&... args);
	template <typename... Args> iterator try_emplace(const_iterator hint, key_type&& key_arg, Args&&... args);
	std::pair<iterator, bool> insert(const value_type& value);
	std::pair<iterator, bool> insert(value_type&& value);
	iterator insert(const_iterator hint, const value_type& value);
	iterator insert(const_iterator hint, value_type&& value);
	template <typename M> std::pair<iterator, bool> insert_or_assign(const key_type& key_arg, M&& mapped_arg);
	template <typename M> std::pair<iterator, bool> insert_or_assign(key_type&& key_arg, M&& mapped_arg);
	template <typename M> iterator insert_or_assign(const_iterator hint, const key_type& key_arg, M&& mapped_arg);
	template <typename M> iterator insert_or_assign(const_iterator hint, key_type&& key_arg, M&& mapped_arg);
	template <typename InputIterator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category
	>
	void insert(InputIterator first, InputIterator last);
	template <typename InputIterator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category
	>
	void insert(sorted_unique_t, InputIterator first, InputIterator last);
	void insert(std::initializer_list<value_type> init);
	void insert(sorted_unique_t, std::initializer_list<value_type> init);
	container_type extract() &&;
	void replace(container_type&& cont);
	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);
	size_type erase(const key_type& key_arg);
	void swap(adjacent_flat_map& other) noexcept(std::is_nothrow_swappable_v<key_compare>);
	void clear() noexcept;

	// Modifiers (transparent):
	template <typename... Args> std::pair<iterator, bool> emplace(Args&&... args);
	template <typename... Args> iterator emplace_hint(const_iterator hint, Args&&... args);
	template <typename K, typename... Args,
		typename C = key_compare,
		typename IsTransparent = typename C::is_transparent,
		typename IsConvertible = std::enable_if_t<
			false == std::is_convertible_v<K&&, const_iterator>
			&& false == std::is_convertible_v<K&&, iterator>
		>
	>
	std::pair<iterator, bool> try_emplace(K&& key_arg, Args&&... args);
	template <typename K, typename... Args,
		typename C = key_compare,
		typename IsTransparent = typename C::is_transparent,
		typename IsConvertible = std::enable_if_t<
			false == std::is_convertible_v<K&&, const_iterator>
			&& false == std::is_convertible_v<K&&, iterator>
		>
	>
	iterator try_emplace(const_iterator hint, K&& key_arg, Args&&... args);
	template <typename K, typename M, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	std::pair<iterator, bool> insert_or_assign(K&& key_arg, M&& mapped_arg);
	template <typename K, typename M, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	iterator insert_or_assign(const_iterator hint, K&& key_arg, M&& mapped_arg);
	template <typename K,
		typename C = key_compare,
		typename IsTransparent = typename C::is_transparent,
		typename IsConvertible = std::enable_if_t<
			false == std::is_convertible_v<K&&, const_iterator>
			&& false == std::is_convertible_v<K&&, iterator>
		>
	>
	size_type erase(const K& key_arg);

	// Modifiers (extensions):
	void reserve(size_type n);
	void shrink_to_fit();

	// Lookup:
	iterator find(const key_type& key_arg);
	const_iterator find(const key_type& key_arg) const;
	size_type count(const key_type& key_arg) const;
	bool contains(const key_type& key_arg) const;
	iterator lower_bound(const key_type& key_arg);
	const_iterator lower_bound(const key_type& key_arg) const;
	iterator upper_bound(const key_type& key_arg);
	const_iterator upper_bound(const key_type& key_arg) const;
	std::pair<iterator, iterator> equal_range(const key_type& key_arg);
	std::pair<const_iterator, const_iterator> equal_range(const key_type& key_arg) const;

	// Lookup (transparent):
	template <typename K, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	iterator find(const K& key_arg);
	template <typename K, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	const_iterator find(const K& key_arg) const;
	template <typename K, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	size_type count(const K& key_arg) const;
	template <typename K, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	bool contains(const K& key_arg) const;
	template <typename K, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	iterator lower_bound(const K& key_arg);
	template <typename K, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	const_iterator lower_bound(const K& key_arg) const;
	template <typename K, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	iterator upper_bound(const K& key_arg);
	template <typename K, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	const_iterator upper_bound(const K& key_arg) const;
	template <typename K, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	std::pair<iterator, iterator> equal_range(const K& key_arg);
	template <typename K, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	std::pair<const_iterator, const_iterator> equal_range(const K& key_arg) const;

	// Observers:
	key_compare key_comp() const;
	value_compare value_comp() const;
	const container_type& container() const;

	// Non-member functions:
	template <typename Key2, typename T2, typename Compare2, typename Container2>
	friend bool operator==(const adjacent_flat_map<Key2, T2, Compare2, Container2>& lhs, const adjacent_flat_map<Key2, T2, Compare2, Container2>& rhs);
	template <typename Key2, typename T2, typename Compare2, typename Container2>
	friend bool operator!=(const adjacent_flat_map<Key2, T2, Compare2, Container2>& lhs, const adjacent_flat_map<Key2, T2, Compare2, Container2>& rhs);

private:
	template <typename Iterator>
	Iterator do_find(const key_type& key_arg, const Iterator first, const Iterator last) const;
	template <typename K, typename Iterator, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	Iterator do_find(const K& key_arg, const Iterator first, const Iterator last) const;
	template <typename Iterator>
	Iterator do_lower_bound(const key_type& key_arg, const Iterator first, const Iterator last) const;
	template <typename K, typename Iterator, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	Iterator do_lower_bound(const K& key_arg, const Iterator first, const Iterator last) const;
	template <typename K, typename... Args>
	std::pair<iterator, bool> do_transparent_emplace_if_unique(K&& key_arg, Args&&... args);
	template <typename InputIterator>
	void do_insert_back_without_sorting(InputIterator first, InputIterator last);
	template <typename Iterator>
	void erase_sorted_duplicates(Iterator first, Iterator last, const value_compare& comp);
	void sort_containers_and_erase_duplicates();

	constexpr key_compare& get_less() noexcept;
	constexpr const key_compare& get_less() const noexcept;

	container_type m_key_value_pairs;
};

// Member functions:
template <typename Key, typename T, typename Compare, typename Container>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map()
	: adjacent_flat_map{ key_compare{} }
{ }
template <typename Key, typename T, typename Compare, typename Container>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const adjacent_flat_map& other)
	: key_compare{ other.get_less() }
	, m_key_value_pairs{ other.m_key_value_pairs }
{ }
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const adjacent_flat_map& other, const Allocator& alloc)
	: key_compare{ other.get_less() }
	, m_key_value_pairs{ other.m_key_value_pairs, alloc }
{ }
template <typename Key, typename T, typename Compare, typename Container>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(adjacent_flat_map&& other)
	: key_compare{ std::move(other.get_less()) }
	, m_key_value_pairs{ std::move(other.m_key_value_pairs) }
{ }
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(adjacent_flat_map&& other, const Allocator& alloc)
	: key_compare{ std::move(other.get_less()) }
	, m_key_value_pairs{ std::move(other.m_key_value_pairs), alloc }
{ }
template <typename Key, typename T, typename Compare, typename Container>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(container_type cont, const key_compare& comp)
	: key_compare{ comp }
	, m_key_value_pairs{ std::move(cont) }
{
	// Sort & unique as containers simply copied in above.
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const container_type& cont, const Allocator& alloc)
	: key_compare{}
	, m_key_value_pairs{ cont, alloc }
{
	// Sort & unique as containers simply copied in above.
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const container_type& cont, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_key_value_pairs{ cont, alloc }
{
	// Sort & unique as containers simply copied in above.
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename Container>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(sorted_unique_t, const container_type& cont)
	: key_compare{}
	, m_key_value_pairs{ cont }
{
	using std::begin;
	using std::end;
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()) == end(m_key_value_pairs),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(sorted_unique_t, const container_type& cont, const Allocator& alloc)
	: key_compare{}
	, m_key_value_pairs{ cont, alloc }
{
	using std::begin;
	using std::end;
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()) == end(m_key_value_pairs),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(sorted_unique_t, const container_type& cont, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_key_value_pairs{ cont, alloc }
{
	using std::begin;
	using std::end;
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()) == end(m_key_value_pairs),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename Container>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const key_compare& comp)
	: key_compare{ comp }
	, m_key_value_pairs{ }
{ }
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_key_value_pairs{ alloc }
{ }
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const Allocator& alloc)
	: key_compare{}
	, m_key_value_pairs{ alloc }
{ }
template <typename Key, typename T, typename Compare, typename Container>
template <typename InputIterator, typename HasIteratorCategory>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const InputIterator first, const InputIterator last, const key_compare& comp)
	: key_compare{ comp }
	, m_key_value_pairs{}
{
	do_insert_back_without_sorting(first, last);
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const InputIterator first, const InputIterator last, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_key_value_pairs{ alloc }
{
	do_insert_back_without_sorting(first, last);
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const InputIterator first, const InputIterator last, const Allocator& alloc)
	: key_compare{}
	, m_key_value_pairs{ alloc }
{
	do_insert_back_without_sorting(first, last);
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename InputIterator, typename HasIteratorCategory>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(sorted_unique_t, const InputIterator first, const InputIterator last, const key_compare& comp)
	: key_compare{ comp }
	, m_key_value_pairs{}
{
	using std::begin;
	using std::end;
	// Add unsorted and assume unique because of sorted_unique tag.
	do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()) == end(m_key_value_pairs),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(sorted_unique_t, const InputIterator first, const InputIterator last, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_key_value_pairs{ alloc }
{
	using std::begin;
	using std::end;
	// Add unsorted and assume unique because of sorted_unique tag.
	do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()) == end(m_key_value_pairs),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(sorted_unique_t, const InputIterator first, const InputIterator last, const Allocator& alloc)
	: key_compare{}
	, m_key_value_pairs{ alloc }
{
	using std::begin;
	using std::end;
	// Add unsorted and assume unique because of sorted_unique tag.
	do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()) == end(m_key_value_pairs),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename Container>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(std::initializer_list<value_type> init, const key_compare& comp)
	: key_compare{ comp }
	, m_key_value_pairs{}
{
	using std::begin;
	using std::end;
	// Add unsorted and then sort & unique.
	do_insert_back_without_sorting(begin(init), end(init));
	sort_containers_and_erase_duplicates();
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()) == end(m_key_value_pairs),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(std::initializer_list<value_type> init, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_key_value_pairs{ alloc }
{
	using std::begin;
	using std::end;
	// Add unsorted and then sort & unique.
	do_insert_back_without_sorting(begin(init), end(init));
	sort_containers_and_erase_duplicates();
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()) == end(m_key_value_pairs),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(std::initializer_list<value_type> init, const Allocator& alloc)
	: key_compare{}
	, m_key_value_pairs{ alloc }
{
	using std::begin;
	using std::end;
	// Add unsorted and then sort & unique.
	do_insert_back_without_sorting(begin(init), end(init));
	sort_containers_and_erase_duplicates();
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()) == end(m_key_value_pairs),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename Container>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(sorted_unique_t, std::initializer_list<value_type> init, const key_compare& comp)
	: key_compare{ comp }
	, m_key_value_pairs{}
{
	using std::begin;
	using std::end;
	// Add unsorted and then sort & unique.
	do_insert_back_without_sorting(begin(init), end(init));
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()) == end(m_key_value_pairs),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(sorted_unique_t, std::initializer_list<value_type> init, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_key_value_pairs{ alloc }
{
	using std::begin;
	using std::end;
	do_insert_back_without_sorting(begin(init), end(init));
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()) == end(m_key_value_pairs),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(sorted_unique_t, std::initializer_list<value_type> init, const Allocator& alloc)
	: key_compare{}
	, m_key_value_pairs{ alloc }
{
	using std::begin;
	using std::end;
	do_insert_back_without_sorting(begin(init), end(init));
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), value_comp()) == end(m_key_value_pairs),
		"Keys tagged with sorted_unique_t must already be unique.");
}

template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::operator=(const adjacent_flat_map& other)
	-> adjacent_flat_map&
{
	get_less() = other.get_less();
	m_key_value_pairs = other.m_key_value_pairs;
	return *this;
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::operator=(adjacent_flat_map&& other) noexcept
	-> adjacent_flat_map&
{
	get_less() = std::move(other.get_less());
	m_key_value_pairs = std::move(other.m_key_value_pairs);
	return *this;
}

// Element access:
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::at(const key_type& key_arg)
	-> mapped_type&
{
	return this->at<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::at(const key_type& key_arg) const
	-> const mapped_type&
{
	return this->at<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::operator[](const key_type& key_arg)
	-> mapped_type&
{
	return this->operator[]<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::operator[](key_type&& key_arg)
	-> mapped_type&
{
	return this->operator[]<key_type&&, void, void>(std::move(key_arg));
}

// Element access (transparent):
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::at(const K& key_arg)
	-> mapped_type&
{
	using std::begin;
	using std::get;
	using std::end;
	const auto iter = do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs));
	if (iter == end(m_key_value_pairs))
	{
		throw std::out_of_range{ "adjacent_flat_map::at" };
	}
	return get<1>(*iter);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::at(const K& key_arg) const
	-> const mapped_type&
{
	using std::get;
	using std::end;
	const auto iter = find(key_arg);
	if (iter == end(m_key_value_pairs))
	{
		throw std::out_of_range{ "adjacent_flat_map::at" };
	}
	return get<1>(*iter);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::operator[](K&& key_arg)
	-> mapped_type&
{
	using std::get;
	using std::end;
	iterator iter = lower_bound(key_arg);
	if (iter.get() == end(m_key_value_pairs) || get_less()(key_arg, get<0>(*iter)))
	{
		iter = iterator{ m_key_value_pairs.emplace(iter.get(), std::forward<K>(key_arg), mapped_type{}) };
	}
	return get<1>(*iter);
}

// Iterators:
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::begin() noexcept
	-> iterator
{
	using std::begin;
	return iterator{ begin(m_key_value_pairs) };
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::end() noexcept
	-> iterator
{
	using std::end;
	return iterator{ end(m_key_value_pairs) };
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::begin() const noexcept
	-> const_iterator
{
	return const_iterator{ m_key_value_pairs.cbegin() };
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::end() const noexcept
	-> const_iterator
{
	return const_iterator{ m_key_value_pairs.cend() };
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::cbegin() const noexcept
	-> const_iterator
{
	return const_iterator{ m_key_value_pairs.cbegin() };
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::cend() const noexcept
	-> const_iterator
{
	return const_iterator{ m_key_value_pairs.cend() };
}

template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::rbegin() noexcept
	-> reverse_iterator
{
	return m_key_value_pairs.rbegin();
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::rend() noexcept
	-> reverse_iterator
{
	return m_key_value_pairs.rend();
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::rbegin() const noexcept
	-> const_reverse_iterator
{
	return m_key_value_pairs.crbegin();
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::rend() const noexcept
	-> const_reverse_iterator
{
	return m_key_value_pairs.crend();
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::crbegin() const noexcept
	-> const_reverse_iterator
{
	return m_key_value_pairs.crbegin();
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::crend() const noexcept
	-> const_reverse_iterator
{
	return m_key_value_pairs.crend();
}

// Capacity:
template <typename Key, typename T, typename Compare, typename Container>
bool adjacent_flat_map<Key, T, Compare, Container>::empty() const noexcept
{
	return m_key_value_pairs.empty();
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::size() const noexcept
	-> size_type
{
	return m_key_value_pairs.size();
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::max_size() const noexcept
	-> size_type
{
	return m_key_value_pairs.max_size();
}

// Modifiers:
template <typename Key, typename T, typename Compare, typename Container>
template <typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::try_emplace(const key_type& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return do_transparent_emplace_if_unique<const key_type&>(key_arg, std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::try_emplace(key_type&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return do_transparent_emplace_if_unique<key_type&&>(std::move(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::try_emplace(const const_iterator hint, const key_type& key_arg, Args&&... args)
	-> iterator
{
	return do_transparent_emplace_if_unique<const key_type&>(key_arg, std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::try_emplace(const const_iterator hint, key_type&& key_arg, Args&&... args)
	-> iterator
{
	return do_transparent_emplace_if_unique<key_type&&>(std::move(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::insert(const value_type& value)
	-> std::pair<iterator, bool>
{
	return emplace(value);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::insert(value_type&& value)
	-> std::pair<iterator, bool>
{
	return emplace(std::move(value));
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::insert(const_iterator hint, const value_type& value)
	-> iterator
{
	return emplace_hint(hint, value);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::insert(const_iterator hint, value_type&& value)
	-> iterator
{
	return emplace_hint(hint, std::move(value));
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename M>
auto adjacent_flat_map<Key, T, Compare, Container>::insert_or_assign(const key_type& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	return this->insert_or_assign<const key_type&, decltype(mapped_arg), void, void>(key_arg, std::forward<M>(mapped_arg));
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename M>
auto adjacent_flat_map<Key, T, Compare, Container>::insert_or_assign(key_type&& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	return this->insert_or_assign<key_type&&, decltype(mapped_arg), void, void>(std::move(key_arg), std::forward<M>(mapped_arg));
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename M>
auto adjacent_flat_map<Key, T, Compare, Container>::insert_or_assign(const_iterator hint, const key_type& key_arg, M&& mapped_arg)
	-> iterator
{
	return this->insert_or_assign<const key_type&, decltype(mapped_arg), void, void>(key_arg, std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename M>
auto adjacent_flat_map<Key, T, Compare, Container>::insert_or_assign(const_iterator hint, key_type&& key_arg, M&& mapped_arg)
	-> iterator
{
	return this->insert_or_assign<key_type&&, decltype(mapped_arg), void, void>(std::move(key_arg), std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename InputIterator, typename HasIteratorCategory>
void adjacent_flat_map<Key, T, Compare, Container>::insert(const InputIterator first, const InputIterator last)
{
	using std::begin;
	using std::end;
	using std::next;
	using std::sort;
	using std::inplace_merge;
	using std::unique;
	using std::get;
	const size_type pre_insert_size{ m_key_value_pairs.size() };
	do_insert_back_without_sorting(first, last);
	// [first_iter, middle_iter) were sorted prior to calling do_insert_back_without_sorting.
	const auto first_iter = begin(m_key_value_pairs);
	const auto middle_iter = next(first_iter, pre_insert_size);
	// [middle_iter, last_iter) are the unsorted elements appended by do_insert_back_without_sorting.
	const auto last_iter = end(m_key_value_pairs);
	// [middle_iter, last_iter) must be sorted (not stable!).
	const value_compare comp = value_comp();
	sort(middle_iter, last_iter, comp);
	// Merge the two sorted ranges together (stable sort).
	inplace_merge(first_iter, middle_iter, last_iter, comp);
	// Deduplicate elements post-merge.
	erase_sorted_duplicates(first_iter, last_iter, comp);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename InputIterator, typename HasIteratorCategory>
void adjacent_flat_map<Key, T, Compare, Container>::insert(const sorted_unique_t, InputIterator first, const InputIterator last)
{
	using std::begin;
	using std::end;
	using std::next;
	using std::inplace_merge;
	using std::unique;
	using std::get;
	const size_type pre_insert_size{ m_key_value_pairs.size() };
	do_insert_back_without_sorting(first, last);
	// [first, middle) were sorted prior to calling do_insert_back_without_sorting.
	const auto first_iter = begin(m_key_value_pairs);
	const auto middle_iter = next(first_iter, pre_insert_size);
	// [middle, last) are the unsorted elements appended by do_insert_back_without_sorting.
	// [middle, last) is already sorted, per the sorted_unique_t tag.
	const auto last_iter = end(m_key_value_pairs);
	// Merge the two sorted ranges together (stable sort).
	const value_compare comp = value_comp();
	inplace_merge(first_iter, middle_iter, last_iter, comp);
	// Deduplicate elements post-merge.
	erase_sorted_duplicates(first_iter, last_iter, comp);
}
template <typename Key, typename T, typename Compare, typename Container>
void adjacent_flat_map<Key, T, Compare, Container>::insert(std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	insert(begin(init), end(init));
}
template <typename Key, typename T, typename Compare, typename Container>
void adjacent_flat_map<Key, T, Compare, Container>::insert(const sorted_unique_t, std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	insert(sorted_unique, begin(init), end(init));
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::extract() &&
	-> container_type
{
	return std::move(m_key_value_pairs);
}
template <typename Key, typename T, typename Compare, typename Container>
void adjacent_flat_map<Key, T, Compare, Container>::replace(container_type&& cont)
{
	try
	{
		m_key_value_pairs = std::move(cont);
	}
	catch (...)
	{
		clear();
		throw;
	}
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::erase(const const_iterator pos)
	-> iterator
{
	using std::get;
	return iterator{ m_key_value_pairs.erase(pos.get()) };
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::erase(const const_iterator first, const const_iterator last)
	-> iterator
{
	using std::get;
	return iterator{ m_key_value_pairs.erase(first.get(), last.get()) };
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::erase(const key_type& key_arg)
	-> size_type
{
	return this->erase<const key_type&, void, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
void adjacent_flat_map<Key, T, Compare, Container>::swap(adjacent_flat_map& other)
	noexcept(std::is_nothrow_swappable_v<key_compare>)
{
	using std::swap;
	// key_compare is allowed to throw, so it goes first.
	swap(get_less(), other.get_less());
	swap(m_key_value_pairs, other.m_key_value_pairs);
}
template <typename Key, typename T, typename Compare, typename Container>
void adjacent_flat_map<Key, T, Compare, Container>::clear() noexcept
{
	m_key_value_pairs.clear();
}

// Modifiers (transparent):
template <typename Key, typename T, typename Compare, typename Container>
template <typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::emplace(Args&&... args)
	-> std::pair<iterator, bool>
{
	using std::get;
	if constexpr (sizeof...(args) == 2)
	{
		// If possible, keep emplace a transparent operation.
		return do_transparent_emplace_if_unique(std::forward<Args>(args)...);
	}
	else
	{
		value_type value{ std::forward<Args>(args)... };
		return do_transparent_emplace_if_unique(get<0>(std::move(value)), get<1>(std::move(value)));
	}
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::emplace_hint(const const_iterator hint, Args&&... args)
	-> iterator
{
	return emplace(std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename... Args, typename C, typename IsTransparent, typename IsConvertible>
auto adjacent_flat_map<Key, T, Compare, Container>::try_emplace(K&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return do_transparent_emplace_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename... Args, typename C, typename IsTransparent, typename IsConvertibl>
auto adjacent_flat_map<Key, T, Compare, Container>::try_emplace(const const_iterator hint, K&& key_arg, Args&&... args)
	-> iterator
{
	return do_transparent_emplace_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename M, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::insert_or_assign(K&& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
std::cout << __LINE__ << '\n';
	const std::pair<iterator, bool> it_inserted = do_transparent_emplace_if_unique(std::forward<K>(key_arg), std::forward<M>(mapped_arg));
	if (it_inserted.second == false)
	{
		using std::get;
		get<1>(*it_inserted.first) = std::forward<M>(mapped_arg);
	}
	return it_inserted;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename M, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::insert_or_assign(const_iterator hint, K&& key_arg, M&& mapped_arg)
	-> iterator
{
std::cout << __LINE__ << '\n';
	return this->insert_or_assign<K, M, void, void>(std::forward<K>(key_arg), std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent, typename IsConvertibl>
auto adjacent_flat_map<Key, T, Compare, Container>::erase(const K& key_arg)
	-> size_type
{
	using std::get;
	using std::end;
	const iterator it = find(key_arg);
	if (it.get() == end(m_key_value_pairs))
	{
		return 0;
	}
	erase(it);
	return 1;
}

// Modifiers (extensions):
template <typename Key, typename T, typename Compare, typename Container>
void adjacent_flat_map<Key, T, Compare, Container>::reserve(const size_type n)
{
	m_key_value_pairs.reserve(n);
}
template <typename Key, typename T, typename Compare, typename Container>
void adjacent_flat_map<Key, T, Compare, Container>::shrink_to_fit()
{
	m_key_value_pairs.shrink_to_fit();
}

// Lookup:
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::find(const key_type& key_arg)
	 -> iterator
{
	return this->find<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::find(const key_type& key_arg) const
	-> const_iterator
{
	return this->find<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::count(const key_type& key_arg) const
	-> size_type
{
	return this->count<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
bool adjacent_flat_map<Key, T, Compare, Container>::contains(const key_type& key_arg) const
{
	return this->count<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::lower_bound(const key_type& key_arg)
	-> iterator
{
	return this->lower_bound<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::lower_bound(const key_type& key_arg) const
	-> const_iterator
{
	return this->lower_bound<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::upper_bound(const key_type& key_arg)
	-> iterator
{
	return this->upper_bound<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::upper_bound(const key_type& key_arg) const
	-> const_iterator
{
	return this->upper_bound<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::equal_range(const key_type& key_arg)
	-> std::pair<iterator, iterator>
{
	return this->equal_range<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::equal_range(const key_type& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	return this->equal_range<const key_type&, void, void>(key_arg);
}

// Lookup (transparent):
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::find(const K& key_arg)
	 -> iterator
{
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	return iterator{ do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) };
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::find(const K& key_arg) const
	-> const_iterator
{
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	return const_iterator{ do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) };
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::count(const K& key_arg) const
	-> size_type
{
	using std::begin;
	using std::end;
	return do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) != end(m_key_value_pairs) ? 1 : 0;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
bool adjacent_flat_map<Key, T, Compare, Container>::contains(const K& key_arg) const
{
	using std::begin;
	using std::end;
	return do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) != end(m_key_value_pairs);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::lower_bound(const K& key_arg)
	-> iterator
{
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	return iterator{ do_lower_bound(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) };
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::lower_bound(const K& key_arg) const
	-> const_iterator
{
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	return const_iterator{ do_lower_bound(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) };
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::upper_bound(const K& key_arg)
	-> iterator
{
	using std::upper_bound;
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const auto& less = get_less();
	return iterator{ upper_bound(begin(m_key_value_pairs), end(m_key_value_pairs), key_arg, less) };
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::upper_bound(const K& key_arg) const
	-> const_iterator
{
	using std::upper_bound;
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const auto& less = get_less();
	return const_iterator{ upper_bound(begin(m_key_value_pairs), end(m_key_value_pairs), key_arg, less) };
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::equal_range(const K& key_arg)
	-> std::pair<iterator, iterator>
{
	using std::get;
	using std::next;
	using std::end;
	const iterator iter = find(key_arg);
	return std::make_pair(iter, iter.get() == end(m_key_value_pairs) ? iter : next(iter));
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::equal_range(const K& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	using std::get;
	using std::next;
	using std::end;
	const const_iterator iter = find(key_arg);
	return std::make_pair(iter, iter == end(m_key_value_pairs) ? iter : next(iter));
}

// Observers:
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::key_comp() const
	-> key_compare
{
	return get_less();
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::value_comp() const
	-> value_compare
{
	return value_compare{ get_less() };
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::container() const
	-> const container_type&
{
	return m_key_value_pairs;
}

// Non-member functions:
template <typename Key, typename T, typename Compare, typename Container>
bool operator==(const adjacent_flat_map<Key, T, Compare, Container>& lhs, const adjacent_flat_map<Key, T, Compare, Container>& rhs)
{
	return lhs.m_key_value_pairs == rhs.m_key_value_pairs;
}
template <typename Key, typename T, typename Compare, typename Container>
bool operator!=(const adjacent_flat_map<Key, T, Compare, Container>& lhs, const adjacent_flat_map<Key, T, Compare, Container>& rhs)
{
	return lhs.m_key_value_pairs != rhs.m_key_value_pairs;
}

template <typename Key, typename T, typename Compare, typename Container>
template <typename Iterator>
auto adjacent_flat_map<Key, T, Compare, Container>::do_find(const key_type& key_arg, const Iterator first, const Iterator last) const
	-> Iterator
{
	using std::get;
	const auto key_iter = do_lower_bound(key_arg, first, last);
	return key_iter == last || get_less()(key_arg, get<0>(*key_iter)) ? last : key_iter;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename Iterator, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::do_find(const K& key_arg, const Iterator first, const Iterator last) const
	-> Iterator
{
	using std::get;
	const auto key_iter = do_lower_bound(key_arg, first, last);
	return key_iter == last || get_less()(key_arg, get<0>(*key_iter)) ? last : key_iter;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename Iterator>
auto adjacent_flat_map<Key, T, Compare, Container>::do_lower_bound(const key_type& key_arg, const Iterator first, const Iterator last) const
	-> Iterator
{
	using std::lower_bound;
	struct key_value_compare final
	{
		constexpr bool operator()(const key_type& lhs, const_reference rhs) const
		{
			using std::get;
			return m_key_compare(lhs, get<0>(rhs));
		}
		constexpr bool operator()(const_reference lhs, const key_type& rhs) const
		{
			using std::get;
			return m_key_compare(get<0>(lhs), rhs);
		}
		const key_compare& m_key_compare;
	};
	return lower_bound(first, last, key_arg, key_value_compare{ *this });
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename Iterator, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::do_lower_bound(const K& key_arg, const Iterator first, const Iterator last) const
	-> Iterator
{
	using std::lower_bound;
	struct key_value_compare final
	{
		constexpr bool operator()(const K& lhs, const_reference rhs) const
		{
			using std::get;
			return m_key_compare(lhs, get<0>(rhs));
		}
		constexpr bool operator()(const_reference lhs, const K& rhs) const
		{
			using std::get;
			return m_key_compare(get<0>(lhs), rhs);
		}
		const key_compare& m_key_compare;
	};
	return lower_bound(first, last, key_arg, key_value_compare{ *this });
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::do_transparent_emplace_if_unique(K&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	using std::end;
	using std::get;
	auto iter = lower_bound(key_arg);
	const bool emplaced = iter.get() == end(m_key_value_pairs) || get_less()(key_arg, get<0>(*iter));
	if (emplaced)
	{
		iter = iterator{ m_key_value_pairs.emplace(iter.get(), std::forward<K>(key_arg), std::forward<Args>(args)...) };
	}
	return std::make_pair(iter, emplaced);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename InputIterator>
void adjacent_flat_map<Key, T, Compare, Container>::do_insert_back_without_sorting(const InputIterator first, const InputIterator last)
{
	m_key_value_pairs.insert(m_key_value_pairs.end(), first, last);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename Iterator>
void adjacent_flat_map<Key, T, Compare, Container>::erase_sorted_duplicates(Iterator first, const Iterator last, const value_compare& comp)
{
	const Iterator result = flat::less_unique(first, last, comp);
	m_key_value_pairs.erase(result, last);
}
template <typename Key, typename T, typename Compare, typename Container>
void adjacent_flat_map<Key, T, Compare, Container>::sort_containers_and_erase_duplicates()
{
	using std::begin;
	using std::end;
	using std::sort;
	using std::unique;
	auto first = begin(m_key_value_pairs);
	auto last = end(m_key_value_pairs);
	const value_compare comp = value_comp();
	sort(first, last, comp);
	erase_sorted_duplicates(first, last, comp);
}

template <typename Key, typename T, typename Compare, typename Container>
constexpr auto adjacent_flat_map<Key, T, Compare, Container>::get_less() noexcept
	-> key_compare&
{
	return *this;
}
template <typename Key, typename T, typename Compare, typename Container>
constexpr auto adjacent_flat_map<Key, T, Compare, Container>::get_less() const noexcept
	-> const key_compare&
{
	return *this;
}

} // namespace sh

#endif
