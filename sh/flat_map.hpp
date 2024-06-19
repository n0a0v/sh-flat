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

#include "flat.hpp"
#include "tuple_algorithm.hpp"

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
	using iterator = tuple_algorithm::iterator_tuple<
		decltype(std::begin(std::declval<const key_container_type&>())),
		decltype(std::begin(std::declval<mapped_container_type&>()))>;
	using const_iterator = tuple_algorithm::iterator_tuple<
		decltype(std::begin(std::declval<const key_container_type&>())),
		decltype(std::begin(std::declval<const mapped_container_type&>()))>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	static_assert(std::is_same_v<Key, typename key_container_type::value_type>);
	static_assert(std::is_same_v<T, typename mapped_container_type::value_type>);
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
			return this->key_compare::operator()(lhs.first, rhs.first);
		}
	};

	// Member functions:
	flat_map();
	flat_map(const flat_map& other);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		const flat_map& other,
		const Allocator& alloc);
	flat_map(flat_map&& other);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		flat_map&& other,
		const Allocator& alloc);
	flat_map(
		key_container_type key_cont,
		mapped_container_type mapped_cont,
		const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		const key_container_type& key_cont,
		const mapped_container_type& mapped_cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
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
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		sorted_unique_t,
		const key_container_type& key_cont,
		const mapped_container_type& mapped_cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		sorted_unique_t,
		const key_container_type& key_cont,
		const mapped_container_type& mapped_cont,
		const key_compare& comp,
		const Allocator& alloc);
	explicit flat_map(const key_compare& comp);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(const key_compare& comp, const Allocator& alloc);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
	>
	explicit flat_map(const Allocator& alloc);
	template <typename InputIterator,
		typename = typename std::iterator_traits<InputIterator>::iterator_category
	>
	flat_map(
		InputIterator first,
		InputIterator last,
		const key_compare& comp = key_compare{});
	template <typename InputIterator, typename Allocator,
		typename = typename std::iterator_traits<InputIterator>::iterator_category,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		InputIterator first,
		InputIterator last,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename = typename std::iterator_traits<InputIterator>::iterator_category,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		InputIterator first,
		InputIterator last,
		const Allocator& alloc);
	template <typename InputIterator,
		typename = typename std::iterator_traits<InputIterator>::iterator_category
	>
	flat_map(
		sorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_compare& comp = key_compare{});
	template <typename InputIterator, typename Allocator,
		typename = typename std::iterator_traits<InputIterator>::iterator_category,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		sorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename = typename std::iterator_traits<InputIterator>::iterator_category,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
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
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		std::initializer_list<value_type> init,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	flat_map(sorted_unique_t,
		std::initializer_list<value_type> init,
		const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
	>
	flat_map(
		sorted_unique_t,
		std::initializer_list<value_type> init,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<key_container_type, Allocator> && uses_allocator_v<mapped_container_type, Allocator>>
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
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
	mapped_type& at(const K& key_arg);
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
	const mapped_type& at(const K& key_arg) const;
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
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
		typename = typename std::iterator_traits<InputIterator>::iterator_category
	>
	void insert(InputIterator first, InputIterator last);
	template <typename InputIterator,
		typename = typename std::iterator_traits<InputIterator>::iterator_category
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
	template <typename... Args> std::pair<iterator, bool> emplace(Args&&... args);
	template <typename... Args> iterator emplace_hint(const_iterator hint, Args&&... args);
	template <typename K, typename... Args,
		typename C = key_compare,
		typename = typename C::is_transparent,
		typename = std::enable_if_t<
			false == std::is_convertible_v<K&&, const_iterator>
			&& false == std::is_convertible_v<K&&, iterator>
		>
	>
	std::pair<iterator, bool> try_emplace(K&& key_arg, Args&&... args);
	template <typename K, typename... Args,
		typename C = key_compare,
		typename = typename C::is_transparent,
		typename = std::enable_if_t<
			false == std::is_convertible_v<K&&, const_iterator>
			&& false == std::is_convertible_v<K&&, iterator>
		>
	>
	iterator try_emplace(const_iterator hint, K&& key_arg, Args&&... args);
	template <typename K, typename M, typename C = key_compare, typename = typename C::is_transparent>
	std::pair<iterator, bool> insert_or_assign(K&& key_arg, M&& mapped_arg);
	template <typename K, typename M, typename C = key_compare, typename = typename C::is_transparent>
	iterator insert_or_assign(const_iterator hint, K&& key_arg, M&& mapped_arg);
	template <typename K,
		typename C = key_compare,
		typename = typename C::is_transparent,
		typename = std::enable_if_t<
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
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
	iterator find(const K& key_arg);
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
	const_iterator find(const K& key_arg) const;
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
	size_type count(const K& key_arg) const;
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
	bool contains(const K& key_arg) const;
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
	iterator lower_bound(const K& key_arg);
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
	const_iterator lower_bound(const K& key_arg) const;
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
	iterator upper_bound(const K& key_arg);
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
	const_iterator upper_bound(const K& key_arg) const;
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
	std::pair<iterator, iterator> equal_range(const K& key_arg);
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
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
	template <typename K, typename KeyIterator, typename C = key_compare, typename = typename C::is_transparent>
	KeyIterator do_find(const K& key_arg, const KeyIterator first, const KeyIterator last) const;
	template <typename KeyIterator>
	KeyIterator do_lower_bound(const key_type& key_arg, const KeyIterator first, const KeyIterator last) const;
	template <typename K, typename KeyIterator, typename C = key_compare, typename = typename C::is_transparent>
	KeyIterator do_lower_bound(const K& key_arg, const KeyIterator first, const KeyIterator last) const;
	template <typename K, typename... Args>
	std::pair<iterator, bool> do_transparent_emplace_if_unique(K&& key_arg, Args&&... args);
	template <typename InputIterator>
	void do_insert_back_without_sorting(InputIterator first, InputIterator last);
	void sort_containers_and_erase_duplicates();

	constexpr key_compare& get_less() noexcept;
	constexpr const key_compare& get_less() const noexcept;
	constexpr auto make_equal() const noexcept;

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
template <typename Allocator, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const flat_map& other, const Allocator& alloc)
	: key_compare{ other.get_less() }
	, m_keys{ other.m_keys, alloc }
	, m_values{ other.m_values, alloc }
{ }
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(flat_map&& other)
	: key_compare{ std::move(other.get_less()) }
	, m_keys{ std::move(other.m_keys) }
	, m_values{ std::move(other.m_values) }
{ }
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename>
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
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const key_container_type& key_cont, const mapped_container_type& mapped_cont, const Allocator& alloc)
	: key_compare{}
	, m_keys{ key_cont, alloc }
	, m_values{ mapped_cont, alloc }
{
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	// Sort & unique as containers simply copied in above.
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const key_container_type& key_cont, const mapped_container_type& mapped_cont, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ key_cont, alloc }
	, m_values{ mapped_cont, alloc }
{
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	// Sort & unique as containers simply copied in above.
	sort_containers_and_erase_duplicates();
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
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(std::adjacent_find(begin(m_keys), end(m_keys), make_equal()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, const key_container_type& key_cont, const mapped_container_type& mapped_cont, const Allocator& alloc)
	: key_compare{}
	, m_keys{ key_cont, alloc }
	, m_values{ mapped_cont, alloc }
{
	using std::begin;
	using std::end;
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(std::adjacent_find(begin(m_keys), end(m_keys), make_equal()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, const key_container_type& key_cont, const mapped_container_type& mapped_cont, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ key_cont, alloc }
	, m_values{ mapped_cont, alloc }
{
	using std::begin;
	using std::end;
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(std::adjacent_find(begin(m_keys), end(m_keys), make_equal()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const key_compare& comp)
	: key_compare{ comp }
	, m_keys{ }
	, m_values{ }
{ }
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ alloc }
	, m_values{ alloc }
{ }
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const Allocator& alloc)
	: key_compare{}
	, m_keys{ alloc }
	, m_values{ alloc }
{ }
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const InputIterator first, const InputIterator last, const key_compare& comp)
	: key_compare{ comp }
	, m_keys{}
	, m_values{}
{
	do_insert_back_without_sorting(first, last);
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename Allocator, typename, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const InputIterator first, const InputIterator last, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ alloc }
	, m_values{ alloc }
{
	do_insert_back_without_sorting(first, last);
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename Allocator, typename, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(const InputIterator first, const InputIterator last, const Allocator& alloc)
	: key_compare{}
	, m_keys{ alloc }
	, m_values{ alloc }
{
	do_insert_back_without_sorting(first, last);
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, const InputIterator first, const InputIterator last, const key_compare& comp)
	: key_compare{ comp }
	, m_keys{}
	, m_values{}
{
	using std::begin;
	using std::end;
	// Add unsorted and assume unique because of sorted_unique tag.
	do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(std::adjacent_find(begin(m_keys), end(m_keys), make_equal()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename Allocator, typename, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, const InputIterator first, const InputIterator last, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	// Add unsorted and assume unique because of sorted_unique tag.
	do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(std::adjacent_find(begin(m_keys), end(m_keys), make_equal()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename Allocator, typename, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, const InputIterator first, const InputIterator last, const Allocator& alloc)
	: key_compare{}
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	// Add unsorted and assume unique because of sorted_unique tag.
	do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(std::adjacent_find(begin(m_keys), end(m_keys), make_equal()) == end(m_keys),
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
	do_insert_back_without_sorting(begin(init), end(init));
	sort_containers_and_erase_duplicates();
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(std::adjacent_find(begin(m_keys), end(m_keys), make_equal()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(std::initializer_list<value_type> init, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	// Add unsorted and then sort & unique.
	do_insert_back_without_sorting(begin(init), end(init));
	sort_containers_and_erase_duplicates();
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(std::adjacent_find(begin(m_keys), end(m_keys), make_equal()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(std::initializer_list<value_type> init, const Allocator& alloc)
	: key_compare{}
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	// Add unsorted and then sort & unique.
	do_insert_back_without_sorting(begin(init), end(init));
	sort_containers_and_erase_duplicates();
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(std::adjacent_find(begin(m_keys), end(m_keys), make_equal()) == end(m_keys),
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
	do_insert_back_without_sorting(begin(init), end(init));
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(std::adjacent_find(begin(m_keys), end(m_keys), make_equal()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, std::initializer_list<value_type> init, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	do_insert_back_without_sorting(begin(init), end(init));
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(std::adjacent_find(begin(m_keys), end(m_keys), make_equal()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename>
flat_map<Key, T, Compare, KeyContainer, MappedContainer>::flat_map(sorted_unique_t, std::initializer_list<value_type> init, const Allocator& alloc)
	: key_compare{}
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	do_insert_back_without_sorting(begin(init), end(init));
	SH_FLAT_ASSERT(std::is_sorted(begin(m_keys), end(m_keys), get_less()),
		"Keys tagged with sorted_unique_t must already be sorted.");
	SH_FLAT_ASSERT(std::adjacent_find(begin(m_keys), end(m_keys), make_equal()) == end(m_keys),
		"Keys tagged with sorted_unique_t must already be unique.");
}

template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::operator=(const flat_map& other)
	-> flat_map&
{
	get_less() = other.get_less();
	m_keys = other.m_keys;
	m_values = other.m_values;
	return *this;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::operator=(flat_map&& other) noexcept
	-> flat_map&
{
	get_less() = std::move(other.get_less());
	m_keys = std::move(other.m_keys);
	m_values = std::move(other.m_values);
	return *this;
}

// Element access:
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::at(const key_type& key_arg)
	-> mapped_type&
{
	return this->at<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::at(const key_type& key_arg) const
	-> const mapped_type&
{
	return this->at<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::operator[](const key_type& key_arg)
	-> mapped_type&
{
	return this->operator[]<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::operator[](key_type&& key_arg)
	-> mapped_type&
{
	return this->operator[]<key_type&&, void, void>(std::move(key_arg));
}

// Element access (transparent):
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::at(const K& key_arg)
	-> mapped_type&
{
	using std::get;
	using std::end;
	const auto iter = find(key_arg);
	if (get<0>(iter) == end(m_keys))
	{
		throw std::out_of_range{ "flat_map::at" };
	}
	return *get<1>(iter);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::at(const K& key_arg) const
	-> const mapped_type&
{
	using std::get;
	using std::end;
	const auto iter = find(key_arg);
	if (get<0>(iter) == end(m_keys))
	{
		throw std::out_of_range{ "flat_map::at" };
	}
	return *get<1>(iter);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::operator[](K&& key_arg)
	-> mapped_type&
{
	return do_transparent_emplace_if_unique(std::forward<K>(key_arg)).first->second;
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
	return const_iterator{ m_keys.cbegin(), m_values.cbegin() };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::end() const noexcept
	-> const_iterator
{
	return const_iterator{ m_keys.cend(), m_values.cend() };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::cbegin() const noexcept
	-> const_iterator
{
	return const_iterator{ m_keys.cbegin(), m_values.cbegin() };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::cend() const noexcept
	-> const_iterator
{
	return const_iterator{ m_keys.cend(), m_values.cend() };
}

template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::rbegin() noexcept
	-> reverse_iterator
{
	return m_keys.rbegin();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::rend() noexcept
	-> reverse_iterator
{
	return m_keys.rend();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::rbegin() const noexcept
	-> const_reverse_iterator
{
	return m_keys.crbegin();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::rend() const noexcept
	-> const_reverse_iterator
{
	return m_keys.crend();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::crbegin() const noexcept
	-> const_reverse_iterator
{
	return m_keys.crbegin();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::crend() const noexcept
	-> const_reverse_iterator
{
	return m_keys.crend();
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
	return m_keys.max_size();
}

// Modifiers:
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::try_emplace(const key_type& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return do_transparent_emplace_if_unique<const key_type&>(key_arg, std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::try_emplace(key_type&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return do_transparent_emplace_if_unique<key_type&&>(std::move(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::try_emplace(const const_iterator hint, const key_type& key_arg, Args&&... args)
	-> iterator
{
	return do_transparent_emplace_if_unique<const key_type&>(key_arg, std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::try_emplace(const const_iterator hint, key_type&& key_arg, Args&&... args)
	-> iterator
{
	return do_transparent_emplace_if_unique<key_type&&>(std::move(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(const value_type& value)
	-> std::pair<iterator, bool>
{
	return emplace(value);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(value_type&& value)
	-> std::pair<iterator, bool>
{
	return emplace(std::move(value));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(const_iterator hint, const value_type& value)
	-> iterator
{
	return emplace_hint(hint, value);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(const_iterator hint, value_type&& value)
	-> iterator
{
	return emplace_hint(hint, std::move(value));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename M>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert_or_assign(const key_type& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	return this->insert_or_assign<const key_type&, decltype(mapped_arg), void, void>(key_arg, std::forward<M>(mapped_arg));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename M>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert_or_assign(key_type&& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	return this->insert_or_assign<key_type&&, decltype(mapped_arg), void, void>(std::move(key_arg), std::forward<M>(mapped_arg));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename M>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert_or_assign(const_iterator hint, const key_type& key_arg, M&& mapped_arg)
	-> iterator
{
	return this->insert_or_assign<const key_type&, decltype(mapped_arg), void, void>(key_arg, std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename M>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert_or_assign(const_iterator hint, key_type&& key_arg, M&& mapped_arg)
	-> iterator
{
	return this->insert_or_assign<key_type&&, decltype(mapped_arg), void, void>(std::move(key_arg), std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(const InputIterator first, const InputIterator last)
{
	using std::begin;
	using std::end;
	using std::next;
	using std::sort;
	using std::inplace_merge;
	using std::unique;
	using std::get;
	const size_type pre_insert_size{ m_keys.size() };
	do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	// [first_iter, middle_iter) were sorted prior to calling do_insert_back_without_sorting.
	const auto first_iter = tuple_algorithm::iterator_tuple{ begin(m_keys), begin(m_values) };
	const auto middle_iter = next(first_iter, pre_insert_size);
	// [middle_iter, last_iter) are the unsorted elements appended by do_insert_back_without_sorting.
	const auto last_iter = tuple_algorithm::iterator_tuple{ end(m_keys), end(m_values) };
	// [middle_iter, last_iter) must be sorted (not stable!).
	sort(middle_iter, last_iter, value_compare{ get_less() });
	// Merge the two sorted ranges together (stable sort).
	inplace_merge(first_iter, middle_iter, last_iter, value_compare{ get_less() });
	// Deduplicate elements post-merge.
	const auto iter = unique(first_iter, last_iter,
		[equal = make_equal()](const auto& lhs, const auto& rhs)
		{
			return equal(get<0>(lhs), get<0>(rhs));
		});
	// Desynchronization occurs if m_keys erases but m_values throws.
	m_keys.erase(get<0>(iter), end(m_keys));
	m_values.erase(get<1>(iter), end(m_values));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(const sorted_unique_t, InputIterator first, const InputIterator last)
{
	using std::begin;
	using std::end;
	using std::next;
	using std::inplace_merge;
	using std::unique;
	using std::get;
	const size_type pre_insert_size{ m_keys.size() };
	do_insert_back_without_sorting(first, last);
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	// [first, middle) were sorted prior to calling do_insert_back_without_sorting.
	const auto first_iter = tuple_algorithm::iterator_tuple{ begin(m_keys), begin(m_values) };
	const auto middle_iter = next(first_iter, pre_insert_size);
	// [middle, last) are the unsorted elements appended by do_insert_back_without_sorting.
	// [middle, last) is already sorted, per the sorted_unique_t tag.
	const auto last_iter = tuple_algorithm::iterator_tuple{ end(m_keys), end(m_values) };
	// Merge the two sorted ranges together (stable sort).
	inplace_merge(first_iter, middle_iter, last_iter, value_compare{ get_less() });
	// Deduplicate elements post-merge.
	const auto iter = unique(first_iter, last_iter,
		[equal = make_equal()](const auto& lhs, const auto& rhs)
		{
			return equal(get<0>(lhs), get<0>(rhs));
		});
	// Desynchronization occurs if m_keys erases but m_values throws.
	m_keys.erase(get<0>(iter), end(m_keys));
	m_values.erase(get<1>(iter), end(m_values));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	insert(begin(init), end(init));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert(const sorted_unique_t, std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	insert(sorted_unique, begin(init), end(init));
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
		clear();
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
	return this->erase<const key_type&, void, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::swap(flat_map& other)
	noexcept(std::is_nothrow_swappable_v<key_compare>)
{
	using std::swap;
	// key_compare is allowed to throw, so it goes first.
	swap(get_less(), other.get_less());
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
template <typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::emplace(Args&&... args)
	-> std::pair<iterator, bool>
{
	if constexpr (sizeof...(args) == 2)
	{
		// If possible, keep emplace a transparent operation.
		return do_transparent_emplace_if_unique(std::forward<Args>(args)...);
	}
	else
	{
		value_type value{ std::forward<Args>(args)... };
		return do_transparent_emplace_if_unique(std::move(value.first), std::move(value.second));
	}
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::emplace_hint(const const_iterator hint, Args&&... args)
	-> iterator
{
	return emplace(std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename... Args, typename, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::try_emplace(K&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return do_transparent_emplace_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename... Args, typename, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::try_emplace(const const_iterator hint, K&& key_arg, Args&&... args)
	-> iterator
{
	return do_transparent_emplace_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename M, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert_or_assign(K&& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	const std::pair<iterator, bool> it_inserted = do_transparent_emplace_if_unique(std::forward<K>(key_arg), std::forward<M>(mapped_arg));
	if (it_inserted.second == false)
	{
		it_inserted.first->second = std::forward<M>(mapped_arg);
	}
	return it_inserted;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename M, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::insert_or_assign(const_iterator hint, K&& key_arg, M&& mapped_arg)
	-> iterator
{
	return this->insert_or_assign<K, M, void, void>(std::forward<K>(key_arg), std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::erase(const K& key_arg)
	-> size_type
{
	using std::get;
	using std::end;
	const iterator it = find(key_arg);
	if (get<0>(it) == end(m_keys))
	{
		return 0;
	}
	erase(it);
	return 1;
}

// Modifiers (extensions):
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::reserve(const size_type n)
{
	m_keys.reserve(n);
	m_values.reserve(n);
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
	return this->find<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::find(const key_type& key_arg) const
	-> const_iterator
{
	return this->find<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::count(const key_type& key_arg) const
	-> size_type
{
	return this->count<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
bool flat_map<Key, T, Compare, KeyContainer, MappedContainer>::contains(const key_type& key_arg) const
{
	return this->count<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::lower_bound(const key_type& key_arg)
	-> iterator
{
	return this->lower_bound<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::lower_bound(const key_type& key_arg) const
	-> const_iterator
{
	return this->lower_bound<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::upper_bound(const key_type& key_arg)
	-> iterator
{
	return this->upper_bound<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::upper_bound(const key_type& key_arg) const
	-> const_iterator
{
	return this->upper_bound<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::equal_range(const key_type& key_arg)
	-> std::pair<iterator, iterator>
{
	return this->equal_range<const key_type&, void, void>(key_arg);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::equal_range(const key_type& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	return this->equal_range<const key_type&, void, void>(key_arg);
}

// Lookup (transparent):
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::find(const K& key_arg)
	 -> iterator
{
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const auto key_iter = do_find(key_arg, begin(m_keys), end(m_keys));
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return iterator{ key_iter, next(begin(m_values), distance(begin(m_keys), key_iter)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::find(const K& key_arg) const
	-> const_iterator
{
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const auto key_iter = do_find(key_arg, begin(m_keys), end(m_keys));
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return const_iterator{ key_iter, next(begin(m_values), distance(begin(m_keys), key_iter)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::count(const K& key_arg) const
	-> size_type
{
	using std::begin;
	using std::end;
	return do_find(key_arg, begin(m_keys), end(m_keys)) != end(m_keys) ? 1 : 0;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename>
bool flat_map<Key, T, Compare, KeyContainer, MappedContainer>::contains(const K& key_arg) const
{
	using std::begin;
	using std::end;
	return do_find(key_arg, begin(m_keys), end(m_keys)) != end(m_keys);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::lower_bound(const K& key_arg)
	-> iterator
{
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const auto key_iter = do_lower_bound(key_arg, begin(m_keys), end(m_keys));
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return iterator{ key_iter, next(begin(m_values), distance(begin(m_keys), key_iter)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::lower_bound(const K& key_arg) const
	-> const_iterator
{
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const auto key_iter = do_lower_bound(key_arg, begin(m_keys), end(m_keys));
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return const_iterator{ key_iter, next(begin(m_values), distance(begin(m_keys), key_iter)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::upper_bound(const K& key_arg)
	-> iterator
{
	using std::upper_bound;
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const auto& less = get_less();
	const auto key_iter = upper_bound(begin(m_keys), end(m_keys), key_arg, less);
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return iterator{ key_iter, next(begin(m_values), distance(begin(m_keys), key_iter)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::upper_bound(const K& key_arg) const
	-> const_iterator
{
	using std::upper_bound;
	using std::begin;
	using std::end;
	using std::next;
	using std::distance;
	const auto& less = get_less();
	const auto key_iter = upper_bound(begin(m_keys), end(m_keys), key_arg, less);
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return const_iterator{ key_iter, next(begin(m_values), distance(begin(m_keys), key_iter)) };
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::equal_range(const K& key_arg)
	-> std::pair<iterator, iterator>
{
	using std::get;
	using std::next;
	using std::end;
	const iterator iter = find(key_arg);
	return std::make_pair(iter, get<0>(iter) == end(m_keys) ? iter : next(iter));
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::equal_range(const K& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	using std::get;
	using std::next;
	using std::end;
	const const_iterator iter = find(key_arg);
	return std::make_pair(iter, get<0>(iter) == end(m_keys) ? iter : next(iter));
}

// Observers:
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::key_comp() const
	-> key_compare
{
	return get_less();
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::value_comp() const
	-> value_compare
{
	return value_compare{ get_less() };
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
	const auto key_iter = do_lower_bound(key_arg, first, last);
	return key_iter == last || get_less()(key_arg, *key_iter) ? last : key_iter;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename KeyIterator, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_find(const K& key_arg, const KeyIterator first, const KeyIterator last) const
	-> KeyIterator
{
	const auto key_iter = do_lower_bound(key_arg, first, last);
	return key_iter == last || get_less()(key_arg, *key_iter) ? last : key_iter;
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename KeyIterator>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_lower_bound(const key_type& key_arg, const KeyIterator first, const KeyIterator last) const
	-> KeyIterator
{
	using std::lower_bound;
	return lower_bound(first, last, key_arg, get_less());
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename KeyIterator, typename, typename>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_lower_bound(const K& key_arg, const KeyIterator first, const KeyIterator last) const
	-> KeyIterator
{
	using std::lower_bound;
	return lower_bound(first, last, key_arg, get_less());
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename K, typename... Args>
auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_transparent_emplace_if_unique(K&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	using std::get;
	using std::end;
	const iterator iter = lower_bound(key_arg);
	if (get<0>(iter) == end(m_keys) || get_less()(key_arg, iter->first))
	{
		const auto key_iter = m_keys.emplace(get<0>(iter), std::forward<K>(key_arg));
		const auto value_iter = m_values.emplace(get<1>(iter), std::forward<Args>(args)...);
		return std::make_pair(iterator{ key_iter, value_iter }, true);
	}
	return std::make_pair(iter, false);
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
template <typename InputIterator>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::do_insert_back_without_sorting(InputIterator first, const InputIterator last)
{
	if constexpr (has_reserve_v<key_container_type> && has_reserve_v<mapped_container_type>)
	{
		using std::distance;
		reserve(distance(first, last));
	}
	while (first != last)
	{
		m_keys.emplace_back(first->first);
		m_values.emplace_back(first->second);
		++first;
	}
}
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
void flat_map<Key, T, Compare, KeyContainer, MappedContainer>::sort_containers_and_erase_duplicates()
{
	using std::begin;
	using std::end;
	using std::sort;
	using std::unique;
	using std::get;
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	auto first = tuple_algorithm::iterator_tuple{ begin(m_keys), begin(m_values) };
	auto last = tuple_algorithm::iterator_tuple{ end(m_keys), end(m_values) };
	sort(first, last, value_compare{ get_less() });
	const auto iter = unique(first, last,
		[equal = make_equal()](const auto& lhs, const auto& rhs)
		{
			return equal(get<0>(lhs), get<0>(rhs));
		});
	m_keys.erase(get<0>(iter), end(m_keys));
	m_values.erase(get<1>(iter), end(m_values));
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
template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
constexpr auto flat_map<Key, T, Compare, KeyContainer, MappedContainer>::make_equal() const noexcept
{
	return [this](const auto& key_arg1, const auto& key_arg2)
	{
		auto& less = get_less();
		return less(key_arg1, key_arg2) == false && less(key_arg2, key_arg1) == false;
	};
}

} // namespace sh

#endif
