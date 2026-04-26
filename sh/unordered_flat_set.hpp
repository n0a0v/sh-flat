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

#ifndef INC_SH__UNORDERED_FLAT_SET_HPP
#define INC_SH__UNORDERED_FLAT_SET_HPP

#include "flat_algorithm.hpp"

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>

namespace sh
{

template <typename Key,
	typename KeyEqual = std::equal_to<Key>,
	typename KeyContainer = std::vector<Key>>
class unordered_flat_set : private KeyEqual
{
public:
	using container_type = KeyContainer;
	using key_type = Key;
	using value_type = Key;
	using key_equal = KeyEqual;
	using value_equal = KeyEqual;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = typename KeyContainer::size_type;
	using difference_type = typename KeyContainer::difference_type;
	using iterator = flat::const_iterator_t<container_type>;
	using const_iterator = iterator;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	static_assert(std::is_same_v<Key, typename std::iterator_traits<flat::iterator_t<container_type>>::value_type>);

	// Member functions:
	unordered_flat_set();
	unordered_flat_set(const unordered_flat_set& other);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		const unordered_flat_set& other,
		const Allocator& alloc);
	unordered_flat_set(unordered_flat_set&& other) noexcept;
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		unordered_flat_set&& other,
		const Allocator& alloc);
	explicit unordered_flat_set(
		container_type cont,
		const key_equal& eq = key_equal{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		const container_type& cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		const container_type& cont,
		const key_equal& eq,
		const Allocator& alloc);
	unordered_flat_set(
		unsorted_unique_t,
		container_type cont,
		const key_equal& eq = key_equal{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		unsorted_unique_t,
		const container_type& cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		unsorted_unique_t,
		const container_type& cont,
		const key_equal& eq,
		const Allocator& alloc);
	explicit unordered_flat_set(const key_equal& eq);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(const key_equal& eq, const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	explicit unordered_flat_set(const Allocator& alloc);
	template <typename InputIterator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category
	>
	unordered_flat_set(
		InputIterator first,
		InputIterator last,
		const key_equal& eq = key_equal{});
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		InputIterator first,
		InputIterator last,
		const key_equal& eq,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		InputIterator first,
		InputIterator last,
		const Allocator& alloc);
	template <typename InputIterator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category
	>
	unordered_flat_set(
		unsorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_equal& eq = key_equal{});
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		unsorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_equal& eq,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		unsorted_unique_t,
		InputIterator first,
		InputIterator last,
		const Allocator& alloc);
	unordered_flat_set(
		std::initializer_list<value_type> init,
		const key_equal& eq = key_equal{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		std::initializer_list<value_type> init,
		const key_equal& eq,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	unordered_flat_set(unsorted_unique_t,
		std::initializer_list<value_type> init,
		const key_equal& eq = key_equal{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		unsorted_unique_t,
		std::initializer_list<value_type> init,
		const key_equal& eq,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_flat_set(
		unsorted_unique_t,
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	~unordered_flat_set() = default;

	unordered_flat_set& operator=(const unordered_flat_set& other);
	unordered_flat_set& operator=(unordered_flat_set&& other) noexcept;

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
	template <typename InputIterator> void insert(unsorted_unique_t, InputIterator first, InputIterator last);
	void insert(std::initializer_list<value_type> init);
	void insert(unsorted_unique_t, std::initializer_list<value_type> init);
	container_type extract() &&;
	void replace(container_type&& cont);
	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);
	size_type erase(const key_type& key_arg);
	void swap(unordered_flat_set& other) noexcept;
	void clear() noexcept;

	// Modifiers (transparent):
	template <typename... Args>
	std::pair<iterator, bool> emplace(Args&&... args);
	template <typename... Args>
	iterator emplace_hint(const_iterator hint, Args&&... args);
	template <typename K,
		typename C = key_equal,
		typename IsTransparentAndNotIterator = std::enable_if_t<
			false == std::is_convertible_v<K&&, const_iterator> && false == std::is_convertible_v<K&&, iterator>,
			typename C::is_transparent
		>
	>
	size_type erase(const K& key_arg);

	// Modifiers (extensions):
	template <
		typename KC = container_type,
		typename HasReserve = std::enable_if_t<flat::has_reserve_v<KC>>
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
	template <typename K, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	iterator find(const K& key_arg);
	template <typename K, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	const_iterator find(const K& key_arg) const;
	template <typename K, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	size_type count(const K& key_arg) const;
	template <typename K, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	bool contains(const K& key_arg) const;
	template <typename K, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	iterator lower_bound(const K& key_arg);
	template <typename K, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	const_iterator lower_bound(const K& key_arg) const;
	template <typename K, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	iterator upper_bound(const K& key_arg);
	template <typename K, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	const_iterator upper_bound(const K& key_arg) const;
	template <typename K, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	std::pair<iterator, iterator> equal_range(const K& key_arg);
	template <typename K, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	std::pair<const_iterator, const_iterator> equal_range(const K& key_arg) const;

	// Observers:
	key_equal key_eq() const;
	value_equal value_eq() const;

	// Observers (extensions):
	const container_type& keys() const;

	// Non-member functions:
	template <typename Key2, typename KeyEqual2, typename KeyContainer2>
	friend bool operator==(const unordered_flat_set<Key2, KeyEqual2, KeyContainer2>& lhs, const unordered_flat_set<Key2, KeyEqual2, KeyContainer2>& rhs);
	template <typename Key2, typename KeyEqual2, typename KeyContainer2>
	friend bool operator!=(const unordered_flat_set<Key2, KeyEqual2, KeyContainer2>& lhs, const unordered_flat_set<Key2, KeyEqual2, KeyContainer2>& rhs);

private:
	template <typename Iterator>
	Iterator do_find(const key_type& key_arg, const Iterator first, const Iterator last) const;
	template <typename K, typename Iterator, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	Iterator do_find(const K& key_arg, const Iterator first, const Iterator last) const;
	template <typename K>
	std::pair<iterator, bool> do_transparent_emplace_back_if_unique(K&& key_arg);

	constexpr key_equal& get_equal() noexcept;
	constexpr const key_equal& get_equal() const noexcept;

	container_type m_keys;
};

// Member functions:
template <typename Key, typename KeyEqual, typename KeyContainer>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set()
	: unordered_flat_set{ key_equal{} }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(const unordered_flat_set& other)
	: key_equal{ other.get_equal() }
	, m_keys{ other.m_keys }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(const unordered_flat_set& other, const Allocator& alloc)
	: key_equal{ other.get_equal() }
	, m_keys{ other.m_keys, alloc }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(unordered_flat_set&& other) noexcept
	: key_equal{ std::move(other.get_equal()) }
	, m_keys{ std::move(other.m_keys) }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(unordered_flat_set&& other, const Allocator& alloc)
	: key_equal{ std::move(other.get_equal()) }
	, m_keys{ std::move(other.m_keys), alloc }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(container_type cont, const key_equal& eq)
	: key_equal{ eq }
	, m_keys{ }
{
	using std::begin;
	using std::end;
	// Use insert rather than copying as it will de-duplicate (the slow way).
	this->insert(begin(cont), end(cont));
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(const container_type& cont, const Allocator& alloc)
	: key_equal{}
	, m_keys{ alloc }
{
	using std::begin;
	using std::end;
	// Use insert rather than copying as it will de-duplicate (the slow way).
	this->insert(begin(cont), end(cont));
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(const container_type& cont, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ alloc }
{
	using std::begin;
	using std::end;
	// Use insert rather than copying as it will de-duplicate (the slow way).
	this->insert(begin(cont), end(cont));
}
template <typename Key, typename KeyEqual, typename KeyContainer>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(unsorted_unique_t, container_type cont, const key_equal& eq)
	: key_equal{ eq }
	, m_keys{ std::move(cont) }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(unsorted_unique_t, const container_type& cont, const Allocator& alloc)
	: key_equal{}
	, m_keys{ cont, alloc }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(unsorted_unique_t, const container_type& cont, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ cont, alloc }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(const key_equal& eq)
	: key_equal{ eq }
	, m_keys{ }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ alloc }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(const Allocator& alloc)
	: key_equal{}
	, m_keys{ alloc }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename InputIterator, typename HasIteratorCategory>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(const InputIterator first, const InputIterator last, const key_equal& eq)
	: key_equal{ eq }
	, m_keys{}
{
	// Use insert to de-duplicate (the slow way).
	this->insert(first, last);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(const InputIterator first, const InputIterator last, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ alloc }
{
	// Use insert to de-duplicate (the slow way).
	this->insert(first, last);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(const InputIterator first, const InputIterator last, const Allocator& alloc)
	: key_equal{}
	, m_keys{ alloc }
{
	// Use insert to de-duplicate (the slow way).
	this->insert(first, last);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename InputIterator, typename HasIteratorCategory>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(unsorted_unique_t, const InputIterator first, const InputIterator last, const key_equal& eq)
	: key_equal{ eq }
	, m_keys{ first, last }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(unsorted_unique_t, const InputIterator first, const InputIterator last, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ first, last, alloc }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(unsorted_unique_t, const InputIterator first, const InputIterator last, const Allocator& alloc)
	: key_equal{}
	, m_keys{ first, last, alloc }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(std::initializer_list<value_type> init, const key_equal& eq)
	: key_equal{ eq }
	, m_keys{}
{
	using std::begin;
	using std::end;
	// Use insert to de-duplicate (the slow way).
	this->insert(begin(init), end(init));
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(std::initializer_list<value_type> init, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ alloc }
{
	using std::begin;
	using std::end;
	// Use insert to de-duplicate (the slow way).
	this->insert(begin(init), end(init));
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(std::initializer_list<value_type> init, const Allocator& alloc)
	: key_equal{}
	, m_keys{ alloc }
{
	using std::begin;
	using std::end;
	// Use insert to de-duplicate (the slow way).
	this->insert(begin(init), end(init));
}
template <typename Key, typename KeyEqual, typename KeyContainer>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(unsorted_unique_t, std::initializer_list<value_type> init, const key_equal& eq)
	: key_equal{ eq }
	, m_keys{ std::move(init) }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(unsorted_unique_t, std::initializer_list<value_type> init, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ std::move(init), alloc }
{ }
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_set<Key, KeyEqual, KeyContainer>::unordered_flat_set(unsorted_unique_t, std::initializer_list<value_type> init, const Allocator& alloc)
	: key_equal{}
	, m_keys{ std::move(init), alloc }
{ }

template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::operator=(const unordered_flat_set& other)
	-> unordered_flat_set&
{
	this->get_equal() = other.get_equal();
	m_keys = other.m_keys;
	return *this;
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::operator=(unordered_flat_set&& other) noexcept
	-> unordered_flat_set&
{
	this->get_equal() = std::move(other.get_equal());
	m_keys = std::move(other.m_keys);
	return *this;
}

// Iterators:
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::begin() noexcept
	-> iterator
{
	using std::begin;
	return begin(m_keys);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::end() noexcept
	-> iterator
{
	using std::end;
	return end(m_keys);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::begin() const noexcept
	-> const_iterator
{
	using std::cbegin;
	return cbegin(m_keys);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::end() const noexcept
	-> const_iterator
{
	using std::cend;
	return cend(m_keys);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::cbegin() const noexcept
	-> const_iterator
{
	using std::cbegin;
	return cbegin(m_keys);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::cend() const noexcept
	-> const_iterator
{
	using std::cend;
	return cend(m_keys);
}

template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::rbegin() noexcept
	-> reverse_iterator
{
	using std::rbegin;
	return rbegin(m_keys);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::rend() noexcept
	-> reverse_iterator
{
	using std::rend;
	return rend(m_keys);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::rbegin() const noexcept
	-> const_reverse_iterator
{
	using std::crbegin;
	return crbegin(m_keys);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::rend() const noexcept
	-> const_reverse_iterator
{
	using std::crend;
	return crend(m_keys);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::crbegin() const noexcept
	-> const_reverse_iterator
{
	using std::crbegin;
	return crbegin(m_keys);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::crend() const noexcept
	-> const_reverse_iterator
{
	using std::crend;
	return crend(m_keys);
}

// Capacity:
template <typename Key, typename KeyEqual, typename KeyContainer>
bool unordered_flat_set<Key, KeyEqual, KeyContainer>::empty() const noexcept
{
	return m_keys.empty();
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::size() const noexcept
	-> size_type
{
	return m_keys.size();
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::max_size() const noexcept
	-> size_type
{
	return m_keys.max_size();
}

// Modifiers:
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::insert(const value_type& value)
	-> std::pair<iterator, bool>
{
	return this->emplace(value);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::insert(value_type&& value)
	-> std::pair<iterator, bool>
{
	return this->emplace(std::move(value));
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::insert(const const_iterator hint, const value_type& value)
	-> iterator
{
	return this->emplace_hint(hint, value);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::insert(const const_iterator hint, value_type&& value)
	-> iterator
{
	return this->emplace_hint(hint, std::move(value));
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename InputIterator>
void unordered_flat_set<Key, KeyEqual, KeyContainer>::insert(InputIterator first, const InputIterator last)
{
	// Deduplicate the slow way, by checking each element against both the
	// preexisting & newly inserted elements.
	while (first != last)
	{
		this->emplace(*(first++));
	}
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename InputIterator>
void unordered_flat_set<Key, KeyEqual, KeyContainer>::insert(const unsorted_unique_t, InputIterator first, const InputIterator last)
{
	using std::begin;
	using std::next;
	// Deduplicate the (fairly) slow way, by checking each element against the
	// preexisting elements but not newly inserted elements, as they're tagged
	// unsorted_unique.
	if constexpr (
		std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>
		&& flat::has_reserve_v<unordered_flat_set<Key, KeyEqual, KeyContainer>>
	)
	{
		using std::distance;
		// This intentionally ignores preexisting elements as a rough heuristic
		// to not over allocate when keys are duplicated. When pre_insert_size
		// is zero, this will reserve enough for [first, last).
		this->reserve(distance(first, last));
	}
	const size_type pre_insert_size{ m_keys.size() };
	while (first != last)
	{
		const flat::iterator_t<container_type> last = next(begin(m_keys), pre_insert_size);
		if (do_find(*first, begin(m_keys), last) == last)
		{
			m_keys.emplace_back(*first);
		}
		++first;
	}
}
template <typename Key, typename KeyEqual, typename KeyContainer>
void unordered_flat_set<Key, KeyEqual, KeyContainer>::insert(std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	this->insert(begin(init), end(init));
}
template <typename Key, typename KeyEqual, typename KeyContainer>
void unordered_flat_set<Key, KeyEqual, KeyContainer>::insert(const unsorted_unique_t, std::initializer_list<value_type> init)
{
	this->insert(std::move(init));
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::extract() &&
	-> container_type
{
	return std::move(m_keys);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
void unordered_flat_set<Key, KeyEqual, KeyContainer>::replace(container_type&& cont)
{
	m_keys = std::move(cont);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::erase(const const_iterator pos)
	-> iterator
{
	{
		using std::begin;
		using std::cbegin;
		using std::distance;
		using std::end;
		using std::next;
		const difference_type pos_index{ distance(cbegin(m_keys), pos) };
		const flat::iterator_t<container_type> pos_iter = next(begin(m_keys), pos_index);
		if (next(pos_iter) != end(m_keys))
		{
			*pos_iter = std::move(m_keys.back());
			m_keys.pop_back();
			return iterator{ pos_iter };
		}
	}
	m_keys.pop_back();
	return this->end();
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::erase(const const_iterator first, const const_iterator last)
	-> iterator
{
	return m_keys.erase(first, last);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::erase(const key_type& key_arg)
	-> size_type
{
	using coopt_transparent = void;
	return this->erase<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
void unordered_flat_set<Key, KeyEqual, KeyContainer>::swap(unordered_flat_set& other) noexcept
{
	using std::swap;
	swap(this->get_equal(), other.get_equal());
	swap(m_keys, other.m_keys);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
void unordered_flat_set<Key, KeyEqual, KeyContainer>::clear() noexcept
{
	m_keys.clear();
}

// Modifiers (transparent):
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename... Args>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::emplace(Args&&... args)
	-> std::pair<iterator, bool>
{
	if constexpr (sizeof...(Args) == 1u)
	{
		if constexpr (std::is_convertible_v<Args&&..., key_type>
			|| (flat::has_is_transparent_v<key_equal> && std::is_invocable_v<key_equal, Args&&...>)
		)
		{
			return this->do_transparent_emplace_back_if_unique(std::forward<Args>(args)...);
		}
		else
		{
			return this->do_transparent_emplace_back_if_unique(value_type(std::forward<Args>(args)...));
		}
	}
	else
	{
		return this->do_transparent_emplace_back_if_unique(value_type(std::forward<Args>(args)...));
	}
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename... Args>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::emplace_hint([[maybe_unused]] const const_iterator hint, Args&&... args)
	-> iterator
{
	return this->emplace(std::forward<Args>(args)...).first;
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename K, typename C, typename IsTransparentAndNotIterator>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::erase(const K& key_arg)
	-> size_type
{
	using std::begin;
	using std::end;
	using std::next;
	const flat::iterator_t<container_type> end_iter = end(m_keys);
	const flat::iterator_t<container_type> iter = this->do_find(key_arg, begin(m_keys), end_iter);
	if (iter == end_iter)
	{
		return 0;
	}
	if (next(iter) != end_iter)
	{
		*iter = std::move(m_keys.back());
	}
	m_keys.pop_back();
	return 1;
}

// Modifiers (extensions):
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename KC, typename HasReserve>
void unordered_flat_set<Key, KeyEqual, KeyContainer>::reserve(const size_type n)
{
	m_keys.reserve(n);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
void unordered_flat_set<Key, KeyEqual, KeyContainer>::shrink_to_fit()
{
	m_keys.shrink_to_fit();
}

// Lookup:
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::find(const key_type& key_arg)
	 -> iterator
{
	using coopt_transparent = void;
	return this->find<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::find(const key_type& key_arg) const
	-> const_iterator
{
	using coopt_transparent = void;
	return this->find<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::count(const key_type& key_arg) const
	-> size_type
{
	using coopt_transparent = void;
	return this->count<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
bool unordered_flat_set<Key, KeyEqual, KeyContainer>::contains(const key_type& key_arg) const
{
	using coopt_transparent = void;
	return this->contains<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::lower_bound(const key_type& key_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->lower_bound<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::lower_bound(const key_type& key_arg) const
	-> const_iterator
{
	using coopt_transparent = void;
	return this->lower_bound<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::upper_bound(const key_type& key_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->upper_bound<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::upper_bound(const key_type& key_arg) const
	-> const_iterator
{
	using coopt_transparent = void;
	return this->upper_bound<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::equal_range(const key_type& key_arg)
	-> std::pair<iterator, iterator>
{
	using coopt_transparent = void;
	return this->equal_range<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::equal_range(const key_type& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	using coopt_transparent = void;
	return this->equal_range<const key_type&, key_equal, coopt_transparent>(key_arg);
}

// Lookup (transparent):
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename K, typename C, typename IsTransparent>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::find(const K& key_arg)
	 -> iterator
{
	using std::begin;
	using std::end;
	return this->do_find(key_arg, begin(m_keys), end(m_keys));
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename K, typename C, typename IsTransparent>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::find(const K& key_arg) const
	-> const_iterator
{
	using std::begin;
	using std::end;
	return this->do_find(key_arg, begin(m_keys), end(m_keys));
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename K, typename C, typename IsTransparent>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::count(const K& key_arg) const
	-> size_type
{
	using std::end;
	return this->find(key_arg) != end(m_keys) ? 1 : 0;
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename K, typename C, typename IsTransparent>
bool unordered_flat_set<Key, KeyEqual, KeyContainer>::contains(const K& key_arg) const
{
	using std::end;
	return this->find(key_arg) != end(m_keys);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename K, typename C, typename IsTransparent>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::equal_range(const K& key_arg)
	-> std::pair<iterator, iterator>
{
	using std::end;
	using std::next;
	const iterator iter = this->find(key_arg);
	return { iter, iter == end(m_keys) ? iter : next(iter) };
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename K, typename C, typename IsTransparent>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::equal_range(const K& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	using std::end;
	using std::next;
	const const_iterator iter = this->find(key_arg);
	return { iter, iter == end(m_keys) ? iter : next(iter) };
}

// Observers:
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::key_eq() const
	-> key_equal
{
	return this->get_equal();
}
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::value_eq() const
	-> value_equal
{
	return this->get_equal();
}

// Observers (extensions):
template <typename Key, typename KeyEqual, typename KeyContainer>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::keys() const
	-> const container_type&
{
	return m_keys;
}

// Non-member functions:
template <typename Key, typename KeyEqual, typename KeyContainer>
bool operator==(const unordered_flat_set<Key, KeyEqual, KeyContainer>& lhs, const unordered_flat_set<Key, KeyEqual, KeyContainer>& rhs)
{
	if (lhs.size() != rhs.size())
	{
		return false;
	}
	for (typename unordered_flat_set<Key, KeyEqual, KeyContainer>::const_reference key : lhs)
	{
		if (rhs.contains(key) == false)
		{
			return false;
		}
	}
	return true;
}
template <typename Key, typename KeyEqual, typename KeyContainer>
bool operator!=(const unordered_flat_set<Key, KeyEqual, KeyContainer>& lhs, const unordered_flat_set<Key, KeyEqual, KeyContainer>& rhs)
{
	return operator==(lhs, rhs) == false;
}

template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename Iterator>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::do_find(const key_type& key_arg, const Iterator first, const Iterator last) const
	-> Iterator
{
	using coopt_transparent = void;
	return this->do_find<const key_type&, Iterator, key_equal, coopt_transparent>(key_arg, first, last);
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename K, typename Iterator, typename C, typename IsTransparent>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::do_find(const K& key_arg, const Iterator first, const Iterator last) const
	-> Iterator
{
	using std::find_if;
	const key_equal& equal = this->get_equal();
	return find_if(first, last,
		[&key_arg, &equal](const key_type& key)
	{
		return equal(key_arg, key);
	});
}
template <typename Key, typename KeyEqual, typename KeyContainer>
template <typename K>
auto unordered_flat_set<Key, KeyEqual, KeyContainer>::do_transparent_emplace_back_if_unique(K&& key_arg)
	-> std::pair<iterator, bool>
{
	using std::end;
	using std::prev;
	iterator iter = this->find(key_arg);
	const bool inserted = iter == end(m_keys);
	if (inserted)
	{
		m_keys.emplace_back(std::forward<K>(key_arg));
		iter = prev(end(m_keys));
	}
	return { iter, inserted };
}

template <typename Key, typename KeyEqual, typename KeyContainer>
constexpr auto unordered_flat_set<Key, KeyEqual, KeyContainer>::get_equal() noexcept
	-> key_equal&
{
	return *this;
}
template <typename Key, typename KeyEqual, typename KeyContainer>
constexpr auto unordered_flat_set<Key, KeyEqual, KeyContainer>::get_equal() const noexcept
	-> const key_equal&
{
	return *this;
}

} // namespace sh

#endif
