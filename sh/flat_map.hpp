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

#ifndef INC_SH__FLAT_MAP_HPP
#define INC_SH__FLAT_MAP_HPP

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
	typename KeyContainer = std::vector<Key>,
	typename MappedContainer = std::vector<T>>
class flat_map : private Compare
{
public:
	using key_container_type = KeyContainer;
	using mapped_container_type = MappedContainer;
	using key_type = Key;
	using mapped_type = T;
	using value_type = std::pair<key_type, mapped_type>;
	using key_compare = Compare;
	using reference = std::pair<const key_type&, mapped_type&>;
	using const_reference = std::pair<const key_type&, const mapped_type&>;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using iterator = flat::iterator_pair<flat::const_iterator_t<key_container_type>, flat::iterator_t<mapped_container_type>>;
	using const_iterator = flat::iterator_pair<flat::const_iterator_t<key_container_type>, flat::const_iterator_t<mapped_container_type>>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	static_assert(std::is_same_v<Key, typename std::iterator_traits<flat::iterator_t<key_container_type>>::value_type>);
	static_assert(std::is_same_v<T, typename std::iterator_traits<flat::iterator_t<mapped_container_type>>::value_type>);
	static_assert(std::is_nothrow_swappable_v<key_container_type>);
	static_assert(std::is_nothrow_swappable_v<mapped_container_type>);

	struct containers
	{
		key_container_type keys;
		mapped_container_type values;
	};
	struct value_compare : private key_compare
	{
		explicit value_compare(const key_compare& comp)
			: key_compare{ comp }
		{ }
		bool operator()(const const_reference& lhs, const const_reference& rhs) const
		{
			using std::get;
			return this->key_compare::operator()(get<0>(lhs), get<0>(rhs));
		}
	};

	// Member functions:
	flat_map();
	flat_map(const flat_map& other);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		const flat_map& other,
		const Allocator& alloc);
	flat_map(flat_map&& other) noexcept;
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		flat_map&& other,
		const Allocator& alloc);
	flat_map(
		key_container_type key_cont,
		mapped_container_type mapped_cont,
		const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		const key_container_type& key_cont,
		const mapped_container_type& mapped_cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		const key_container_type& key_cont,
		const mapped_container_type& mapped_cont,
		const key_compare& comp,
		const Allocator& alloc);
	flat_map(
		sorted_unique_t,
		const key_container_type& key_cont,
		const mapped_container_type& mapped_cont);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		sorted_unique_t,
		const key_container_type& key_cont,
		const mapped_container_type& mapped_cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		sorted_unique_t,
		const key_container_type& key_cont,
		const mapped_container_type& mapped_cont,
		const key_compare& comp,
		const Allocator& alloc);
	explicit flat_map(const key_compare& comp);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(const key_compare& comp, const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	explicit flat_map(const Allocator& alloc);
	template <typename InputIterator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category
	>
	flat_map(
		InputIterator first,
		InputIterator last,
		const key_compare& comp = key_compare{});
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		InputIterator first,
		InputIterator last,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		InputIterator first,
		InputIterator last,
		const Allocator& alloc);
	template <typename InputIterator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category
	>
	flat_map(
		sorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_compare& comp = key_compare{});
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		sorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		sorted_unique_t,
		InputIterator first,
		InputIterator last,
		const Allocator& alloc);
	flat_map(
		std::initializer_list<value_type> init,
		 const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		std::initializer_list<value_type> init,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	flat_map(sorted_unique_t,
		std::initializer_list<value_type> init,
		const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		sorted_unique_t,
		std::initializer_list<value_type> init,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		sorted_unique_t,
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	~flat_map() = default;

	flat_map& operator=(const flat_map& other);
	flat_map& operator=(flat_map&& other) noexcept;

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
	containers extract() &&;
	void replace(containers&& cont);
	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);
	size_type erase(const key_type& key_arg);
	void swap(flat_map& other) noexcept(std::is_nothrow_swappable_v<key_compare>);
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
		typename KC = key_container_type,
		typename MC = mapped_container_type,
		typename HasReserve = std::enable_if_t<flat::has_reserve_v<KC> || flat::has_reserve_v<MC>>
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
	const key_container_type& keys() const;
	const mapped_container_type& values() const;

	// Non-member functions:
	template <typename Key2, typename T2, typename Compare2, typename KeyContainer2, typename MappedContainer2>
	friend bool operator==(const flat_map<Key2, T2, Compare2, KeyContainer2, MappedContainer2>& lhs, const flat_map<Key2, T2, Compare2, KeyContainer2, MappedContainer2>& rhs);
	template <typename Key2, typename T2, typename Compare2, typename KeyContainer2, typename MappedContainer2>
	friend bool operator!=(const flat_map<Key2, T2, Compare2, KeyContainer2, MappedContainer2>& lhs, const flat_map<Key2, T2, Compare2, KeyContainer2, MappedContainer2>& rhs);

private:
	template <typename KeyIterator>
	KeyIterator do_find(const key_type& key_arg, const KeyIterator first, const KeyIterator last) const;
	template <typename K, typename KeyIterator, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	KeyIterator do_find(const K& key_arg, const KeyIterator first, const KeyIterator last) const;
	template <typename KeyIterator>
	KeyIterator do_lower_bound(const key_type& key_arg, const KeyIterator first, const KeyIterator last) const;
	template <typename K, typename KeyIterator, typename C = key_compare, typename IsTransparent = typename C::is_transparent>
	KeyIterator do_lower_bound(const K& key_arg, const KeyIterator first, const KeyIterator last) const;
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

	key_container_type m_keys;
	mapped_container_type m_values;
};

// Member functions:
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map()
	: flat_map{ key_compare{} }
{ }
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const flat_map& other)
	: key_compare{ other.get_less() }
	, m_keys{ other.m_keys }
	, m_values{ other.m_values }
{ }
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const flat_map& other, const Allocator& alloc)
	: key_compare{ other.get_less() }
	, m_keys{ other.m_keys, alloc }
	, m_values{ other.m_values, alloc }
{ }
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(flat_map&& other) noexcept
	: key_compare{ std::move(other.get_less()) }
	, m_keys{ std::move(other.m_keys) }
	, m_values{ std::move(other.m_values) }
{ }
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(flat_map&& other, const Allocator& alloc)
	: key_compare{ std::move(other.get_less()) }
	, m_keys{ std::move(other.m_keys), alloc }
	, m_values{ std::move(other.m_values), alloc }
{ }
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(key_container_type key_cont, mapped_container_type mapped_cont, const key_compare& comp)
	: key_compare{ comp }
	, m_keys{ std::move(key_cont) }
	, m_values{ std::move(mapped_cont) }
{
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	// Sort & unique as containers simply copied in above.
	this->sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const key_container_type& key_cont, const mapped_container_type& mapped_cont, const Allocator& alloc)
	: key_compare{}
	, m_keys{ key_cont, alloc }
	, m_values{ mapped_cont, alloc }
{
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	// Sort & unique as containers simply copied in above.
	this->sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const key_container_type& key_cont, const mapped_container_type& mapped_cont, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ key_cont, alloc }
	, m_values{ mapped_cont, alloc }
{
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	// Sort & unique as containers simply copied in above.
	this->sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, const key_container_type& key_cont, const mapped_container_type& mapped_cont)
	: key_compare{}
	, m_keys{ key_cont }
	, m_values{ mapped_cont }
{
	using std::begin;
	using std::end;
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), this->get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_keys), end(m_keys), this->get_less()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, const key_container_type& key_cont, const mapped_container_type& mapped_cont, const Allocator& alloc)
	: key_compare{}
	, m_keys{ key_cont, alloc }
	, m_values{ mapped_cont, alloc }
{
	using std::begin;
	using std::end;
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), this->get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_keys), end(m_keys), this->get_less()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, const key_container_type& key_cont, const mapped_container_type& mapped_cont, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ key_cont, alloc }
	, m_values{ mapped_cont, alloc }
{
	using std::begin;
	using std::end;
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), this->get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_keys), end(m_keys), this->get_less()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const key_compare& comp)
	: key_compare{ comp }
	, m_keys{ }
	, m_values{ }
{ }
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ alloc }
	, m_values{ alloc }
{ }
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const Allocator& alloc)
	: key_compare{}
	, m_keys{ alloc }
	, m_values{ alloc }
{ }
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename HasIteratorCategory>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const InputIterator first, const InputIterator last, const key_compare& comp)
	: key_compare{ comp }
	, m_keys{}
	, m_values{}
{
	this->do_insert_back_without_sorting(first, last);
	this->sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const InputIterator first, const InputIterator last, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ alloc }
	, m_values{ alloc }
{
	this->do_insert_back_without_sorting(first, last);
	this->sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const InputIterator first, const InputIterator last, const Allocator& alloc)
	: key_compare{}
	, m_keys{ alloc }
	, m_values{ alloc }
{
	this->do_insert_back_without_sorting(first, last);
	this->sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename HasIteratorCategory>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, const InputIterator first, const InputIterator last, const key_compare& comp)
	: key_compare{ comp }
	, m_keys{}
	, m_values{}
{
	using std::begin;
	using std::end;
	// Add unsorted and assume unique because of sorted_unique tag.
	this->do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), this->get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_keys), end(m_keys), this->get_less()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, const InputIterator first, const InputIterator last, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	// Add unsorted and assume unique because of sorted_unique tag.
	this->do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), this->get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_keys), end(m_keys), this->get_less()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, const InputIterator first, const InputIterator last, const Allocator& alloc)
	: key_compare{}
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	// Add unsorted and assume unique because of sorted_unique tag.
	this->do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), this->get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_keys), end(m_keys), this->get_less()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(std::initializer_list<value_type> init, const key_compare& comp)
	: key_compare{ comp }
	, m_keys{}
	, m_values{}
{
	using std::begin;
	using std::end;
	// Add unsorted and then sort & unique.
	this->do_insert_back_without_sorting(begin(init), end(init));
	this->sort_containers_and_erase_duplicates();
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), this->get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_keys), end(m_keys), this->get_less()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(std::initializer_list<value_type> init, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	// Add unsorted and then sort & unique.
	this->do_insert_back_without_sorting(begin(init), end(init));
	this->sort_containers_and_erase_duplicates();
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), this->get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_keys), end(m_keys), this->get_less()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(std::initializer_list<value_type> init, const Allocator& alloc)
	: key_compare{}
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	// Add unsorted and then sort & unique.
	this->do_insert_back_without_sorting(begin(init), end(init));
	sort_containers_and_erase_duplicates();
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), this->get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_keys), end(m_keys), this->get_less()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, std::initializer_list<value_type> init, const key_compare& comp)
	: key_compare{ comp }
	, m_keys{}
	, m_values{}
{
	using std::begin;
	using std::end;
	// Add unsorted and then sort & unique.
	this->do_insert_back_without_sorting(begin(init), end(init));
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), this->get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_keys), end(m_keys), this->get_less()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, std::initializer_list<value_type> init, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	this->do_insert_back_without_sorting(begin(init), end(init));
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), this->get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_keys), end(m_keys), this->get_less()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, std::initializer_list<value_type> init, const Allocator& alloc)
	: key_compare{}
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	this->do_insert_back_without_sorting(begin(init), end(init));
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), this->get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_keys), end(m_keys), this->get_less()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}

template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::operator=(const flat_map& other)
	-> flat_map&
{
	this->get_less() = other.get_less();
	m_keys = other.m_keys;
	m_values = other.m_values;
	return *this;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::operator=(flat_map&& other) noexcept
	-> flat_map&
{
	this->get_less() = std::move(other.get_less());
	m_keys = std::move(other.m_keys);
	m_values = std::move(other.m_values);
	return *this;
}

// Element access:
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::at(const key_type& key_arg)
	-> mapped_type&
{
	using coopt_transparent = void;
	return this->at<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::at(const key_type& key_arg) const
	-> const mapped_type&
{
	using coopt_transparent = void;
	return this->at<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::operator[](const key_type& key_arg)
	-> mapped_type&
{
	using coopt_transparent = void;
	return this->operator[]<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::operator[](key_type&& key_arg)
	-> mapped_type&
{
	using coopt_transparent = void;
	return this->operator[]<key_type&&, key_compare, coopt_transparent>(std::move(key_arg));
}

// Element access (transparent):
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::at(const K& key_arg)
	-> mapped_type&
{
	using std::get;
	using std::end;
	const iterator iter = this->find(key_arg);
	if (get<0>(iter) == end(m_keys))
	{
		throw std::out_of_range{ "flat_map::at" };
	}
	return *get<1>(iter);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::at(const K& key_arg) const
	-> const mapped_type&
{
	using std::get;
	using std::end;
	const const_iterator iter = this->find(key_arg);
	if (get<0>(iter) == end(m_keys))
	{
		throw std::out_of_range{ "flat_map::at" };
	}
	return *get<1>(iter);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::operator[](K&& key_arg)
	-> mapped_type&
{
	using std::get;
	return get<1>(*this->do_transparent_emplace_if_unique(std::forward<K>(key_arg)).first);
}

// Iterators:
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::begin() noexcept
	-> iterator
{
	using std::begin;
	return iterator{ begin(m_keys), begin(m_values) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::end() noexcept
	-> iterator
{
	using std::end;
	return iterator{ end(m_keys), end(m_values) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::begin() const noexcept
	-> const_iterator
{
	using std::cbegin;
	return const_iterator{ cbegin(m_keys), cbegin(m_values) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::end() const noexcept
	-> const_iterator
{
	using std::cend;
	return const_iterator{ cend(m_keys), cend(m_values) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::cbegin() const noexcept
	-> const_iterator
{
	using std::cbegin;
	return const_iterator{ cbegin(m_keys), cbegin(m_values) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::cend() const noexcept
	-> const_iterator
{
	using std::cend;
	return const_iterator{ cend(m_keys), cend(m_values) };
}

template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::rbegin() noexcept
	-> reverse_iterator
{
	using std::rbegin;
	return rbegin(m_keys);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::rend() noexcept
	-> reverse_iterator
{
	using std::rend;
	return rend(m_keys);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::rbegin() const noexcept
	-> const_reverse_iterator
{
	using std::crbegin;
	return crbegin(m_keys);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::rend() const noexcept
	-> const_reverse_iterator
{
	using std::crend;
	return crend(m_keys);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::crbegin() const noexcept
	-> const_reverse_iterator
{
	using std::crbegin;
	return crbegin(m_keys);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::crend() const noexcept
	-> const_reverse_iterator
{
	using std::crend;
	return crend(m_keys);
}

// Capacity:
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
bool flat_map<Key, T, Compare, KeyContainer, MappedContainer>::empty() const noexcept
{
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return m_keys.empty();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::size() const noexcept
	-> size_type
{
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return m_keys.size();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::max_size() const noexcept
	-> size_type
{
	using std::min;
	return min(m_keys.max_size(), m_values.max_size());
}

// Modifiers:
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::try_emplace(const key_type& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_if_unique<const key_type&>(key_arg, std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::try_emplace(key_type&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_if_unique<key_type&&>(std::move(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::try_emplace(const const_iterator hint, const key_type& key_arg, Args&&... args)
	-> iterator
{
	return this->do_transparent_emplace_if_unique<const key_type&>(key_arg, std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::try_emplace(const const_iterator hint, key_type&& key_arg, Args&&... args)
	-> iterator
{
	return this->do_transparent_emplace_if_unique<key_type&&>(std::move(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(const value_type& value)
	-> std::pair<iterator, bool>
{
	return this->emplace(value);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(value_type&& value)
	-> std::pair<iterator, bool>
{
	return this->emplace(std::move(value));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(const_iterator hint, const value_type& value)
	-> iterator
{
	return this->emplace_hint(hint, value);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(const_iterator hint, value_type&& value)
	-> iterator
{
	return this->emplace_hint(hint, std::move(value));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename M>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert_or_assign(const key_type& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	using coopt_transparent = void;
	return this->insert_or_assign<const key_type&, decltype(mapped_arg), key_compare, coopt_transparent>(key_arg, std::forward<M>(mapped_arg));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename M>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert_or_assign(key_type&& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	using coopt_transparent = void;
	return this->insert_or_assign<key_type&&, decltype(mapped_arg), key_compare, coopt_transparent>(std::move(key_arg), std::forward<M>(mapped_arg));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename M>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert_or_assign([[maybe_unused]] const const_iterator hint, const key_type& key_arg, M&& mapped_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->insert_or_assign<const key_type&, decltype(mapped_arg), key_compare, coopt_transparent>(key_arg, std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename M>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert_or_assign([[maybe_unused]] const const_iterator hint, key_type&& key_arg, M&& mapped_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->insert_or_assign<key_type&&, decltype(mapped_arg), key_compare, coopt_transparent>(std::move(key_arg), std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename HasIteratorCategory>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(const InputIterator first, const InputIterator last)
{
	using std::begin;
	using std::end;
	using std::next;
	using std::sort;
	using std::inplace_merge;
	const size_type pre_insert_size{ m_keys.size() };
	this->do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	// [first_iter, middle_iter) were sorted prior to calling do_insert_back_without_sorting.
	const flat::iterator_pair first_iter{ begin(m_keys), begin(m_values) };
	const flat::iterator_pair middle_iter{ next(first_iter, pre_insert_size) };
	// [middle_iter, last_iter) are the unsorted elements appended by do_insert_back_without_sorting.
	const flat::iterator_pair last_iter{ end(m_keys), end(m_values) };
	// [middle_iter, last_iter) must be sorted (not stable!).
	const value_compare comp = this->value_comp();
	sort(middle_iter, last_iter, comp);
	// Merge the two sorted ranges together (stable sort).
	inplace_merge(first_iter, middle_iter, last_iter, comp);
	// Deduplicate elements post-merge.
	this->erase_sorted_duplicates(first_iter, last_iter, comp);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename HasIteratorCategory>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(const sorted_unique_t, InputIterator first, const InputIterator last)
{
	using std::begin;
	using std::end;
	const size_type pre_insert_size{ m_keys.size() };
	this->do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	// No need to merge & unique as incoming elements are sorted & unique and
	// there were none previously.
	if (pre_insert_size != 0)
	{
		using std::next;
		using std::inplace_merge;
		// [first, middle) were sorted prior to calling do_insert_back_without_sorting.
		const flat::iterator_pair first_iter{ begin(m_keys), begin(m_values) };
		const flat::iterator_pair middle_iter{ next(first_iter, pre_insert_size) };
		// [middle, last) are the unsorted elements appended by do_insert_back_without_sorting.
		// [middle, last) is already sorted, per the sorted_unique_t tag.
		const flat::iterator_pair last_iter{ end(m_keys), end(m_values) };
		// Merge the two sorted ranges together (stable sort).
		const value_compare comp = this->value_comp();
		inplace_merge(first_iter, middle_iter, last_iter, comp);
		// Deduplicate elements post-merge.
		this->erase_sorted_duplicates(first_iter, last_iter, comp);
	}
	else
	{
		SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), this->get_less()),
			"Keys tagged with sorted_unique_t must already be sorted.");
		SH_FLAT_ASSERT(flat::less_adjacent_find(begin(m_keys), end(m_keys), this->get_less()) == end(m_keys),
			"Keys tagged with sorted_unique_t must already be unique.");
	}
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	this->insert(begin(init), end(init));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(const sorted_unique_t, std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	this->insert(sorted_unique, begin(init), end(init));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::extract() &&
	-> containers
{
	return containers{ std::move(m_keys), std::move(m_values) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::replace(containers&& cont)
{
	try
	{
		m_keys = std::move(cont.keys);
		m_values = std::move(cont.values);
		SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
			"Key & value containers expected to be the same size.");
	}
	catch (...)
	{
		this->clear();
		throw;
	}
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::erase(const const_iterator pos)
	-> iterator
{
	using std::get;
	// Desynchronization occurs if m_keys erases but m_values throws.
	return iterator{ m_keys.erase(get<0>(pos)), m_values.erase(get<1>(pos)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::erase(const const_iterator first, const const_iterator last)
	-> iterator
{
	using std::get;
	// Desynchronization occurs if m_keys erases but m_values throws.
	return iterator{ m_keys.erase(get<0>(first), get<0>(last)), m_values.erase(get<1>(first), get<1>(last)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::erase(const key_type& key_arg)
	-> size_type
{
	using coopt_transparent = void;
	return this->erase<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::swap(flat_map& other)
	noexcept(std::is_nothrow_swappable_v<key_compare>)
{
	using std::swap;
	// key_compare is allowed to throw, so it goes first.
	swap(this->get_less(), other.get_less());
	swap(m_keys, other.m_keys);
	swap(m_values, other.m_values);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::clear() noexcept
{
	m_keys.clear();
	m_values.clear();
}

// Modifiers (transparent):
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename... Args, typename IsConstructible>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::emplace(Args&&... args)
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
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename... Args, typename IsConstructible>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::emplace_hint(const const_iterator hint, Args&&... args)
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
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename... Args, typename C, typename IsTransparentAndNotIterator>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::try_emplace(K&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename... Args, typename C, typename IsTransparentAndNotIterator>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::try_emplace(const const_iterator hint, K&& key_arg, Args&&... args)
	-> iterator
{
	return this->do_transparent_emplace_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename M, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert_or_assign(K&& key_arg, M&& mapped_arg)
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
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename M, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert_or_assign(const_iterator hint, K&& key_arg, M&& mapped_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->insert_or_assign<K, M, key_compare, coopt_transparent>(std::forward<K>(key_arg), std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparentAndNotIterator>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::erase(const K& key_arg)
	-> size_type
{
	using std::get;
	using std::end;
	const iterator it = this->find(key_arg);
	if (get<0>(it) == end(m_keys))
	{
		return 0;
	}
	this->erase(it);
	return 1;
}

// Modifiers (extensions):
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename KC, typename MC, typename HasReserve>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::reserve(const size_type n)
{
	if constexpr (flat::has_reserve_v<key_container_type>)
	{
		m_keys.reserve(n);
	}
	if constexpr (flat::has_reserve_v<mapped_container_type>)
	{
		m_values.reserve(n);
	}
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::shrink_to_fit()
{
	m_keys.shrink_to_fit();
	m_values.shrink_to_fit();
}

// Lookup:
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::find(const key_type& key_arg)
	 -> iterator
{
	using coopt_transparent = void;
	return this->find<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::find(const key_type& key_arg) const
	-> const_iterator
{
	using coopt_transparent = void;
	return this->find<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::count(const key_type& key_arg) const
	-> size_type
{
	using coopt_transparent = void;
	return this->count<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
bool flat_map<Key, T, Compare, KeyContainer, MappedContainer>::contains(const key_type& key_arg) const
{
	using coopt_transparent = void;
	return this->contains<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::lower_bound(const key_type& key_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->lower_bound<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::lower_bound(const key_type& key_arg) const
	-> const_iterator
{
	using coopt_transparent = void;
	return this->lower_bound<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::upper_bound(const key_type& key_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->upper_bound<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::upper_bound(const key_type& key_arg) const
	-> const_iterator
{
	using coopt_transparent = void;
	return this->upper_bound<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::equal_range(const key_type& key_arg)
	-> std::pair<iterator, iterator>
{
	using coopt_transparent = void;
	return this->equal_range<const key_type&, key_compare, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::equal_range(const key_type& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	using coopt_transparent = void;
	return this->equal_range<const key_type&, key_compare, coopt_transparent>(key_arg);
}

// Lookup (transparent):
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::find(const K& key_arg)
	 -> iterator
{
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const flat::iterator_t<key_container_type> key_iter = this->do_find(key_arg, begin(m_keys), end(m_keys));
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return iterator{ key_iter, next(begin(m_values), distance(begin(m_keys), key_iter)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::find(const K& key_arg) const
	-> const_iterator
{
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const flat::iterator_t<const key_container_type> key_iter = this->do_find(key_arg, begin(m_keys), end(m_keys));
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return const_iterator{ key_iter, next(begin(m_values), distance(begin(m_keys), key_iter)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::count(const K& key_arg) const
	-> size_type
{
	using std::begin;
	using std::end;
	return this->do_find(key_arg, begin(m_keys), end(m_keys)) != end(m_keys) ? 1 : 0;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
bool flat_map<Key, T, Compare, KeyContainer, MappedContainer>::contains(const K& key_arg) const
{
	using std::begin;
	using std::end;
	return this->do_find(key_arg, begin(m_keys), end(m_keys)) != end(m_keys);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::lower_bound(const K& key_arg)
	-> iterator
{
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const flat::iterator_t<key_container_type> key_iter = this->do_lower_bound(key_arg, begin(m_keys), end(m_keys));
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return iterator{ key_iter, next(begin(m_values), distance(begin(m_keys), key_iter)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::lower_bound(const K& key_arg) const
	-> const_iterator
{
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const flat::iterator_t<const key_container_type> key_iter = this->do_lower_bound(key_arg, begin(m_keys), end(m_keys));
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return const_iterator{ key_iter, next(begin(m_values), distance(begin(m_keys), key_iter)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::upper_bound(const K& key_arg)
	-> iterator
{
	using std::upper_bound;
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const key_compare& less = this->get_less();
	const flat::iterator_t<key_container_type> key_iter = upper_bound(begin(m_keys), end(m_keys), key_arg, less);
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return iterator{ key_iter, next(begin(m_values), distance(begin(m_keys), key_iter)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::upper_bound(const K& key_arg) const
	-> const_iterator
{
	using std::upper_bound;
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const key_compare& less = this->get_less();
	const flat::iterator_t<const key_container_type> key_iter = upper_bound(begin(m_keys), end(m_keys), key_arg, less);
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return const_iterator{ key_iter, next(begin(m_values), distance(begin(m_keys), key_iter)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::equal_range(const K& key_arg)
	-> std::pair<iterator, iterator>
{
	using std::get;
	using std::next;
	using std::end;
	const iterator iter = this->find(key_arg);
	return { iter, get<0>(iter) == end(m_keys) ? iter : next(iter) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::equal_range(const K& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	using std::get;
	using std::next;
	using std::end;
	const const_iterator iter = this->find(key_arg);
	return { iter, get<0>(iter) == end(m_keys) ? iter : next(iter) };
}

// Observers:
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::key_comp() const
	-> key_compare
{
	return this->get_less();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::value_comp() const
	-> value_compare
{
	return value_compare{ this->get_less() };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::keys() const
	-> const key_container_type&
{
	return m_keys;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::values() const
	-> const mapped_container_type&
{
	return m_values;
}

// Non-member functions:
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
bool operator==(const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& lhs, const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& rhs)
{
	return lhs.m_keys == rhs.m_keys
		&& lhs.m_values == rhs.m_values;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
bool operator!=(const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& lhs, const flat_map<Key, T, Compare, KeyContainer, MappedContainer>& rhs)
{
	return lhs.m_keys != rhs.m_keys
		|| lhs.m_values != rhs.m_values;
}

template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename KeyIterator>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_find(const key_type& key_arg, const KeyIterator first, const KeyIterator last) const
	-> KeyIterator
{
	const KeyIterator key_iter = this->do_lower_bound(key_arg, first, last);
	return key_iter == last || this->get_less()(key_arg, *key_iter) ? last : key_iter;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename KeyIterator, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_find(const K& key_arg, const KeyIterator first, const KeyIterator last) const
	-> KeyIterator
{
	const KeyIterator key_iter = this->do_lower_bound(key_arg, first, last);
	return key_iter == last || this->get_less()(key_arg, *key_iter) ? last : key_iter;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename KeyIterator>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_lower_bound(const key_type& key_arg, const KeyIterator first, const KeyIterator last) const
	-> KeyIterator
{
	using std::lower_bound;
	return lower_bound(first, last, key_arg, this->get_less());
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename KeyIterator, typename C, typename IsTransparent>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_lower_bound(const K& key_arg, const KeyIterator first, const KeyIterator last) const
	-> KeyIterator
{
	using std::lower_bound;
	return lower_bound(first, last, key_arg, this->get_less());
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_emplace_if_unique(const key_type& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_if_unique(key_arg, std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_emplace_if_unique(key_type&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_if_unique(std::move(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_transparent_emplace_if_unique(K&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	using std::end;
	using std::get;
	const iterator iter = this->lower_bound(key_arg);
	if (get<0>(iter) == end(m_keys) || this->get_less()(key_arg, get<0>(*iter)))
	{
		// Desynchronization occurs if m_keys emplaces but m_values throws.
		const flat::iterator_t<key_container_type> key_iter = m_keys.emplace(get<0>(iter), std::forward<K>(key_arg));
		const flat::iterator_t<mapped_container_type> value_iter = m_values.emplace(get<1>(iter), std::forward<Args>(args)...);
		return { iterator{ key_iter, value_iter }, true };
	}
	return { iter, false };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_transparent_emplace_hint_if_unique(const const_iterator hint, K&& key_arg, Args&&... args)
	-> iterator
{
	using std::begin;
	using std::end;
	using std::get;
	using std::prev;
	if (m_keys.empty() == false)
	{
		const flat::iterator_t<const key_container_type> key_hint = get<0>(hint);
		const key_compare& less = this->get_less();
		if (key_hint == end(m_keys) && less(m_keys.back(), key_arg))
		{
			// Hint is at the end and the back is less than the given key.
			const flat::iterator_t<const mapped_container_type> value_hint = get<1>(hint);
			// Desynchronization occurs if m_keys emplaces but m_values throws.
			const flat::iterator_t<key_container_type> key_iter = m_keys.emplace(key_hint, std::forward<K>(key_arg));
			const flat::iterator_t<mapped_container_type> value_iter = m_values.emplace(value_hint, std::forward<Args>(args)...);
			return iterator{ key_iter, value_iter };
		}
		else if ((key_hint == begin(m_keys) || less(*prev(key_hint), key_arg))
			&& less(key_arg, *key_hint))
		{
			// Hint is at the end and the back is less than the given key.
			const flat::iterator_t<const mapped_container_type> value_hint = get<1>(hint);
			// Desynchronization occurs if m_keys emplaces but m_values throws.
			const flat::iterator_t<key_container_type> key_iter = m_keys.emplace(key_hint, std::forward<K>(key_arg));
			const flat::iterator_t<mapped_container_type> value_iter = m_values.emplace(value_hint, std::forward<Args>(args)...);
			return iterator{ key_iter, value_iter };
		}
	}
	// Fallback to binary search emplacement.
	return this->do_transparent_emplace_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_insert_back_without_sorting(InputIterator first, const InputIterator last)
{
	if constexpr (
		std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>
		&& flat::has_reserve_v<flat_map<Key, T, Compare, KeyContainer, MappedContainer>>)
	{
		using std::distance;
		this->reserve(distance(first, last));
	}
	while (first != last)
	{
		using std::get;
		m_keys.emplace_back(get<0>(*first));
		m_values.emplace_back(get<1>(*first));
		++first;
	}
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Iterator>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::erase_sorted_duplicates(const Iterator first, const Iterator last, const value_compare& comp)
{
	using std::get;
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	const Iterator result = flat::less_unique(first, last, comp);
	// Desynchronization occurs if m_keys erases but m_values throws.
	m_keys.erase(get<0>(result), get<0>(last));
	m_values.erase(get<1>(result), get<1>(last));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::sort_containers_and_erase_duplicates()
{
	using std::begin;
	using std::end;
	using std::sort;
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	flat::iterator_pair first{ begin(m_keys), begin(m_values) };
	flat::iterator_pair last{ end(m_keys), end(m_values) };
	const value_compare comp = this->value_comp();
	sort(first, last, comp);
	this->erase_sorted_duplicates(first, last, comp);
}

template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
constexpr auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::get_less() noexcept
	-> key_compare&
{
	return *this;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
constexpr auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::get_less() const noexcept
	-> const key_compare&
{
	return *this;
}

} // namespace sh

#endif
