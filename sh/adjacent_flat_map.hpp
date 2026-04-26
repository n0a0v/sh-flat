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

#include "flat_algorithm.hpp"

#include <algorithm>
#include <initializer_list>
#include <iterator>
#include <memory>
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
	using value_type = typename std::iterator_traits<flat::iterator_t<container_type>>::value_type;
	using key_compare = Compare;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using iterator = flat::iterator_wrapper<flat::iterator_t<container_type>,
		std::pair<key_type, mapped_type>,
		std::pair<const key_type&, mapped_type&>>;
	using const_iterator = flat::iterator_wrapper<flat::const_iterator_t<container_type>,
		std::pair<key_type, mapped_type>,
		std::pair<const key_type&, const mapped_type&>>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	static_assert(std::is_same_v<Key, std::tuple_element_t<0, value_type>>);
	static_assert(std::is_same_v<T, std::tuple_element_t<1, value_type>>);
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
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		const adjacent_flat_map& other,
		const Allocator& alloc);
	adjacent_flat_map(adjacent_flat_map&& other) noexcept;
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		adjacent_flat_map&& other,
		const Allocator& alloc);
	adjacent_flat_map(
		container_type cont,
		const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		const container_type& cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		const container_type& cont,
		const key_compare& comp,
		const Allocator& alloc);
	adjacent_flat_map(
		sorted_unique_t,
		const container_type& cont);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		sorted_unique_t,
		const container_type& cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		sorted_unique_t,
		const container_type& cont,
		const key_compare& comp,
		const Allocator& alloc);
	explicit adjacent_flat_map(const key_compare& comp);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(const key_compare& comp, const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
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
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		InputIterator first,
		InputIterator last,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
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
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		sorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
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
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		std::initializer_list<value_type> init,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	adjacent_flat_map(sorted_unique_t,
		std::initializer_list<value_type> init,
		const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	adjacent_flat_map(
		sorted_unique_t,
		std::initializer_list<value_type> init,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
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
	template <typename... Args,
		typename IsConstructible = std::enable_if_t<std::is_constructible_v<value_type, Args...>>
	>
	std::pair<iterator, bool> emplace(Args&&... args);
	template <typename... Args,
		typename IsConstructible = std::enable_if_t<std::is_constructible_v<value_type, Args...>>
	>
	iterator emplace_hint(const_iterator hint, Args&&... args);
	template <typename K, typename... Args,
		typename C = key_compare,
		typename IsTransparentAndNotIterator = std::enable_if_t<
			false == std::is_convertible_v<K&&, const_iterator> && false == std::is_convertible_v<K&&, iterator>,
			typename C::is_transparent
		>
	>
	std::pair<iterator, bool> try_emplace(K&& key_arg, Args&&... args);
	template <typename K, typename... Args,
		typename C = key_compare,
		typename IsTransparentAndNotIterator = std::enable_if_t<
			false == std::is_convertible_v<K&&, const_iterator> && false == std::is_convertible_v<K&&, iterator>,
			typename C::is_transparent
		>
	>
	iterator try_emplace(const_iterator hint, K&& key_arg, Args&&... args);
	template <typename K, typename M, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	std::pair<iterator, bool> insert_or_assign(K&& key_arg, M&& mapped_arg);
	template <typename K, typename M, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	iterator insert_or_assign(const_iterator hint, K&& key_arg, M&& mapped_arg);
	template <typename K,
		typename C = key_compare,
		typename IsTransparentAndNotIterator = std::enable_if_t<
			false == std::is_convertible_v<K&&, const_iterator> && false == std::is_convertible_v<K&&, iterator>,
			typename C::is_transparent
		>
	>
	size_type erase(const K& key_arg);

	// Modifiers (extensions):
	template <
		typename C = container_type,
		typename HasReserve = std::enable_if_t<flat::has_reserve_v<C>>
	>
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
	template <typename... Args>
	std::pair<iterator, bool> do_emplace_if_unique(const key_type& key_arg, Args&&... args);
	template <typename... Args>
	std::pair<iterator, bool> do_emplace_if_unique(key_type&& key_arg, Args&&... args);
	template <typename K, typename... Args>
	std::pair<iterator, bool> do_transparent_emplace_if_unique(K&& key_arg, Args&&... args);
	template <typename K, typename... Args>
	iterator do_transparent_emplace_hint_if_unique(const_iterator hint, K&& key_arg, Args&&... args);
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
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(adjacent_flat_map&& other) noexcept
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
	this->sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const container_type& cont, const Allocator& alloc)
	: key_compare{}
	, m_key_value_pairs{ cont, alloc }
{
	// Sort & unique as containers simply copied in above.
	this->sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename Allocator, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const container_type& cont, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_key_value_pairs{ cont, alloc }
{
	// Sort & unique as containers simply copied in above.
	this->sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename Container>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(sorted_unique_t, const container_type& cont)
	: key_compare{}
	, m_key_value_pairs{ cont }
{
	using std::begin;
	using std::end;
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()) == end(m_key_value_pairs),
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
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()) == end(m_key_value_pairs),
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
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()) == end(m_key_value_pairs),
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
	this->do_insert_back_without_sorting(first, last);
	this->sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const InputIterator first, const InputIterator last, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_key_value_pairs{ alloc }
{
	this->do_insert_back_without_sorting(first, last);
	this->sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
adjacent_flat_map<Key, T, Compare, Container>::adjacent_flat_map(const InputIterator first, const InputIterator last, const Allocator& alloc)
	: key_compare{}
	, m_key_value_pairs{ alloc }
{
	this->do_insert_back_without_sorting(first, last);
	this->sort_containers_and_erase_duplicates();
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
	this->do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()) == end(m_key_value_pairs),
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
	this->do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()) == end(m_key_value_pairs),
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
	this->do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()) == end(m_key_value_pairs),
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
	this->do_insert_back_without_sorting(begin(init), end(init));
	this->sort_containers_and_erase_duplicates();
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()) == end(m_key_value_pairs),
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
	this->do_insert_back_without_sorting(begin(init), end(init));
	this->sort_containers_and_erase_duplicates();
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()) == end(m_key_value_pairs),
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
	this->do_insert_back_without_sorting(begin(init), end(init));
	this->sort_containers_and_erase_duplicates();
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()) == end(m_key_value_pairs),
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
	this->do_insert_back_without_sorting(begin(init), end(init));
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()) == end(m_key_value_pairs),
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
	this->do_insert_back_without_sorting(begin(init), end(init));
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()) == end(m_key_value_pairs),
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
	this->do_insert_back_without_sorting(begin(init), end(init));
	SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()) == end(m_key_value_pairs),
		"Keys tagged with sorted_unique_t must already be unique.");
}

template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::operator=(const adjacent_flat_map& other)
	-> adjacent_flat_map&
{
	this->get_less() = other.get_less();
	m_key_value_pairs = other.m_key_value_pairs;
	return *this;
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::operator=(adjacent_flat_map&& other) noexcept
	-> adjacent_flat_map&
{
	this->get_less() = std::move(other.get_less());
	m_key_value_pairs = std::move(other.m_key_value_pairs);
	return *this;
}

// Element access:
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::at(const key_type& key_arg)
	-> mapped_type&
{
	using coopt_transparent = void;
	return this->at<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::at(const key_type& key_arg) const
	-> const mapped_type&
{
	using coopt_transparent = void;
	return this->at<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::operator[](const key_type& key_arg)
	-> mapped_type&
{
	using coopt_transparent = void;
	return this->operator[]<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::operator[](key_type&& key_arg)
	-> mapped_type&
{
	using coopt_transparent = void;
	return this->operator[]<key_type&&, key_compare, coopt_transparent>(std::move(key_arg));
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
	const flat::iterator_t<container_type> iter = this->do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs));
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
	const iterator iter = this->find(key_arg);
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
	iterator iter = this->lower_bound(key_arg);
	if (iter.get() == end(m_key_value_pairs) || this->get_less()(key_arg, get<0>(*iter)))
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
	using std::cbegin;
	return const_iterator{ cbegin(m_key_value_pairs) };
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::end() const noexcept
	-> const_iterator
{
	using std::cend;
	return const_iterator{ cend(m_key_value_pairs) };
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::cbegin() const noexcept
	-> const_iterator
{
	using std::cbegin;
	return const_iterator{ cbegin(m_key_value_pairs) };
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::cend() const noexcept
	-> const_iterator
{
	using std::cend;
	return const_iterator{ cend(m_key_value_pairs) };
}

template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::rbegin() noexcept
	-> reverse_iterator
{
	using std::rbegin;
	return rbegin(m_key_value_pairs);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::rend() noexcept
	-> reverse_iterator
{
	using std::rend;
	return rend(m_key_value_pairs);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::rbegin() const noexcept
	-> const_reverse_iterator
{
	using std::crbegin;
	return crbegin(m_key_value_pairs);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::rend() const noexcept
	-> const_reverse_iterator
{
	using std::crend;
	return crend(m_key_value_pairs);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::crbegin() const noexcept
	-> const_reverse_iterator
{
	using std::crbegin;
	return crbegin(m_key_value_pairs);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::crend() const noexcept
	-> const_reverse_iterator
{
	using std::crend;
	return crend(m_key_value_pairs);
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
	return this->do_transparent_emplace_if_unique<const key_type&>(key_arg, std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::try_emplace(key_type&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_if_unique<key_type&&>(std::move(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::try_emplace(const const_iterator hint, const key_type& key_arg, Args&&... args)
	-> iterator
{
	return this->do_transparent_emplace_if_unique<const key_type&>(key_arg, std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::try_emplace(const const_iterator hint, key_type&& key_arg, Args&&... args)
	-> iterator
{
	return this->do_transparent_emplace_if_unique<key_type&&>(std::move(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::insert(const value_type& value)
	-> std::pair<iterator, bool>
{
	return this->emplace(value);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::insert(value_type&& value)
	-> std::pair<iterator, bool>
{
	return this->emplace(std::move(value));
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::insert(const_iterator hint, const value_type& value)
	-> iterator
{
	return this->emplace_hint(hint, value);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::insert(const_iterator hint, value_type&& value)
	-> iterator
{
	return this->emplace_hint(hint, std::move(value));
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename M>
auto adjacent_flat_map<Key, T, Compare, Container>::insert_or_assign(const key_type& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	using coopt_transparent = void;
	return this->insert_or_assign<const key_type&, decltype(mapped_arg), key_compare, coopt_transparent>(key_arg, std::forward<M>(mapped_arg));
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename M>
auto adjacent_flat_map<Key, T, Compare, Container>::insert_or_assign(key_type&& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	using coopt_transparent = void;
	return this->insert_or_assign<key_type&&, decltype(mapped_arg), key_compare, coopt_transparent>(std::move(key_arg), std::forward<M>(mapped_arg));
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename M>
auto adjacent_flat_map<Key, T, Compare, Container>::insert_or_assign(const_iterator hint, const key_type& key_arg, M&& mapped_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->insert_or_assign<const key_type&, decltype(mapped_arg), key_compare, coopt_transparent>(key_arg, std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename M>
auto adjacent_flat_map<Key, T, Compare, Container>::insert_or_assign(const_iterator hint, key_type&& key_arg, M&& mapped_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->insert_or_assign<key_type&&, decltype(mapped_arg), key_compare, coopt_transparent>(std::move(key_arg), std::forward<M>(mapped_arg)).first;
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
	const size_type pre_insert_size{ m_key_value_pairs.size() };
	this->do_insert_back_without_sorting(first, last);
	// [first_iter, middle_iter) were sorted prior to calling do_insert_back_without_sorting.
	const flat::iterator_t<container_type> first_iter = begin(m_key_value_pairs);
	const flat::iterator_t<container_type> middle_iter = next(first_iter, pre_insert_size);
	// [middle_iter, last_iter) are the unsorted elements appended by do_insert_back_without_sorting.
	const flat::iterator_t<container_type> last_iter = end(m_key_value_pairs);
	// [middle_iter, last_iter) must be sorted (not stable!).
	const value_compare comp = this->value_comp();
	sort(middle_iter, last_iter, comp);
	// Merge the two sorted ranges together (stable sort).
	inplace_merge(first_iter, middle_iter, last_iter, comp);
	// Deduplicate elements post-merge.
	this->erase_sorted_duplicates(first_iter, last_iter, comp);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename InputIterator, typename HasIteratorCategory>
void adjacent_flat_map<Key, T, Compare, Container>::insert(const sorted_unique_t, InputIterator first, const InputIterator last)
{
	using std::begin;
	using std::end;
	const size_type pre_insert_size{ m_key_value_pairs.size() };
	this->do_insert_back_without_sorting(first, last);
	// No need to merge & unique as incoming elements are sorted & unique and
	// there were none previously.
	if (pre_insert_size != 0)
	{
		using std::next;
		using std::inplace_merge;
		// [first, middle) were sorted prior to calling do_insert_back_without_sorting.
		const flat::iterator_t<container_type> first_iter = begin(m_key_value_pairs);
		const flat::iterator_t<container_type> middle_iter = next(first_iter, pre_insert_size);
		// [middle, last) are the unsorted elements appended by do_insert_back_without_sorting.
		// [middle, last) is already sorted, per the sorted_unique_t tag.
		const flat::iterator_t<container_type> last_iter = end(m_key_value_pairs);
		// Merge the two sorted ranges together (stable sort).
		const value_compare comp = this->value_comp();
		inplace_merge(first_iter, middle_iter, last_iter, comp);
		// Deduplicate elements post-merge.
		this->erase_sorted_duplicates(first_iter, last_iter, comp);
	}
	else
	{
		SH_FLAT_ASSERT(std::is_sorted(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()),
			"Keys tagged with sorted_unique_t must already be sorted.");
		SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_key_value_pairs), end(m_key_value_pairs), this->value_comp()) == end(m_key_value_pairs),
			"Keys tagged with sorted_unique_t must already be unique.");
	}
}
template <typename Key, typename T, typename Compare, typename Container>
void adjacent_flat_map<Key, T, Compare, Container>::insert(std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	this->insert(begin(init), end(init));
}
template <typename Key, typename T, typename Compare, typename Container>
void adjacent_flat_map<Key, T, Compare, Container>::insert(const sorted_unique_t, std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	this->insert(sorted_unique, begin(init), end(init));
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
		this->clear();
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
	using coopt_transparent = void;
	return this->erase<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
void adjacent_flat_map<Key, T, Compare, Container>::swap(adjacent_flat_map& other)
	noexcept(std::is_nothrow_swappable_v<key_compare>)
{
	using std::swap;
	// key_compare is allowed to throw, so it goes first.
	swap(this->get_less(), other.get_less());
	swap(m_key_value_pairs, other.m_key_value_pairs);
}
template <typename Key, typename T, typename Compare, typename Container>
void adjacent_flat_map<Key, T, Compare, Container>::clear() noexcept
{
	m_key_value_pairs.clear();
}

// Modifiers (transparent):
template <typename Key, typename T, typename Compare, typename Container>
template <typename... Args, typename IsConstructible>
auto adjacent_flat_map<Key, T, Compare, Container>::emplace(Args&&... args)
	-> std::pair<iterator, bool>
{
	using std::get;
	// std::pair has the following constructors:
	//	1. Default: pair()
	//	2. Copy: pair(const pair&)
	//	3. Move: pair(pair&&)
	//	4. First & second copy: pair(const T1&, const T2&)
	//	5. First & second forward: pair(U1&&, U2&&)
	//	6. Template copy: pair(const pair<U1, U2>&)
	//	7. Template move: pair(pair<U1, U2>&&)
	//	8. Template copy rvalue: pair(const pair<U1, U2>&&)
	//	9. Piecewise: pair(std::piecewise_construct_t, std::tuple first, std::tuple second)
	if constexpr (sizeof...(args) == 0)
	{
		// Handle default construction (#1).
		return this->do_emplace_if_unique(key_type{});
	}
	else if constexpr (sizeof...(args) == 1)
	{
		if constexpr (std::is_convertible_v<std::add_pointer_t<Args&&>..., value_type*>
			&& std::is_rvalue_reference_v<Args&&...>)
		{
			// Handle forwarding an r-value pair that is or is like value_type (#3).
			value_type&& value = (static_cast<value_type&&>(args), ...);
			return this->do_emplace_if_unique(get<0>(std::move(value)), get<1>(std::move(value)));
		}
		else if constexpr (std::is_convertible_v<std::add_pointer_t<Args&&>..., const value_type*>)
		{
			// Handle an l-value pair that is or is like value_type (#2).
			const value_type& value = (static_cast<const value_type&>(args), ...);
			return this->do_emplace_if_unique(get<0>(value), get<1>(value));
		}
		else if (flat::has_conversion_operator_v<Args&&..., value_type&> || flat::has_conversion_operator_v<Args&&..., const value_type&>)
		{
			// Unwrap std::reference_wrapper and similar with conversion operators to value_type (none of the above).
			const value_type& value{ args... };
			return this->do_emplace_if_unique(get<0>(value), get<1>(value));
		}
		else
		{
			// Handle other construction such as copies from convertible but different types (#6, #7, #8).
			value_type value(std::forward<Args>(args)...);
			return this->do_emplace_if_unique(get<0>(std::move(value)), get<1>(std::move(value)));
		}
	}
	else if constexpr (sizeof...(args) == 2)
	{
		// Handle construction of first & second copy/move (#4, #5).
		return this->do_emplace_if_unique(std::forward<Args>(args)...);
	}
	else
	{
		// Handle other construction such as with piecewise_construct_t (#9).
		value_type value(std::forward<Args>(args)...);
		return this->do_emplace_if_unique(get<0>(std::move(value)), get<1>(std::move(value)));
	}
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename... Args, typename IsConstructible>
auto adjacent_flat_map<Key, T, Compare, Container>::emplace_hint(const const_iterator hint, Args&&... args)
	-> iterator
{
	using std::get;
	if constexpr (sizeof...(args) == 2)
	{
		// If possible, keep emplace a transparent operation.
		return this->do_transparent_emplace_hint_if_unique(hint, std::forward<Args>(args)...);
	}
	else
	{
		value_type value(std::forward<Args>(args)...);
		return this->do_transparent_emplace_hint_if_unique(hint, get<0>(std::move(value)), get<1>(std::move(value)));
	}
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename... Args, typename C, typename IsTransparentAndNotIterator>
auto adjacent_flat_map<Key, T, Compare, Container>::try_emplace(K&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename... Args, typename C, typename IsTransparentAndNotIterator>
auto adjacent_flat_map<Key, T, Compare, Container>::try_emplace(const const_iterator hint, K&& key_arg, Args&&... args)
	-> iterator
{
	return this->do_transparent_emplace_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename M, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::insert_or_assign(K&& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	const std::pair<iterator, bool> it_inserted = this->do_transparent_emplace_if_unique(std::forward<K>(key_arg), std::forward<M>(mapped_arg));
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
	using coopt_transparent = void;
	return this->insert_or_assign<K, M, key_compare, coopt_transparent>(std::forward<K>(key_arg), std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparentAndNotIterator>
auto adjacent_flat_map<Key, T, Compare, Container>::erase(const K& key_arg)
	-> size_type
{
	using std::get;
	using std::end;
	const iterator it = this->find(key_arg);
	if (it.get() == end(m_key_value_pairs))
	{
		return 0;
	}
	this->erase(it);
	return 1;
}

// Modifiers (extensions):
template <typename Key, typename T, typename Compare, typename Container>
template <typename C, typename HasReserve>
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
	using coopt_transparent = void;
	return this->find<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::find(const key_type& key_arg) const
	-> const_iterator
{
	using coopt_transparent = void;
	return this->find<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::count(const key_type& key_arg) const
	-> size_type
{
	using coopt_transparent = void;
	return this->count<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
bool adjacent_flat_map<Key, T, Compare, Container>::contains(const key_type& key_arg) const
{
	using coopt_transparent = void;
	return this->contains<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::lower_bound(const key_type& key_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->lower_bound<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::lower_bound(const key_type& key_arg) const
	-> const_iterator
{
	using coopt_transparent = void;
	return this->lower_bound<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::upper_bound(const key_type& key_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->upper_bound<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::upper_bound(const key_type& key_arg) const
	-> const_iterator
{
	using coopt_transparent = void;
	return this->upper_bound<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::equal_range(const key_type& key_arg)
	-> std::pair<iterator, iterator>
{
	using coopt_transparent = void;
	return this->equal_range<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::equal_range(const key_type& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	using coopt_transparent = void;
	return this->equal_range<const key_type&, key_compare, coopt_transparent>(key_arg);
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
	return iterator{ this->do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) };
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::find(const K& key_arg) const
	-> const_iterator
{
	using std::begin;
	using std::end;
	using std::next;
	return const_iterator{ this->do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) };
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::count(const K& key_arg) const
	-> size_type
{
	using std::begin;
	using std::end;
	return this->do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) != end(m_key_value_pairs) ? 1 : 0;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
bool adjacent_flat_map<Key, T, Compare, Container>::contains(const K& key_arg) const
{
	using std::begin;
	using std::end;
	return this->do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) != end(m_key_value_pairs);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::lower_bound(const K& key_arg)
	-> iterator
{
	using std::begin;
	using std::end;
	using std::next;
	return iterator{ this->do_lower_bound(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) };
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::lower_bound(const K& key_arg) const
	-> const_iterator
{
	using std::begin;
	using std::end;
	using std::next;
	return const_iterator{ this->do_lower_bound(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) };
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
	const key_compare& less = this->get_less();
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
	const key_compare& less = this->get_less();
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
	const iterator iter = this->find(key_arg);
	return { iter, iter.get() == end(m_key_value_pairs) ? iter : next(iter) };
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::equal_range(const K& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	using std::get;
	using std::next;
	using std::end;
	const const_iterator iter = this->find(key_arg);
	return { iter, iter == end(m_key_value_pairs) ? iter : next(iter) };
}

// Observers:
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::key_comp() const
	-> key_compare
{
	return this->get_less();
}
template <typename Key, typename T, typename Compare, typename Container>
auto adjacent_flat_map<Key, T, Compare, Container>::value_comp() const
	-> value_compare
{
	return value_compare{ this->get_less() };
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
	const Iterator key_iter = this->do_lower_bound(key_arg, first, last);
	return key_iter == last || this->get_less()(key_arg, get<0>(*key_iter)) ? last : key_iter;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename Iterator, typename C, typename IsTransparent>
auto adjacent_flat_map<Key, T, Compare, Container>::do_find(const K& key_arg, const Iterator first, const Iterator last) const
	-> Iterator
{
	using std::get;
	const Iterator key_iter = this->do_lower_bound(key_arg, first, last);
	return key_iter == last || this->get_less()(key_arg, get<0>(*key_iter)) ? last : key_iter;
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
	return lower_bound(first, last, key_arg, key_value_compare{ this->get_less() });
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
	return lower_bound(first, last, key_arg, key_value_compare{ this->get_less() });
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::do_emplace_if_unique(const key_type& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_if_unique(key_arg, std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::do_emplace_if_unique(key_type&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_if_unique(std::move(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::do_transparent_emplace_if_unique(K&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	using std::end;
	using std::get;
	iterator iter = this->lower_bound(key_arg);
	const bool emplaced = iter.get() == end(m_key_value_pairs) || this->get_less()(key_arg, get<0>(*iter));
	if (emplaced)
	{
		iter = iterator{ m_key_value_pairs.emplace(iter.get(), std::forward<K>(key_arg), mapped_type(std::forward<Args>(args)...)) };
	}
	return { iter, emplaced };
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename K, typename... Args>
auto adjacent_flat_map<Key, T, Compare, Container>::do_transparent_emplace_hint_if_unique(const const_iterator hint, K&& key_arg, Args&&... args)
	-> iterator
{
	using std::begin;
	using std::end;
	using std::get;
	using std::prev;
	if (m_key_value_pairs.empty() == false)
	{
		const key_compare& less = this->get_less();
		if (hint.get() == end(m_key_value_pairs) && less(get<0>(m_key_value_pairs.back()), key_arg))
		{
			// Hint is at the end and the back is less than the given key.
			return iterator{ m_key_value_pairs.emplace(hint.get(), std::forward<K>(key_arg), std::forward<Args>(args)...) };
		}
		else if ((hint.get() == begin(m_key_value_pairs) || less(get<0>(*prev(hint.get())), key_arg))
			&& less(key_arg, get<0>(*hint.get())))
		{
			// Hint is at the end and the back is less than the given key.
			return iterator{ m_key_value_pairs.emplace(hint.get(), std::forward<K>(key_arg), std::forward<Args>(args)...) };
		}
	}
	// Fallback to binary search emplacement.
	return this->do_transparent_emplace_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename InputIterator>
void adjacent_flat_map<Key, T, Compare, Container>::do_insert_back_without_sorting(const InputIterator first, const InputIterator last)
{
	using std::end;
	m_key_value_pairs.insert(end(m_key_value_pairs), first, last);
}
template <typename Key, typename T, typename Compare, typename Container>
template <typename Iterator>
void adjacent_flat_map<Key, T, Compare, Container>::erase_sorted_duplicates(const Iterator first, const Iterator last, const value_compare& comp)
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
	flat::iterator_t<container_type> first = begin(m_key_value_pairs);
	flat::iterator_t<container_type> last = end(m_key_value_pairs);
	const value_compare comp = this->value_comp();
	sort(first, last, comp);
	this->erase_sorted_duplicates(first, last, comp);
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
