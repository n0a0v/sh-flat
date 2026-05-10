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

#ifndef INC_SH__UNORDERED_FLAT_MAP_HPP
#define INC_SH__UNORDERED_FLAT_MAP_HPP

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
	typename KeyEqual = std::equal_to<Key>,
	typename KeyContainer = std::vector<Key>,
	typename MappedContainer = std::vector<T>>
class unordered_flat_map : private KeyEqual
{
public:
	using key_container_type = KeyContainer;
	using mapped_container_type = MappedContainer;
	using key_type = Key;
	using mapped_type = T;
	using value_type = std::pair<key_type, mapped_type>;
	using key_equal = KeyEqual;
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
	struct value_equal : private key_equal
	{
		explicit value_equal(const key_equal& eq)
			: key_equal{ eq }
		{ }
		bool operator()(const const_reference& lhs, const const_reference& rhs) const
		{
			using std::get;
			return this->key_equal::operator()(get<0>(lhs), get<0>(rhs));
		}
	};

	// Member functions:
	unordered_flat_map();
	unordered_flat_map(const unordered_flat_map& other);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		const unordered_flat_map& other,
		const Allocator& alloc);
	unordered_flat_map(unordered_flat_map&& other) noexcept;
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		unordered_flat_map&& other,
		const Allocator& alloc);
	unordered_flat_map(
		key_container_type key_cont,
		mapped_container_type mapped_cont,
		const key_equal& eq = key_equal{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		const key_container_type& key_cont,
		const mapped_container_type& mapped_cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		const key_container_type& key_cont,
		const mapped_container_type& mapped_cont,
		const key_equal& eq,
		const Allocator& alloc);
	unordered_flat_map(
		unsorted_unique_t,
		const key_container_type& key_cont,
		const mapped_container_type& mapped_cont);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		unsorted_unique_t,
		const key_container_type& key_cont,
		const mapped_container_type& mapped_cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		unsorted_unique_t,
		const key_container_type& key_cont,
		const mapped_container_type& mapped_cont,
		const key_equal& eq,
		const Allocator& alloc);
	explicit unordered_flat_map(const key_equal& eq);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(const key_equal& eq, const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	explicit unordered_flat_map(const Allocator& alloc);
	template <typename InputIterator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category
	>
	unordered_flat_map(
		InputIterator first,
		InputIterator last,
		const key_equal& eq = key_equal{});
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		InputIterator first,
		InputIterator last,
		const key_equal& eq,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		InputIterator first,
		InputIterator last,
		const Allocator& alloc);
	template <typename InputIterator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category
	>
	unordered_flat_map(
		unsorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_equal& eq = key_equal{});
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		unsorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_equal& eq,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		unsorted_unique_t,
		InputIterator first,
		InputIterator last,
		const Allocator& alloc);
	unordered_flat_map(
		std::initializer_list<value_type> init,
		 const key_equal& eq = key_equal{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		std::initializer_list<value_type> init,
		const key_equal& eq,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	unordered_flat_map(unsorted_unique_t,
		std::initializer_list<value_type> init,
		const key_equal& eq = key_equal{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		unsorted_unique_t,
		std::initializer_list<value_type> init,
		const key_equal& eq,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<key_container_type, Allocator> && std::uses_allocator_v<mapped_container_type, Allocator>>
	>
	unordered_flat_map(
		unsorted_unique_t,
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	~unordered_flat_map() = default;

	unordered_flat_map& operator=(const unordered_flat_map& other);
	unordered_flat_map& operator=(unordered_flat_map&& other) noexcept;

	// Element access:
	mapped_type& at(const key_type& key_arg);
	const mapped_type& at(const key_type& key_arg) const;
	mapped_type& operator[](const key_type& key_arg);
	mapped_type& operator[](key_type&& key_arg);

	// Element access (transparent):
	template <typename K, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	mapped_type& at(const K& key_arg);
	template <typename K, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	const mapped_type& at(const K& key_arg) const;
	template <typename K, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
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
	void insert(unsorted_unique_t, InputIterator first, InputIterator last);
	void insert(std::initializer_list<value_type> init);
	void insert(unsorted_unique_t, std::initializer_list<value_type> init);
	containers extract() &&;
	void replace(containers&& cont);
	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);
	size_type erase(const key_type& key_arg);
	void swap(unordered_flat_map& other) noexcept(std::is_nothrow_swappable_v<key_equal>);
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
		typename C = key_equal,
		typename IsTransparentAndNotIterator = std::enable_if_t<
			false == std::is_convertible_v<K&&, const_iterator> && false == std::is_convertible_v<K&&, iterator>,
			typename C::is_transparent
		>
	>
	std::pair<iterator, bool> try_emplace(K&& key_arg, Args&&... args);
	template <typename K, typename... Args,
		typename C = key_equal,
		typename IsTransparentAndNotIterator = std::enable_if_t<
			false == std::is_convertible_v<K&&, const_iterator> && false == std::is_convertible_v<K&&, iterator>,
			typename C::is_transparent
		>
	>
	iterator try_emplace(const_iterator hint, K&& key_arg, Args&&... args);
	template <typename K, typename M, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	std::pair<iterator, bool> insert_or_assign(K&& key_arg, M&& mapped_arg);
	template <typename K, typename M, typename C = key_equal, typename IsTransparent = typename C::is_transparent>
	iterator insert_or_assign(const_iterator hint, K&& key_arg, M&& mapped_arg);
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
	const key_container_type& keys() const;
	const mapped_container_type& values() const;

	// Non-member functions:
	template <typename Key2, typename T2, typename KeyEqual2, typename KeyContainer2, typename MappedContainer2>
	friend bool operator==(
		const unordered_flat_map<Key2, T2, KeyEqual2, KeyContainer2, MappedContainer2>& lhs,
		const unordered_flat_map<Key2, T2, KeyEqual2, KeyContainer2, MappedContainer2>& rhs);
	template <typename Key2, typename T2, typename KeyEqual2, typename KeyContainer2, typename MappedContainer2>
	friend bool operator!=(
		const unordered_flat_map<Key2, T2, KeyEqual2, KeyContainer2, MappedContainer2>& lhs,
		const unordered_flat_map<Key2, T2, KeyEqual2, KeyContainer2, MappedContainer2>& rhs);

private:
	template <typename KeyIterator>
	KeyIterator do_find(const key_type& key_arg, const KeyIterator first, const KeyIterator last) const;
	template <typename K,
		typename KeyIterator,
		typename C = key_equal,
		typename IsTransparent = typename C::is_transparent>
	KeyIterator do_find(const K& key_arg, const KeyIterator first, const KeyIterator last) const;
	template <typename... Args>
	std::pair<iterator, bool> do_emplace_back_if_unique(const key_type& key_arg, Args&&... args);
	template <typename... Args>
	std::pair<iterator, bool> do_emplace_back_if_unique(key_type&& key_arg, Args&&... args);
	template <typename K,
		typename... Args>
	std::pair<iterator, bool> do_transparent_emplace_back_if_unique(K&& key_arg, Args&&... args);
	template <typename InputIterator>
	void do_insert_back_without_checking_if_unique(InputIterator first, InputIterator last);

	constexpr key_equal& get_equal() noexcept;
	constexpr const key_equal& get_equal() const noexcept;

	key_container_type m_keys;
	mapped_container_type m_values;
};

// Member functions:
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map()
	: unordered_flat_map{ key_equal{} }
{ }
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(const unordered_flat_map& other)
	: key_equal{ other.get_equal() }
	, m_keys{ other.m_keys }
	, m_values{ other.m_values }
{ }
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(const unordered_flat_map& other, const Allocator& alloc)
	: key_equal{ other.get_equal() }
	, m_keys{ other.m_keys, alloc }
	, m_values{ other.m_values, alloc }
{ }
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(unordered_flat_map&& other) noexcept
	: key_equal{ std::move(other.get_equal()) }
	, m_keys{ std::move(other.m_keys) }
	, m_values{ std::move(other.m_values) }
{ }
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(unordered_flat_map&& other, const Allocator& alloc)
	: key_equal{ std::move(other.get_equal()) }
	, m_keys{ std::move(other.m_keys), alloc }
	, m_values{ std::move(other.m_values), alloc }
{ }
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(key_container_type key_cont, mapped_container_type mapped_cont, const key_equal& eq)
	: key_equal{ eq }
	, m_keys{}
	, m_values{}
{
	using std::begin;
	using std::end;
	const flat::iterator_pair first{ begin(key_cont), begin(mapped_cont) };
	const flat::iterator_pair last{ end(key_cont), end(mapped_cont) };
	// Use insert rather than copying as it will de-duplicate (the slow way).
	this->insert(first, last);
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(const key_container_type& key_cont, const mapped_container_type& mapped_cont, const Allocator& alloc)
	: key_equal{}
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	const flat::iterator_pair first{ begin(key_cont), begin(mapped_cont) };
	const flat::iterator_pair last{ end(key_cont), end(mapped_cont) };
	// Use insert rather than copying as it will de-duplicate (the slow way).
	this->insert(first, last);
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(const key_container_type& key_cont, const mapped_container_type& mapped_cont, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	const flat::iterator_pair first{ begin(key_cont), begin(mapped_cont) };
	const flat::iterator_pair last{ end(key_cont), end(mapped_cont) };
	// Use insert rather than copying as it will de-duplicate (the slow way).
	this->insert(first, last);
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(unsorted_unique_t, const key_container_type& key_cont, const mapped_container_type& mapped_cont)
	: key_equal{}
	, m_keys{ key_cont }
	, m_values{ mapped_cont }
{
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(unsorted_unique_t, const key_container_type& key_cont, const mapped_container_type& mapped_cont, const Allocator& alloc)
	: key_equal{}
	, m_keys{ key_cont, alloc }
	, m_values{ mapped_cont, alloc }
{
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(unsorted_unique_t, const key_container_type& key_cont, const mapped_container_type& mapped_cont, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ key_cont, alloc }
	, m_values{ mapped_cont, alloc }
{
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(const key_equal& eq)
	: key_equal{ eq }
	, m_keys{ }
	, m_values{ }
{ }
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ alloc }
	, m_values{ alloc }
{ }
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(const Allocator& alloc)
	: key_equal{}
	, m_keys{ alloc }
	, m_values{ alloc }
{ }
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename HasIteratorCategory>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(const InputIterator first, const InputIterator last, const key_equal& eq)
	: key_equal{ eq }
	, m_keys{}
	, m_values{}
{
	// Use insert to de-duplicate (the slow way).
	this->insert(first, last);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(const InputIterator first, const InputIterator last, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ alloc }
	, m_values{ alloc }
{
	// Use insert to de-duplicate (the slow way).
	this->insert(first, last);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(const InputIterator first, const InputIterator last, const Allocator& alloc)
	: key_equal{}
	, m_keys{ alloc }
	, m_values{ alloc }
{
	// Use insert to de-duplicate (the slow way).
	this->insert(first, last);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename HasIteratorCategory>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(unsorted_unique_t, const InputIterator first, const InputIterator last, const key_equal& eq)
	: key_equal{ eq }
	, m_keys{}
	, m_values{}
{
	// Is unsorted_unique, insert blindly.
	this->do_insert_back_without_checking_if_unique(first, last);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(unsorted_unique_t, const InputIterator first, const InputIterator last, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ alloc }
	, m_values{ alloc }
{
	// Is unsorted_unique, insert blindly.
	this->do_insert_back_without_checking_if_unique(first, last);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(unsorted_unique_t, const InputIterator first, const InputIterator last, const Allocator& alloc)
	: key_equal{}
	, m_keys{ alloc }
	, m_values{ alloc }
{
	// Is unsorted_unique, insert blindly.
	this->do_insert_back_without_checking_if_unique(first, last);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(std::initializer_list<value_type> init, const key_equal& eq)
	: key_equal{ eq }
	, m_keys{}
	, m_values{}
{
	using std::begin;
	using std::end;
	// Use insert to de-duplicate (the slow way).
	this->insert(begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(std::initializer_list<value_type> init, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	// Use insert to de-duplicate (the slow way).
	this->insert(begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(std::initializer_list<value_type> init, const Allocator& alloc)
	: key_equal{}
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	// Use insert to de-duplicate (the slow way).
	this->insert(begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(unsorted_unique_t, std::initializer_list<value_type> init, const key_equal& eq)
	: key_equal{ eq }
	, m_keys{}
	, m_values{}
{
	using std::begin;
	using std::end;
	// Is unsorted_unique, insert blindly.
	this->do_insert_back_without_checking_if_unique(begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(unsorted_unique_t, std::initializer_list<value_type> init, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	// Is unsorted_unique, insert blindly.
	this->do_insert_back_without_checking_if_unique(begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename Allocator, typename UsesAllocator>
unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::unordered_flat_map(unsorted_unique_t, std::initializer_list<value_type> init, const Allocator& alloc)
	: key_equal{}
	, m_keys{ alloc }
	, m_values{ alloc }
{
	using std::begin;
	using std::end;
	// Is unsorted_unique, insert blindly.
	this->do_insert_back_without_checking_if_unique(begin(init), end(init));
}

template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::operator=(const unordered_flat_map& other)
	-> unordered_flat_map&
{
	this->get_equal() = other.get_equal();
	m_keys = other.m_keys;
	m_values = other.m_values;
	return *this;
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::operator=(unordered_flat_map&& other) noexcept
	-> unordered_flat_map&
{
	this->get_equal() = std::move(other.get_equal());
	m_keys = std::move(other.m_keys);
	m_values = std::move(other.m_values);
	return *this;
}

// Element access:
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::at(const key_type& key_arg)
	-> mapped_type&
{
	using coopt_transparent = void;
	return this->at<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::at(const key_type& key_arg) const
	-> const mapped_type&
{
	using coopt_transparent = void;
	return this->at<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::operator[](const key_type& key_arg)
	-> mapped_type&
{
	using coopt_transparent = void;
	return this->operator[]<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::operator[](key_type&& key_arg)
	-> mapped_type&
{
	using coopt_transparent = void;
	return this->operator[]<key_type&&, key_equal, coopt_transparent>(std::move(key_arg));
}

// Element access (transparent):
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::at(const K& key_arg)
	-> mapped_type&
{
	using std::get;
	using std::end;
	const iterator iter = this->find(key_arg);
	if (get<0>(iter) == end(m_keys))
	{
		throw std::out_of_range{ "unordered_flat_map::at" };
	}
	return *get<1>(iter);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::at(const K& key_arg) const
	-> const mapped_type&
{
	using std::get;
	using std::end;
	const iterator iter = this->find(key_arg);
	if (get<0>(iter) == end(m_keys))
	{
		throw std::out_of_range{ "unordered_flat_map::at" };
	}
	return *get<1>(iter);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::operator[](K&& key_arg)
	-> mapped_type&
{
	using std::get;
	return get<1>(*try_emplace(std::forward<K>(key_arg)).first);
}

// Iterators:
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::begin() noexcept
	-> iterator
{
	using std::begin;
	return iterator{ begin(m_keys), begin(m_values) };
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::end() noexcept
	-> iterator
{
	using std::end;
	return iterator{ end(m_keys), end(m_values) };
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::begin() const noexcept
	-> const_iterator
{
	using std::cbegin;
	return const_iterator{ cbegin(m_keys), m_values.cbegin() };
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::end() const noexcept
	-> const_iterator
{
	using std::cend;
	return const_iterator{ cend(m_keys), m_values.cend() };
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::cbegin() const noexcept
	-> const_iterator
{
	using std::cbegin;
	return const_iterator{ cbegin(m_keys), m_values.cbegin() };
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::cend() const noexcept
	-> const_iterator
{
	using std::cend;
	return const_iterator{ cend(m_keys), m_values.cend() };
}

template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::rbegin() noexcept
	-> reverse_iterator
{
	using std::rbegin;
	return rbegin(m_keys);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::rend() noexcept
	-> reverse_iterator
{
	using std::rend;
	return rend(m_keys);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::rbegin() const noexcept
	-> const_reverse_iterator
{
	using std::crbegin;
	return crbegin(m_keys);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::rend() const noexcept
	-> const_reverse_iterator
{
	using std::crend;
	return crend(m_keys);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::crbegin() const noexcept
	-> const_reverse_iterator
{
	using std::crbegin;
	return crbegin(m_keys);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::crend() const noexcept
	-> const_reverse_iterator
{
	using std::crend;
	return crend(m_keys);
}

// Capacity:
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
bool unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::empty() const noexcept
{
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return m_keys.empty();
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::size() const noexcept
	-> size_type
{
	SH_FLAT_ASSERT(m_keys.size() == m_values.size(),
		"Key & value containers expected to be the same size.");
	return m_keys.size();
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::max_size() const noexcept
	-> size_type
{
	using std::min;
	return min(m_keys.max_size(), m_values.max_size());
}

// Modifiers:
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::try_emplace(const key_type& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_back_if_unique<const key_type&>(key_arg, std::forward<Args>(args)...);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::try_emplace(key_type&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_back_if_unique<key_type&&>(std::move(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::try_emplace(const const_iterator hint, const key_type& key_arg, Args&&... args)
	-> iterator
{
	return this->do_transparent_emplace_back_if_unique<const key_type&>(key_arg, std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::try_emplace(const const_iterator hint, key_type&& key_arg, Args&&... args)
	-> iterator
{
	return this->do_transparent_emplace_back_if_unique<key_type&&>(std::move(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert(const value_type& value)
	-> std::pair<iterator, bool>
{
	return this->emplace(value);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert(value_type&& value)
	-> std::pair<iterator, bool>
{
	return this->emplace(std::move(value));
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert(const_iterator hint, const value_type& value)
	-> iterator
{
	return this->emplace_hint(hint, value);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert(const_iterator hint, value_type&& value)
	-> iterator
{
	return this->emplace_hint(hint, std::move(value));
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename M>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert_or_assign(const key_type& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	using coopt_transparent = void;
	return this->insert_or_assign<const key_type&, decltype(mapped_arg), key_equal, coopt_transparent>(key_arg, std::forward<M>(mapped_arg));
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename M>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert_or_assign(key_type&& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	using coopt_transparent = void;
	return this->insert_or_assign<key_type&&, decltype(mapped_arg), key_equal, coopt_transparent>(std::move(key_arg), std::forward<M>(mapped_arg));
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename M>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert_or_assign(const const_iterator hint, const key_type& key_arg, M&& mapped_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->insert_or_assign<const key_type&, decltype(mapped_arg), key_equal, coopt_transparent>(key_arg, std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename M>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert_or_assign(const const_iterator hint, key_type&& key_arg, M&& mapped_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->insert_or_assign<key_type&&, decltype(mapped_arg), key_equal, coopt_transparent>(std::move(key_arg), std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename HasIteratorCategory>
void unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert(InputIterator first, const InputIterator last)
{
	// Deduplicate the slow way, by checking each element against both the
	// preexisting & newly inserted elements.
	while (first != last)
	{
		this->emplace(*(first++));
	}
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename InputIterator, typename HasIteratorCategory>
void unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert(const unsorted_unique_t, InputIterator first, const InputIterator last)
{
	using std::begin;
	using std::get;
	using std::next;
	// Deduplicate the (fairly) slow way, by checking each element against the
	// preexisting elements but not newly inserted elements, as they're tagged
	// unsorted_unique.
	if constexpr (
		std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>
		&& flat::has_reserve_v<unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>>)
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
		const flat::iterator_t<key_container_type> keys_last = next(begin(m_keys), pre_insert_size);
		if (this->do_find(get<0>(*first), begin(m_keys), keys_last) == keys_last)
		{
			// Desynchronization occurs if m_keys emplaces but m_values throws.
			m_keys.emplace_back(first->first);
			m_values.emplace_back(first->second);
		}
		++first;
	}
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
void unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert(std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	this->insert(begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
void unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert(const unsorted_unique_t, std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	this->insert(unsorted_unique, begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::extract() &&
	-> containers
{
	return containers{ std::move(m_keys), std::move(m_values) };
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
void unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::replace(containers&& cont)
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
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::erase(const const_iterator pos)
	-> iterator
{
	{
		using std::begin;
		using std::cbegin;
		using std::distance;
		using std::end;
		using std::get;
		using std::next;
		const difference_type pos_index{ distance(cbegin(m_keys), get<0>(pos)) };
		const flat::iterator_t<key_container_type> key_iter = next(begin(m_keys), pos_index);
		if (next(key_iter) != end(m_keys))
		{
			const flat::iterator_t<mapped_container_type> value_iter = next(begin(m_values), pos_index);
			// Desynchronization occurs if exception thrown before m_values pops.
			*key_iter = std::move(m_keys.back());
			*value_iter = std::move(m_values.back());
			m_keys.pop_back();
			m_values.pop_back();
			return iterator{ key_iter, value_iter };
		}
	}
	// Desynchronization occurs if m_keys pops but m_values throws.
	m_keys.pop_back();
	m_values.pop_back();
	return this->end();
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::erase(const const_iterator first, const const_iterator last)
	-> iterator
{
	using std::get;
	// Desynchronization occurs if m_keys erases but m_values throws.
	return iterator{ m_keys.erase(get<0>(first), get<0>(last)), m_values.erase(get<1>(first), get<1>(last)) };
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::erase(const key_type& key_arg)
	-> size_type
{
	using coopt_transparent = void;
	return this->erase<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
void unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::swap(unordered_flat_map& other)
	noexcept(std::is_nothrow_swappable_v<key_equal>)
{
	using std::swap;
	// key_equal is allowed to throw, so it goes first.
	swap(this->get_equal(), other.get_equal());
	swap(m_keys, other.m_keys);
	swap(m_values, other.m_values);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
void unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::clear() noexcept
{
	m_keys.clear();
	m_values.clear();
}

// Modifiers (transparent):
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename... Args, typename IsConstructible>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::emplace(Args&&... args)
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
		return this->do_emplace_back_if_unique(key_type{});
	}
	else if constexpr (sizeof...(args) == 1)
	{
		if constexpr (std::is_convertible_v<std::add_pointer_t<Args&&>..., value_type*>
			&& std::is_rvalue_reference_v<Args&&...>)
		{
			// Handle forwarding an r-value pair that is or is like value_type (#3).
			value_type&& value = (static_cast<value_type&&>(args), ...);
			return this->do_emplace_back_if_unique(get<0>(std::move(value)), get<1>(std::move(value)));
		}
		else if constexpr (std::is_convertible_v<std::add_pointer_t<Args&&>..., const value_type*>)
		{
			// Handle an l-value pair that is or is like value_type (#2).
			const value_type& value = (static_cast<const value_type&>(args), ...);
			return this->do_emplace_back_if_unique(get<0>(value), get<1>(value));
		}
		else if (flat::has_conversion_operator_v<Args&&..., value_type&> || flat::has_conversion_operator_v<Args&&..., const value_type&>)
		{
			// Unwrap std::reference_wrapper and similar with conversion operators to value_type (none of the above).
			const value_type& value{ args... };
			return this->do_emplace_back_if_unique(get<0>(value), get<1>(value));
		}
		else
		{
			// Handle other construction such as copies from convertible but different types (#6, #7, #8).
			value_type value(std::forward<Args>(args)...);
			return this->do_emplace_back_if_unique(get<0>(std::move(value)), get<1>(std::move(value)));
		}
	}
	else if constexpr (sizeof...(args) == 2)
	{
		// Handle construction of first & second copy/move (#4, #5).
		return this->do_emplace_back_if_unique(std::forward<Args>(args)...);
	}
	else
	{
		// Handle other construction such as with piecewise_construct_t (#9).
		value_type value(std::forward<Args>(args)...);
		return this->do_emplace_back_if_unique(get<0>(std::move(value)), get<1>(std::move(value)));
	}

}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename... Args, typename IsConstructible>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::emplace_hint([[maybe_unused]] const const_iterator hint, Args&&... args)
	-> iterator
{
	return this->emplace(std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename... Args, typename C, typename IsTransparentAndNotIterator>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::try_emplace(K&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_back_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename... Args, typename C, typename IsTransparentAndNotIterator>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::try_emplace([[maybe_unused]] const const_iterator hint, K&& key_arg, Args&&... args)
	-> iterator
{
	return this->do_transparent_emplace_back_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename M, typename C, typename IsTransparent>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert_or_assign(K&& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	const std::pair<iterator, bool> it_inserted = this->do_transparent_emplace_back_if_unique(std::forward<K>(key_arg), std::forward<M>(mapped_arg));
	if (it_inserted.second == false)
	{
		using std::get;
		get<1>(*it_inserted.first) = std::forward<M>(mapped_arg);
	}
	return it_inserted;
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename M, typename C, typename IsTransparent>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::insert_or_assign([[maybe_unused]] const const_iterator hint, K&& key_arg, M&& mapped_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->insert_or_assign<K, M, key_equal, coopt_transparent>(std::forward<K>(key_arg), std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparentAndConvertible>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::erase(const K& key_arg)
	-> size_type
{
	using std::begin;
	using std::end;
	using std::next;
	const flat::iterator_t<key_container_type> key_begin = begin(m_keys);
	const flat::iterator_t<key_container_type> key_end = end(m_keys);
	const flat::iterator_t<key_container_type> key_iter = this->do_find(key_arg, key_begin, key_end);
	if (key_iter == key_end)
	{
		return 0;
	}
	if (next(key_iter) != key_end)
	{
		using std::distance;
		const flat::iterator_t<mapped_container_type> value_iter = next(begin(m_values), distance(key_begin, key_iter));
		// Desynchronization occurs if anything throws before m_values pops.
		*key_iter = std::move(m_keys.back());
		*value_iter = std::move(m_values.back());
	}
	m_keys.pop_back();
	m_values.pop_back();
	return 1;
}

// Modifiers (extensions):
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename KC, typename MC, typename HasReserve>
void unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::reserve(const size_type n)
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
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
void unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::shrink_to_fit()
{
	m_keys.shrink_to_fit();
	m_values.shrink_to_fit();
}

// Lookup:
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::find(const key_type& key_arg)
	 -> iterator
{
	using coopt_transparent = void;
	return this->find<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::find(const key_type& key_arg) const
	-> const_iterator
{
	using coopt_transparent = void;
	return this->find<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::count(const key_type& key_arg) const
	-> size_type
{
	using coopt_transparent = void;
	return this->count<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
bool unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::contains(const key_type& key_arg) const
{
	using coopt_transparent = void;
	return this->contains<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::equal_range(const key_type& key_arg)
	-> std::pair<iterator, iterator>
{
	using coopt_transparent = void;
	return this->equal_range<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::equal_range(const key_type& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	using coopt_transparent = void;
	return this->equal_range<const key_type&, key_equal, coopt_transparent>(key_arg);
}

// Lookup (transparent):
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::find(const K& key_arg)
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
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::find(const K& key_arg) const
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
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::count(const K& key_arg) const
	-> size_type
{
	using std::begin;
	using std::end;
	return this->do_find(key_arg, begin(m_keys), end(m_keys)) != end(m_keys) ? 1 : 0;
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
bool unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::contains(const K& key_arg) const
{
	using std::begin;
	using std::end;
	return this->do_find(key_arg, begin(m_keys), end(m_keys)) != end(m_keys);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::equal_range(const K& key_arg)
	-> std::pair<iterator, iterator>
{
	const iterator iter = this->find(key_arg);
	using std::get;
	using std::end;
	using std::next;
	return { iter, get<0>(iter) == end(m_keys) ? iter : next(iter) };
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename C, typename IsTransparent>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::equal_range(const K& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	const const_iterator iter = this->find(key_arg);
	using std::get;
	using std::end;
	using std::next;
	return { iter, get<0>(iter) == end(m_keys) ? iter : next(iter) };
}

// Observers:
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::key_eq() const
	-> key_equal
{
	return get_equal();
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::value_eq() const
	-> value_equal
{
	return value_equal{ get_equal() };
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::keys() const
	-> const key_container_type&
{
	return m_keys;
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::values() const
	-> const mapped_container_type&
{
	return m_values;
}

// Non-member functions:
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
bool operator==(const unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>& lhs, const unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>& rhs)
{
	using std::end;
	using std::get;
	if (lhs.size() != rhs.size())
	{
		return false;
	}
	for (const auto& [key, value] : lhs)
	{
		const flat::iterator_t<const unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>> iter = rhs.find(key);
		if (iter == end(rhs) || (value == get<1>(*iter)) == false)
		{
			return false;
		}
	}
	return true;
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
bool operator!=(const unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>& lhs, const unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>& rhs)
{
	using std::end;
	using std::get;
	if (lhs.size() != rhs.size())
	{
		return true;
	}
	for (const auto& [key, value] : lhs)
	{
		const flat::iterator_t<const unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>> iter = rhs.find(key);
		if (iter == end(rhs) || value != get<1>(*iter))
		{
			return true;
		}
	}
	return false;
}

template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename KeyIterator>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::do_find(const key_type& key_arg, const KeyIterator first, const KeyIterator last) const
	-> KeyIterator
{
	using coopt_transparent = void;
	return this->do_find<const key_type&, KeyIterator, key_equal, coopt_transparent>(key_arg, first, last);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename KeyIterator, typename C, typename IsTransparent>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::do_find(const K& key_arg, const KeyIterator first, const KeyIterator last) const
	-> KeyIterator
{
	using std::find_if;
	const key_equal& equal = this->get_equal();
	return find_if(first, last,
		[&key_arg, &equal](const key_type& key)
	{
		return equal(key_arg, key);
	});
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename InputIterator>
void unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::do_insert_back_without_checking_if_unique(InputIterator first, const InputIterator last)
{
	SH_FLAT_ASSERT(m_keys.empty() && m_values.empty(),
		"Key & value containers expected to be empty during do_insert_back_without_checking_if_unique.");
	if constexpr (
		std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>
		&& flat::has_reserve_v<unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>>)
	{
		using std::distance;
		// This is expected to only call from a constructor, at which point
		// there are no preexisting elements to include in the reserve size.
		this->reserve(distance(first, last));
	}
	while (first != last)
	{
		m_keys.emplace_back(first->first);
		m_values.emplace_back(first->second);
		++first;
	}
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::do_emplace_back_if_unique(const key_type& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_back_if_unique(key_arg, std::forward<Args>(args)...);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename... Args>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::do_emplace_back_if_unique(key_type&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_back_if_unique(std::move(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
template <typename K, typename... Args>
auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::do_transparent_emplace_back_if_unique(K&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	const iterator iter = this->find(key_arg);
	using std::get;
	using std::end;
	using std::prev;
	if (get<0>(iter) != end(m_keys))
	{
		return { iter, false };
	}
	if constexpr (std::is_nothrow_constructible_v<mapped_type, Args...>)
	{
		m_keys.emplace_back(std::forward<K>(key_arg));
		flat::scope_guard guard{ [&]() noexcept { m_keys.pop_back(); } };
		m_values.emplace_back(std::forward<Args>(args)...);
		guard.disarm();
	}
	else
	{
		// Construct value before insertion. As its construction may throw, best
		// to do that before mutating the key container.
		mapped_type value(std::forward<Args>(args)...);
		m_keys.emplace_back(std::forward<K>(key_arg));
		flat::scope_guard guard{ [&]() noexcept { m_keys.pop_back(); } };
		m_values.emplace_back(std::move(value));
		guard.disarm();
	}
	return { iterator{ prev(end(m_keys)), prev(end(m_values)) }, true };
}

template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
constexpr auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::get_equal() noexcept
	-> key_equal&
{
	return *this;
}
template <typename Key, typename T, typename KeyEqual, typename KeyContainer, typename MappedContainer>
constexpr auto unordered_flat_map<Key, T, KeyEqual, KeyContainer, MappedContainer>::get_equal() const noexcept
	-> const key_equal&
{
	return *this;
}

} // namespace sh

#endif
