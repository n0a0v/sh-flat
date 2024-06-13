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

#ifndef INC_SH__FLAT_SET_HPP
#define INC_SH__FLAT_SET_HPP

#include "flat.hpp"

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <utility>
#include <vector>

namespace sh
{

template <typename Key,
	typename Compare = std::less<Key>,
	typename KeyContainer = std::vector<Key>>
class flat_set : private Compare
{
public:
	using container_type = KeyContainer;
	using key_type = Key;
	using value_type = Key;
	using key_compare = Compare;
	using value_compare = Compare;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = typename KeyContainer::size_type;
	using difference_type = typename KeyContainer::difference_type;
	using iterator = decltype(std::declval<const container_type&>().begin());
	using const_iterator = iterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	// Member functions:
	flat_set();
	flat_set(const flat_set& other);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		const flat_set& other,
		const Allocator& alloc);
	flat_set(flat_set&& other);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		flat_set&& other,
		const Allocator& alloc);
	explicit flat_set(
		container_type cont,
		const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		const container_type& cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		const container_type& cont,
		const key_compare& comp,
		const Allocator& alloc);
	flat_set(
		sorted_unique_t,
		container_type cont,
		const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		sorted_unique_t,
		const container_type& cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		sorted_unique_t,
		const container_type& cont,
		const key_compare& comp,
		const Allocator& alloc);
	explicit flat_set(
		const key_compare& comp);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		const key_compare& comp,
		const Allocator& alloc);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	explicit flat_set(
		const Allocator& alloc);
	template <typename InputIterator,
		typename = typename std::iterator_traits<InputIterator>::iterator_category
	>
	flat_set(
		InputIterator first,
		InputIterator last,
		const key_compare& comp = key_compare{});
	template <typename InputIterator,
		typename Allocator,
		typename = typename std::iterator_traits<InputIterator>::iterator_category,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		InputIterator first,
		InputIterator last,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename InputIterator,
		typename Allocator,
		typename = typename std::iterator_traits<InputIterator>::iterator_category,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		InputIterator first,
		InputIterator last,
		const Allocator& alloc);
	template <typename InputIterator,
		typename = typename std::iterator_traits<InputIterator>::iterator_category
	>
	flat_set(
		sorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_compare& comp = key_compare{});
	template <typename InputIterator,
		typename Allocator,
		typename = typename std::iterator_traits<InputIterator>::iterator_category,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		sorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename InputIterator,
		typename Allocator,
		typename = typename std::iterator_traits<InputIterator>::iterator_category,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		sorted_unique_t,
		InputIterator first,
		InputIterator last,
		const Allocator& alloc);
	flat_set(
		std::initializer_list<value_type> init,
		const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		std::initializer_list<value_type> init,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	flat_set(
		sorted_unique_t,
		std::initializer_list<value_type> init,
		const key_compare& comp = key_compare{});
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		sorted_unique_t,
		std::initializer_list<value_type> init,
		const key_compare& comp,
		const Allocator& alloc);
	template <typename Allocator,
		typename = std::enable_if_t<uses_allocator_v<container_type, Allocator>>
	>
	flat_set(
		sorted_unique_t,
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	~flat_set() = default;

	flat_set& operator=(const flat_set& other);
	flat_set& operator=(flat_set&& other) noexcept;

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
	std::pair<iterator, bool> insert(const value_type& value);
	std::pair<iterator, bool> insert(value_type&& value);
	iterator insert(const_iterator hint, const value_type& value);
	iterator insert(const_iterator hint, value_type&& value);
	template <typename InputIterator> void insert(InputIterator first, InputIterator last);
	template <typename InputIterator> void insert(sorted_unique_t, InputIterator first, InputIterator last);
	void insert(std::initializer_list<value_type> init);
	void insert(sorted_unique_t, std::initializer_list<value_type> init);
	container_type extract() &&;
	void replace(container_type&& cont);
	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);
	size_type erase(const key_type& key_arg);
	void swap(flat_set& other) noexcept;
	void clear() noexcept;

	// Modifiers (transparent):
	template <typename K>
	std::pair<iterator, bool> emplace(K&& key_arg);
	template <typename K>
	iterator emplace_hint(const_iterator hint, K&& key_arg);
	template <typename K, typename C = key_compare, typename = typename C::is_transparent>
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

	// Non-member functions:
	template <typename Key2, typename Compare2, typename KeyContainer2>
	friend bool operator==(const flat_set<Key2, Compare2, KeyContainer2>& lhs, const flat_set<Key2, Compare2, KeyContainer2>& rhs);
	template <typename Key2, typename Compare2, typename KeyContainer2>
	friend bool operator!=(const flat_set<Key2, Compare2, KeyContainer2>& lhs, const flat_set<Key2, Compare2, KeyContainer2>& rhs);

private:
	template <typename Iterator>
	Iterator do_find(const key_type& key_arg, const Iterator first, const Iterator last) const;
	template <typename K, typename Iterator, typename C = key_compare, typename = typename C::is_transparent>
	Iterator do_find(const K& key_arg, const Iterator first, const Iterator last) const;
	void sort_containers_and_erase_duplicates();

	constexpr key_compare& get_less() noexcept;
	constexpr const key_compare& get_less() const noexcept;
	constexpr auto make_equal() const noexcept;

	container_type m_keys;
};

// Member functions:
template <typename Key, typename Compare, typename KeyContainer>
flat_set<Key, Compare, KeyContainer>::flat_set()
	: flat_set{ key_compare{} }
{ }
template <typename Key, typename Compare, typename KeyContainer>
flat_set<Key, Compare, KeyContainer>::flat_set(const flat_set& other)
	: key_compare{ other.get_less() }
	, m_keys{ other.m_keys }
{ }
template <typename Key, typename Compare, typename KeyContainer>
template <typename Allocator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(const flat_set& other, const Allocator& alloc)
	: key_compare{ other.get_less() }
	, m_keys{ other.m_keys, alloc }
{ }
template <typename Key, typename Compare, typename KeyContainer>
flat_set<Key, Compare, KeyContainer>::flat_set(flat_set&& other)
	: key_compare{ std::move(other.get_less()) }
	, m_keys{ std::move(other.m_keys) }
{ }
template <typename Key, typename Compare, typename KeyContainer>
template <typename Allocator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(flat_set&& other, const Allocator& alloc)
	: key_compare{ std::move(other.get_less()) }
	, m_keys{ std::move(other.m_keys), alloc }
{ }
template <typename Key, typename Compare, typename KeyContainer>
flat_set<Key, Compare, KeyContainer>::flat_set(container_type cont, const key_compare& comp)
	: key_compare{ comp }
	, m_keys{ std::move(cont) }
{
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename Allocator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(const container_type& cont, const Allocator& alloc)
	: key_compare{}
	, m_keys{ cont, alloc }
{
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename Allocator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(const container_type& cont, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ cont, alloc }
{
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename Compare, typename KeyContainer>
flat_set<Key, Compare, KeyContainer>::flat_set(sorted_unique_t, container_type cont, const key_compare& comp)
	: key_compare{ comp }
	, m_keys{ std::move(cont) }
{ }
template <typename Key, typename Compare, typename KeyContainer>
template <typename Allocator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(sorted_unique_t, const container_type& cont, const Allocator& alloc)
	: key_compare{}
	, m_keys{ cont, alloc }
{ }
template <typename Key, typename Compare, typename KeyContainer>
template <typename Allocator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(sorted_unique_t, const container_type& cont, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ cont, alloc }
{ }
template <typename Key, typename Compare, typename KeyContainer>
flat_set<Key, Compare, KeyContainer>::flat_set(const key_compare& comp)
	: key_compare{ comp }
	, m_keys{ }
{ }
template <typename Key, typename Compare, typename KeyContainer>
template <typename Allocator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ alloc }
{ }
template <typename Key, typename Compare, typename KeyContainer>
template <typename Allocator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(const Allocator& alloc)
	: key_compare{}
	, m_keys{ alloc }
{ }
template <typename Key, typename Compare, typename KeyContainer>
template <typename InputIterator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(InputIterator first, InputIterator last, const key_compare& comp)
	: key_compare{ comp }
	, m_keys{ first, last }
{
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename InputIterator, typename Allocator, typename, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(InputIterator first, InputIterator last, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ first, last, alloc }
{
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename InputIterator, typename Allocator, typename, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(InputIterator first, InputIterator last, const Allocator& alloc)
	: key_compare{}
	, m_keys{ first, last, alloc }
{
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename InputIterator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(sorted_unique_t, InputIterator first, InputIterator last, const key_compare& comp)
	: key_compare{ comp }
	, m_keys{ first, last }
{ }
template <typename Key, typename Compare, typename KeyContainer>
template <typename InputIterator, typename Allocator, typename, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(sorted_unique_t, InputIterator first, InputIterator last, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ first, last, alloc }
{ }
template <typename Key, typename Compare, typename KeyContainer>
template <typename InputIterator, typename Allocator, typename, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(sorted_unique_t, InputIterator first, InputIterator last, const Allocator& alloc)
	: key_compare{}
	, m_keys{ first, last, alloc }
{ }
template <typename Key, typename Compare, typename KeyContainer>
flat_set<Key, Compare, KeyContainer>::flat_set(std::initializer_list<value_type> init, const key_compare& comp)
	: key_compare{ comp }
	, m_keys{ std::move(init) }
{
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename Allocator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(std::initializer_list<value_type> init, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ std::move(init), alloc }
{
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename Allocator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(std::initializer_list<value_type> init, const Allocator& alloc)
	: key_compare{}
	, m_keys{ std::move(init), alloc }
{
	sort_containers_and_erase_duplicates();
}
template <typename Key, typename Compare, typename KeyContainer>
flat_set<Key, Compare, KeyContainer>::flat_set(sorted_unique_t, std::initializer_list<value_type> init, const key_compare& comp)
	: key_compare{ comp }
	, m_keys{ std::move(init) }
{ }
template <typename Key, typename Compare, typename KeyContainer>
template <typename Allocator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(sorted_unique_t, std::initializer_list<value_type> init, const key_compare& comp, const Allocator& alloc)
	: key_compare{ comp }
	, m_keys{ std::move(init), alloc }
{ }
template <typename Key, typename Compare, typename KeyContainer>
template <typename Allocator, typename>
flat_set<Key, Compare, KeyContainer>::flat_set(sorted_unique_t, std::initializer_list<value_type> init, const Allocator& alloc)
	: key_compare{}
	, m_keys{ std::move(init), alloc }
{ }

template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::operator=(const flat_set& other)
	-> flat_set&
{
	get_less() = other.get_less();
	m_keys = other.m_keys;
	return *this;
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::operator=(flat_set&& other) noexcept
	-> flat_set&
{
	get_less() = std::move(other.get_less());
	m_keys = std::move(other.m_keys);
	return *this;
}

// Iterators:
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::begin() noexcept
	-> iterator
{
	return m_keys.begin();
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::end() noexcept
	-> iterator
{
	return m_keys.end();
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::begin() const noexcept
	-> const_iterator
{
	return m_keys.cbegin();
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::end() const noexcept
	-> const_iterator
{
	return m_keys.cend();
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::cbegin() const noexcept
	-> const_iterator
{
	return m_keys.cbegin();
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::cend() const noexcept
	-> const_iterator
{
	return m_keys.cend();
}

template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::rbegin() noexcept
	-> reverse_iterator
{
	return m_keys.rbegin();
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::rend() noexcept
	-> reverse_iterator
{
	return m_keys.rend();
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::rbegin() const noexcept
	-> const_reverse_iterator
{
	return m_keys.crbegin();
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::rend() const noexcept
	-> const_reverse_iterator
{
	return m_keys.crend();
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::crbegin() const noexcept
	-> const_reverse_iterator
{
	return m_keys.crbegin();
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::crend() const noexcept
	-> const_reverse_iterator
{
	return m_keys.crend();
}

// Capacity:
template <typename Key, typename Compare, typename KeyContainer>
bool flat_set<Key, Compare, KeyContainer>::empty() const noexcept
{
	return m_keys.empty();
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::size() const noexcept
	-> size_type
{
	return m_keys.size();
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::max_size() const noexcept
	-> size_type
{
	return m_keys.max_size();
}

// Modifiers:
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::insert(const value_type& value)
	-> std::pair<iterator, bool>
{
	return emplace(value);
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::insert(value_type&& value)
	-> std::pair<iterator, bool>
{
	return emplace(std::move(value));
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::insert(const const_iterator hint, const value_type& value)
	-> iterator
{
	return emplace_hint(hint, value);
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::insert(const const_iterator hint, value_type&& value)
	-> iterator
{
	return emplace_hint(hint, std::move(value));
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename InputIterator>
void flat_set<Key, Compare, KeyContainer>::insert(const InputIterator first, const InputIterator last)
{
	using std::sort;
	using std::inplace_merge;
	using std::unique;
	// [first, iter) are sorted prior to calling insert.
	auto iter = m_keys.insert(m_keys.end(), first, last);
	// [iter, end()) must be sorted (not stable!).
	sort(iter, m_keys.end(), get_less());
	// Merge the two sorted range together (stable sort).
	inplace_merge(m_keys.begin(), iter, m_keys.end());
	// Deduplicate elements post-merge.
	iter = unique(m_keys.begin(), m_keys.end(), make_equal());
	m_keys.erase(iter, m_keys.end());
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename InputIterator>
void flat_set<Key, Compare, KeyContainer>::insert(const sorted_unique_t, InputIterator first, const InputIterator last)
{
	using std::inplace_merge;
	using std::unique;
	// [first, iter) and [iter, end) will both be sorted per the sorted_unique tag.
	auto iter = m_keys.insert(m_keys.end(), first, last);
	// Merge the two sorted range together (stable sort).
	inplace_merge(m_keys.begin(), iter, m_keys.end());
	// Deduplicate elements post-merge.
	iter = unique(m_keys.begin(), m_keys.end(), make_equal());
	m_keys.erase(iter, m_keys.end());
}
template <typename Key, typename Compare, typename KeyContainer>
void flat_set<Key, Compare, KeyContainer>::insert(const std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	insert(begin(init), end(init));
}
template <typename Key, typename Compare, typename KeyContainer>
void flat_set<Key, Compare, KeyContainer>::insert(const sorted_unique_t, std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	insert(sorted_unique, begin(init), end(init));
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::extract() &&
	-> container_type
{
	return std::move(m_keys);
}
template <typename Key, typename Compare, typename KeyContainer>
void flat_set<Key, Compare, KeyContainer>::replace(container_type&& cont)
{
	m_keys = std::move(cont);
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::erase(const const_iterator pos)
	-> iterator
{
	return m_keys.erase(pos);
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::erase(const const_iterator first, const const_iterator last)
	-> iterator
{
	return m_keys.erase(first, last);
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::erase(const key_type& key_arg)
	-> size_type
{
	return this->erase<const key_type&, void, void>(key_arg);
}
template <typename Key, typename Compare, typename KeyContainer>
void flat_set<Key, Compare, KeyContainer>::swap(flat_set& other) noexcept
{
	using std::swap;
	swap(get_less(), other.get_less());
	swap(m_keys, other.m_keys);
}
template <typename Key, typename Compare, typename KeyContainer>
void flat_set<Key, Compare, KeyContainer>::clear() noexcept
{
	m_keys.clear();
}

// Modifiers (transparent):
template <typename Key, typename Compare, typename KeyContainer>
template <typename K>
auto flat_set<Key, Compare, KeyContainer>::emplace(K&& key_arg)
	-> std::pair<iterator, bool>
{
	using std::lower_bound;
	const auto& less = get_less();
	auto iter = lower_bound(m_keys.begin(), m_keys.end(), key_arg, less);
	const bool inserted = iter == m_keys.end() || less(key_arg, *iter);
	if (inserted)
	{
		iter = m_keys.insert(iter, std::forward<K>(key_arg));
	}
	return std::make_pair(iter, inserted);
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename K>
auto flat_set<Key, Compare, KeyContainer>::emplace_hint(const const_iterator hint, K&& key_arg)
	-> iterator
{
	return emplace(std::forward<K>(key_arg)).first;
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename K, typename, typename>
auto flat_set<Key, Compare, KeyContainer>::erase(const K& key_arg)
	-> size_type
{
	const iterator it = find(key_arg);
	if (it == end())
	{
		return 0;
	}
	m_keys.erase(it);
	return 1;
}

// Modifiers (extensions):
template <typename Key, typename Compare, typename KeyContainer>
void flat_set<Key, Compare, KeyContainer>::reserve(const size_type n)
{
	m_keys.reserve(n);
}
template <typename Key, typename Compare, typename KeyContainer>
void flat_set<Key, Compare, KeyContainer>::shrink_to_fit()
{
	m_keys.shrink_to_fit();
}

// Lookup:
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::find(const key_type& key_arg)
	 -> iterator
{
	return this->find<const key_type&, void, void>(key_arg);
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::find(const key_type& key_arg) const
	-> const_iterator
{
	return this->find<const key_type&, void, void>(key_arg);
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::count(const key_type& key_arg) const
	-> size_type
{
	return this->contains<const key_type&, void, void>(key_arg);
}
template <typename Key, typename Compare, typename KeyContainer>
bool flat_set<Key, Compare, KeyContainer>::contains(const key_type& key_arg) const
{
	return this->contains<const key_type&, void, void>(key_arg);
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::lower_bound(const key_type& key_arg)
	-> iterator
{
	return this->lower_bound<const key_type&, void, void>(key_arg);
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::lower_bound(const key_type& key_arg) const
	-> const_iterator
{
	return this->lower_bound<const key_type&, void, void>(key_arg);
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::upper_bound(const key_type& key_arg)
	-> iterator
{
	return this->upper_bound<const key_type&, void, void>(key_arg);
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::upper_bound(const key_type& key_arg) const
	-> const_iterator
{
	return this->upper_bound<const key_type&, void, void>(key_arg);
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::equal_range(const key_type& key_arg)
	-> std::pair<iterator, iterator>
{
	return this->equal_range<const key_type&, void, void>(key_arg);
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::equal_range(const key_type& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	return this->equal_range<const key_type&, void, void>(key_arg);
}

// Lookup (transparent):
template <typename Key, typename Compare, typename KeyContainer>
template <typename K, typename, typename>
auto flat_set<Key, Compare, KeyContainer>::find(const K& key_arg)
	 -> iterator
{
	return do_find(key_arg, m_keys.begin(), m_keys.end());
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename K, typename, typename>
auto flat_set<Key, Compare, KeyContainer>::find(const K& key_arg) const
	-> const_iterator
{
	return do_find(key_arg, m_keys.begin(), m_keys.end());
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename K, typename, typename>
auto flat_set<Key, Compare, KeyContainer>::count(const K& key_arg) const
	-> size_type
{
	return find(key_arg) != m_keys.end() ? 1 : 0;
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename K, typename, typename>
bool flat_set<Key, Compare, KeyContainer>::contains(const K& key_arg) const
{
	return find(key_arg) != m_keys.end();
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename K, typename, typename>
auto flat_set<Key, Compare, KeyContainer>::lower_bound(const K& key_arg)
	-> iterator
{
	using std::lower_bound;
	return lower_bound(m_keys.begin(), m_keys.end(), key_arg, get_less());
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename K, typename, typename>
auto flat_set<Key, Compare, KeyContainer>::lower_bound(const K& key_arg) const
	-> const_iterator
{
	using std::lower_bound;
	return lower_bound(m_keys.begin(), m_keys.end(), key_arg, get_less());
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename K, typename, typename>
auto flat_set<Key, Compare, KeyContainer>::upper_bound(const K& key_arg)
	-> iterator
{
	using std::upper_bound;
	return upper_bound(m_keys.begin(), m_keys.end(), key_arg, get_less());
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename K, typename, typename>
auto flat_set<Key, Compare, KeyContainer>::upper_bound(const K& key_arg) const
	-> const_iterator
{
	using std::upper_bound;
	return upper_bound(m_keys.begin(), m_keys.end(), key_arg, get_less());
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename K, typename, typename>
auto flat_set<Key, Compare, KeyContainer>::equal_range(const K& key_arg)
	-> std::pair<iterator, iterator>
{
	const iterator iter = find(key_arg);
	return std::make_pair(iter, iter == m_keys.end() ? iter : std::next(iter));
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename K, typename, typename>
auto flat_set<Key, Compare, KeyContainer>::equal_range(const K& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	const const_iterator iter = find(key_arg);
	return std::make_pair(iter, iter == m_keys.end() ? iter : std::next(iter));
}

// Observers:
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::key_comp() const
	-> key_compare
{
	return get_less();
}
template <typename Key, typename Compare, typename KeyContainer>
auto flat_set<Key, Compare, KeyContainer>::value_comp() const
	-> value_compare
{
	return get_less();
}

// Non-member functions:
template <typename Key, typename Compare, typename KeyContainer>
bool operator==(const flat_set<Key, Compare, KeyContainer>& lhs, const flat_set<Key, Compare, KeyContainer>& rhs)
{
	return lhs.m_keys == rhs.m_keys;
}
template <typename Key, typename Compare, typename KeyContainer>
bool operator!=(const flat_set<Key, Compare, KeyContainer>& lhs, const flat_set<Key, Compare, KeyContainer>& rhs)
{
	return operator==(lhs, rhs) == false;
}

template <typename Key, typename Compare, typename KeyContainer>
template <typename Iterator>
auto flat_set<Key, Compare, KeyContainer>::do_find(const key_type& key_arg, const Iterator first, const Iterator last) const
	 -> Iterator
{
	const auto& less = get_less();
	using std::lower_bound;
	const auto iter = lower_bound(first, last, key_arg, less);
	return iter != last && less(key_arg, *iter) == false
		? iter
		: last;
}
template <typename Key, typename Compare, typename KeyContainer>
template <typename K, typename Iterator, typename, typename>
auto flat_set<Key, Compare, KeyContainer>::do_find(const K& key_arg, const Iterator first, const Iterator last) const
	-> Iterator
{
	const auto& less = get_less();
	using std::lower_bound;
	const auto iter = lower_bound(first, last, key_arg, less);
	return iter != last && less(key_arg, *iter) == false
		? iter
		: last;
}
template <typename Key, typename Compare, typename KeyContainer>
void flat_set<Key, Compare, KeyContainer>::sort_containers_and_erase_duplicates()
{
	using std::sort;
	using std::unique;
	sort(m_keys.begin(), m_keys.end(), get_less());
	const auto iter = unique(m_keys.begin(), m_keys.end(), make_equal());
	m_keys.erase(iter, m_keys.end());
}

template <typename Key, typename Compare, typename KeyContainer>
constexpr auto flat_set<Key, Compare, KeyContainer>::get_less() noexcept
	-> key_compare&
{
	return *this;
}
template <typename Key, typename Compare, typename KeyContainer>
constexpr auto flat_set<Key, Compare, KeyContainer>::get_less() const noexcept
	-> const key_compare&
{
	return *this;
}
template <typename Key, typename Compare, typename KeyContainer>
constexpr auto flat_set<Key, Compare, KeyContainer>::make_equal() const noexcept
{
	return [this](const auto& key_arg1, const auto& key_arg2)
	{
		const auto& less = get_less();
		return less(key_arg1, key_arg2) == false && less(key_arg2, key_arg1) == false;
	};
}

} // namespace sh

#endif
