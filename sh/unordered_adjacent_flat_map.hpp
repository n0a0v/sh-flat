/*
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

#ifndef INC_SH__UNORDERED_ADJACENT_FLAT_MAP_HPP
#define INC_SH__UNORDERED_ADJACENT_FLAT_MAP_HPP

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
	typename Container = std::vector<std::pair<Key, T>>>
class unordered_adjacent_flat_map : private KeyEqual
{
public:
	using container_type = Container;
	using key_type = Key;
	using mapped_type = T;
	using value_type = typename std::iterator_traits<flat::iterator_t<container_type>>::value_type;
	using key_equal = KeyEqual;
	using reference = value_type&;
	using const_reference = const value_type&;
	using size_type = std::size_t;
	using difference_type = std::ptrdiff_t;
	using iterator = flat::iterator_wrapper<flat::iterator_t<container_type>,
		std::pair<key_type, mapped_type>,
		std::pair<const key_type, mapped_type&>>;
	using const_iterator = flat::iterator_wrapper<flat::const_iterator_t<container_type>,
		std::pair<key_type, mapped_type>,
		std::pair<const key_type&, const mapped_type&>>;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	static_assert(std::is_same_v<Key, std::tuple_element_t<0, value_type>>);
	static_assert(std::is_same_v<T, std::tuple_element_t<1, value_type>>);
	static_assert(std::is_nothrow_swappable_v<container_type>);

	struct value_equal : private key_equal
	{
		explicit value_equal(const key_equal& eq)
			: key_equal{ eq }
		{ }
		bool operator()(const_reference lhs, const_reference rhs) const
		{
			using std::get;
			return this->key_equal::operator()(get<0>(lhs), get<0>(rhs));
		}
	};

	// Member functions:
	unordered_adjacent_flat_map();
	unordered_adjacent_flat_map(const unordered_adjacent_flat_map& other);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		const unordered_adjacent_flat_map& other,
		const Allocator& alloc);
	unordered_adjacent_flat_map(unordered_adjacent_flat_map&& other) noexcept;
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		unordered_adjacent_flat_map&& other,
		const Allocator& alloc);
	unordered_adjacent_flat_map(
		container_type cont,
		const key_equal& eq = key_equal{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		const container_type& cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		const container_type& cont,
		const key_equal& eq,
		const Allocator& alloc);
	unordered_adjacent_flat_map(
		unsorted_unique_t,
		const container_type& cont);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		unsorted_unique_t,
		const container_type& cont,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		unsorted_unique_t,
		const container_type& cont,
		const key_equal& eq,
		const Allocator& alloc);
	explicit unordered_adjacent_flat_map(const key_equal& eq);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(const key_equal& eq, const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	explicit unordered_adjacent_flat_map(const Allocator& alloc);
	template <typename InputIterator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category
	>
	unordered_adjacent_flat_map(
		InputIterator first,
		InputIterator last,
		const key_equal& eq = key_equal{});
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		InputIterator first,
		InputIterator last,
		const key_equal& eq,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		InputIterator first,
		InputIterator last,
		const Allocator& alloc);
	template <typename InputIterator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category
	>
	unordered_adjacent_flat_map(
		unsorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_equal& eq = key_equal{});
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		unsorted_unique_t,
		InputIterator first,
		InputIterator last,
		const key_equal& eq,
		const Allocator& alloc);
	template <typename InputIterator, typename Allocator,
		typename HasIteratorCategory = typename std::iterator_traits<InputIterator>::iterator_category,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		unsorted_unique_t,
		InputIterator first,
		InputIterator last,
		const Allocator& alloc);
	unordered_adjacent_flat_map(
		std::initializer_list<value_type> init,
		 const key_equal& eq = key_equal{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		std::initializer_list<value_type> init,
		const key_equal& eq,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	unordered_adjacent_flat_map(unsorted_unique_t,
		std::initializer_list<value_type> init,
		const key_equal& eq = key_equal{});
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		unsorted_unique_t,
		std::initializer_list<value_type> init,
		const key_equal& eq,
		const Allocator& alloc);
	template <typename Allocator,
		typename UsesAllocator = std::enable_if_t<std::uses_allocator_v<container_type, Allocator>>
	>
	unordered_adjacent_flat_map(
		unsorted_unique_t,
		std::initializer_list<value_type> init,
		const Allocator& alloc);
	~unordered_adjacent_flat_map() = default;

	unordered_adjacent_flat_map& operator=(const unordered_adjacent_flat_map& other);
	unordered_adjacent_flat_map& operator=(unordered_adjacent_flat_map&& other) noexcept;

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
	container_type extract() &&;
	void replace(container_type&& cont);
	iterator erase(const_iterator pos);
	iterator erase(const_iterator first, const_iterator last);
	size_type erase(const key_type& key_arg);
	void swap(unordered_adjacent_flat_map& other) noexcept(std::is_nothrow_swappable_v<key_equal>);
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
	const container_type& container() const;

	// Non-member functions:
	template <typename Key2, typename T2, typename KeyEqual2, typename Container2>
	friend bool operator==(
		const unordered_adjacent_flat_map<Key2, T2, KeyEqual2, Container2>& lhs,
		const unordered_adjacent_flat_map<Key2, T2, KeyEqual2, Container2>& rhs);
	template <typename Key2, typename T2, typename KeyEqual2, typename Container2>
	friend bool operator!=(
		const unordered_adjacent_flat_map<Key2, T2, KeyEqual2, Container2>& lhs,
		const unordered_adjacent_flat_map<Key2, T2, KeyEqual2, Container2>& rhs);

private:
	template <typename Iterator>
	Iterator do_find(const key_type& key_arg, const Iterator first, const Iterator last) const;
	template <typename K,
		typename Iterator,
		typename C = key_equal,
		typename IsTransparent = typename C::is_transparent>
	Iterator do_find(const K& key_arg, const Iterator first, const Iterator last) const;
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

	container_type m_key_value_pairs;
};

// Member functions:
template <typename Key, typename T, typename KeyEqual, typename Container>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map()
	: unordered_adjacent_flat_map{ key_equal{} }
{ }
template <typename Key, typename T, typename KeyEqual, typename Container>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(const unordered_adjacent_flat_map& other)
	: key_equal{ other.get_equal() }
	, m_key_value_pairs{ other.m_key_value_pairs }
{ }
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename Allocator, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(const unordered_adjacent_flat_map& other, const Allocator& alloc)
	: key_equal{ other.get_equal() }
	, m_key_value_pairs{ other.m_key_value_pairs, alloc }
{ }
template <typename Key, typename T, typename KeyEqual, typename Container>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(unordered_adjacent_flat_map&& other) noexcept
	: key_equal{ std::move(other.get_equal()) }
	, m_key_value_pairs{ std::move(other.m_key_value_pairs) }
{ }
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename Allocator, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(unordered_adjacent_flat_map&& other, const Allocator& alloc)
	: key_equal{ std::move(other.get_equal()) }
	, m_key_value_pairs{ std::move(other.m_key_value_pairs), alloc }
{ }
template <typename Key, typename T, typename KeyEqual, typename Container>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(container_type cont, const key_equal& eq)
	: key_equal{ eq }
	, m_key_value_pairs{}
{
	using std::begin;
	using std::end;
	// Use insert rather than copying as it will de-duplicate (the slow way).
	this->insert(begin(cont), end(cont));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename Allocator, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(const container_type& cont, const Allocator& alloc)
	: key_equal{}
	, m_key_value_pairs{ alloc }
{
	using std::begin;
	using std::end;
	// Use insert rather than copying as it will de-duplicate (the slow way).
	this->insert(begin(cont), end(cont));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename Allocator, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(const container_type& cont, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_key_value_pairs{ alloc }
{
	using std::begin;
	using std::end;
	// Use insert rather than copying as it will de-duplicate (the slow way).
	this->insert(begin(cont), end(cont));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(unsorted_unique_t, const container_type& cont)
	: key_equal{}
	, m_key_value_pairs{ cont }
{ }
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename Allocator, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(unsorted_unique_t, const container_type& cont, const Allocator& alloc)
	: key_equal{}
	, m_key_value_pairs{ cont, alloc }
{ }
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename Allocator, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(unsorted_unique_t, const container_type& cont, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_key_value_pairs{ cont, alloc }
{ }
template <typename Key, typename T, typename KeyEqual, typename Container>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(const key_equal& eq)
	: key_equal{ eq }
	, m_key_value_pairs{ }
{ }
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename Allocator, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_key_value_pairs{ alloc }
{ }
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename Allocator, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(const Allocator& alloc)
	: key_equal{}
	, m_key_value_pairs{ alloc }
{ }
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename InputIterator, typename HasIteratorCategory>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(const InputIterator first, const InputIterator last, const key_equal& eq)
	: key_equal{ eq }
	, m_key_value_pairs{}
{
	// Use insert to de-duplicate (the slow way).
	this->insert(first, last);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(const InputIterator first, const InputIterator last, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_key_value_pairs{ alloc }
{
	// Use insert to de-duplicate (the slow way).
	this->insert(first, last);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(const InputIterator first, const InputIterator last, const Allocator& alloc)
	: key_equal{}
	, m_key_value_pairs{ alloc }
{
	// Use insert to de-duplicate (the slow way).
	this->insert(first, last);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename InputIterator, typename HasIteratorCategory>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(unsorted_unique_t, const InputIterator first, const InputIterator last, const key_equal& eq)
	: key_equal{ eq }
	, m_key_value_pairs{}
{
	// Is unsorted_unique, insert blindly.
	this->do_insert_back_without_checking_if_unique(first, last);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(unsorted_unique_t, const InputIterator first, const InputIterator last, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_key_value_pairs{ alloc }
{
	// Is unsorted_unique, insert blindly.
	this->do_insert_back_without_checking_if_unique(first, last);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename InputIterator, typename Allocator, typename HasIteratorCategory, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(unsorted_unique_t, const InputIterator first, const InputIterator last, const Allocator& alloc)
	: key_equal{}
	, m_key_value_pairs{ alloc }
{
	// Is unsorted_unique, insert blindly.
	this->do_insert_back_without_checking_if_unique(first, last);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(std::initializer_list<value_type> init, const key_equal& eq)
	: key_equal{ eq }
	, m_key_value_pairs{}
{
	using std::begin;
	using std::end;
	// Use insert to de-duplicate (the slow way).
	this->insert(begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename Allocator, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(std::initializer_list<value_type> init, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_key_value_pairs{ alloc }
{
	using std::begin;
	using std::end;
	// Use insert to de-duplicate (the slow way).
	this->insert(begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename Allocator, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(std::initializer_list<value_type> init, const Allocator& alloc)
	: key_equal{}
	, m_key_value_pairs{ alloc }
{
	using std::begin;
	using std::end;
	// Use insert to de-duplicate (the slow way).
	this->insert(begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(unsorted_unique_t, std::initializer_list<value_type> init, const key_equal& eq)
	: key_equal{ eq }
	, m_key_value_pairs{}
{
	using std::begin;
	using std::end;
	// Is unsorted_unique, insert blindly.
	this->do_insert_back_without_checking_if_unique(begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename Allocator, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(unsorted_unique_t, std::initializer_list<value_type> init, const key_equal& eq, const Allocator& alloc)
	: key_equal{ eq }
	, m_key_value_pairs{ alloc }
{
	using std::begin;
	using std::end;
	// Is unsorted_unique, insert blindly.
	this->do_insert_back_without_checking_if_unique(begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename Allocator, typename UsesAllocator>
unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::unordered_adjacent_flat_map(unsorted_unique_t, std::initializer_list<value_type> init, const Allocator& alloc)
	: key_equal{}
	, m_key_value_pairs{ alloc }
{
	using std::begin;
	using std::end;
	// Is unsorted_unique, insert blindly.
	this->do_insert_back_without_checking_if_unique(begin(init), end(init));
}

template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::operator=(const unordered_adjacent_flat_map& other)
	-> unordered_adjacent_flat_map&
{
	this->get_equal() = other.get_equal();
	m_key_value_pairs = other.m_key_value_pairs;
	return *this;
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::operator=(unordered_adjacent_flat_map&& other) noexcept
	-> unordered_adjacent_flat_map&
{
	this->get_equal() = std::move(other.get_equal());
	m_key_value_pairs = std::move(other.m_key_value_pairs);
	return *this;
}

// Element access:
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::at(const key_type& key_arg)
	-> mapped_type&
{
	using coopt_transparent = void;
	return this->at<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::at(const key_type& key_arg) const
	-> const mapped_type&
{
	using coopt_transparent = void;
	return this->at<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::operator[](const key_type& key_arg)
	-> mapped_type&
{
	using coopt_transparent = void;
	return this->operator[]<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::operator[](key_type&& key_arg)
	-> mapped_type&
{
	using coopt_transparent = void;
	return this->operator[]<key_type&&, key_equal, coopt_transparent>(std::move(key_arg));
}

// Element access (transparent):
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename C, typename IsTransparent>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::at(const K& key_arg)
	-> mapped_type&
{
	const iterator iter = this->find(key_arg);
	using std::end;
	using std::get;
	if (iter.get() == end(m_key_value_pairs))
	{
		throw std::out_of_range{ "unordered_adjacent_flat_map::at" };
	}
	return get<1>(*iter);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename C, typename IsTransparent>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::at(const K& key_arg) const
	-> const mapped_type&
{
	const const_iterator iter = find(key_arg);
	using std::end;
	using std::get;
	if (iter.get() == end(m_key_value_pairs))
	{
		throw std::out_of_range{ "unordered_adjacent_flat_map::at" };
	}
	return get<1>(*iter);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename C, typename IsTransparent>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::operator[](K&& key_arg)
	-> mapped_type&
{
	using std::get;
	return get<1>(*try_emplace(std::forward<K>(key_arg)).first);
}

// Iterators:
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::begin() noexcept
	-> iterator
{
	using std::begin;
	return iterator{ begin(m_key_value_pairs) };
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::end() noexcept
	-> iterator
{
	using std::end;
	return iterator{ end(m_key_value_pairs) };
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::begin() const noexcept
	-> const_iterator
{
	using std::cbegin;
	return const_iterator{ cbegin(m_key_value_pairs) };
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::end() const noexcept
	-> const_iterator
{
	using std::cend;
	return const_iterator{ cend(m_key_value_pairs) };
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::cbegin() const noexcept
	-> const_iterator
{
	using std::cbegin;
	return const_iterator{ cbegin(m_key_value_pairs) };
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::cend() const noexcept
	-> const_iterator
{
	using std::cend;
	return const_iterator{ cend(m_key_value_pairs) };
}

template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::rbegin() noexcept
	-> reverse_iterator
{
	using std::rbegin;
	return rbegin(m_key_value_pairs);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::rend() noexcept
	-> reverse_iterator
{
	using std::rend;
	return rend(m_key_value_pairs);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::rbegin() const noexcept
	-> const_reverse_iterator
{
	using std::crbegin;
	return crbegin(m_key_value_pairs);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::rend() const noexcept
	-> const_reverse_iterator
{
	using std::crend;
	return crend(m_key_value_pairs);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::crbegin() const noexcept
	-> const_reverse_iterator
{
	using std::crbegin;
	return crbegin(m_key_value_pairs);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::crend() const noexcept
	-> const_reverse_iterator
{
	using std::crend;
	return crend(m_key_value_pairs);
}

// Capacity:
template <typename Key, typename T, typename KeyEqual, typename Container>
bool unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::empty() const noexcept
{
	return m_key_value_pairs.empty();
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::size() const noexcept
	-> size_type
{
	return m_key_value_pairs.size();
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::max_size() const noexcept
	-> size_type
{
	return m_key_value_pairs.max_size();
}

// Modifiers:
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename... Args>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::try_emplace(const key_type& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_back_if_unique<const key_type&>(key_arg, std::forward<Args>(args)...);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename... Args>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::try_emplace(key_type&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_back_if_unique<key_type&&>(std::move(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename... Args>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::try_emplace(const const_iterator hint, const key_type& key_arg, Args&&... args)
	-> iterator
{
	return this->do_transparent_emplace_back_if_unique<const key_type&>(key_arg, std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename... Args>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::try_emplace(const const_iterator hint, key_type&& key_arg, Args&&... args)
	-> iterator
{
	return this->do_transparent_emplace_back_if_unique<key_type&&>(std::move(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert(const value_type& value)
	-> std::pair<iterator, bool>
{
	return this->emplace(value);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert(value_type&& value)
	-> std::pair<iterator, bool>
{
	return this->emplace(std::move(value));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert(const_iterator hint, const value_type& value)
	-> iterator
{
	return this->emplace_hint(hint, value);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert(const_iterator hint, value_type&& value)
	-> iterator
{
	return this->emplace_hint(hint, std::move(value));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename M>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert_or_assign(const key_type& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	using coopt_transparent = void;
	return this->insert_or_assign<const key_type&, decltype(mapped_arg), key_equal, coopt_transparent>(key_arg, std::forward<M>(mapped_arg));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename M>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert_or_assign(key_type&& key_arg, M&& mapped_arg)
	-> std::pair<iterator, bool>
{
	using coopt_transparent = void;
	return this->insert_or_assign<key_type&&, decltype(mapped_arg), key_equal, coopt_transparent>(std::move(key_arg), std::forward<M>(mapped_arg));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename M>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert_or_assign(const_iterator hint, const key_type& key_arg, M&& mapped_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->insert_or_assign<const key_type&, decltype(mapped_arg), key_equal, coopt_transparent>(key_arg, std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename M>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert_or_assign(const_iterator hint, key_type&& key_arg, M&& mapped_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->insert_or_assign<key_type&&, decltype(mapped_arg), key_equal, coopt_transparent>(std::move(key_arg), std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename InputIterator, typename HasIteratorCategory>
void unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert(InputIterator first, const InputIterator last)
{
	// Deduplicate the slow way, by checking each element against both the
	// preexisting & newly inserted elements.
	while (first != last)
	{
		this->emplace(*(first++));
	}
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename InputIterator, typename HasIteratorCategory>
void unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert(const unsorted_unique_t, InputIterator first, const InputIterator last)
{
	using std::begin;
	using std::get;
	using std::next;
	// Deduplicate the (fairly) slow way, by checking each element against the
	// preexisting elements but not newly inserted elements, as they're tagged
	// unsorted_unique.
	if constexpr (
		std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<InputIterator>::iterator_category>
		&& flat::has_reserve_v<unordered_adjacent_flat_map<Key, T, KeyEqual, Container>>)
	{
		using std::distance;
		// This intentionally ignores preexisting elements as a rough heuristic
		// to not over allocate when keys are duplicated. When pre_insert_size
		// is zero, this will reserve enough for [first, last).
		this->reserve(distance(first, last));
	}
	const size_type pre_insert_size{ m_key_value_pairs.size() };
	while (first != last)
	{
		const flat::iterator_t<container_type> key_value_last = next(begin(m_key_value_pairs), pre_insert_size);
		if (this->do_find(get<0>(*first), begin(m_key_value_pairs), key_value_last) == key_value_last)
		{
			m_key_value_pairs.emplace_back(*first);
		}
		++first;
	}
}
template <typename Key, typename T, typename KeyEqual, typename Container>
void unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert(std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	this->insert(begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
void unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert(const unsorted_unique_t, std::initializer_list<value_type> init)
{
	using std::begin;
	using std::end;
	this->insert(unsorted_unique, begin(init), end(init));
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::extract() &&
	-> container_type
{
	return std::move(m_key_value_pairs);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
void unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::replace(container_type&& cont)
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
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::erase(const const_iterator pos)
	-> iterator
{
	{
		using std::begin;
		using std::cbegin;
		using std::distance;
		using std::end;
		using std::next;
		const difference_type pos_index{ distance(cbegin(m_key_value_pairs), pos.get()) };
		const flat::iterator_t<container_type> pos_iter = next(begin(m_key_value_pairs), pos_index);
		if (next(pos_iter) != end(m_key_value_pairs))
		{
			*pos_iter = std::move(m_key_value_pairs.back());
			m_key_value_pairs.pop_back();
			return iterator{ pos_iter };
		}
	}
	m_key_value_pairs.pop_back();
	return this->end();
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::erase(const const_iterator first, const const_iterator last)
	-> iterator
{
	return iterator{ m_key_value_pairs.erase(first.get(), last.get()) };
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::erase(const key_type& key_arg)
	-> size_type
{
	using coopt_transparent = void;
	return this->erase<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
void unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::swap(unordered_adjacent_flat_map& other)
	noexcept(std::is_nothrow_swappable_v<key_equal>)
{
	using std::swap;
	// key_equal is allowed to throw, so it goes first.
	swap(this->get_equal(), other.get_equal());
	swap(m_key_value_pairs, other.m_key_value_pairs);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
void unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::clear() noexcept
{
	m_key_value_pairs.clear();
}

// Modifiers (transparent):
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename... Args, typename IsConstructible>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::emplace(Args&&... args)
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
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename... Args, typename IsConstructible>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::emplace_hint([[maybe_unused]] const const_iterator hint, Args&&... args)
	-> iterator
{
	return this->emplace(std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename... Args, typename C, typename IsTransparentAndNotIterator>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::try_emplace(K&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_back_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename... Args, typename C, typename IsTransparentAndNotIterator>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::try_emplace([[maybe_unused]] const const_iterator hint, K&& key_arg, Args&&... args)
	-> iterator
{
	return this->do_transparent_emplace_back_if_unique(std::forward<K>(key_arg), std::forward<Args>(args)...).first;
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename M, typename C, typename IsTransparent>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert_or_assign(K&& key_arg, M&& mapped_arg)
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
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename M, typename C, typename IsTransparent>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::insert_or_assign(const_iterator hint, K&& key_arg, M&& mapped_arg)
	-> iterator
{
	using coopt_transparent = void;
	return this->insert_or_assign<K, M, key_equal, coopt_transparent>(std::forward<K>(key_arg), std::forward<M>(mapped_arg)).first;
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename C, typename IsTransparentAndNotIterator>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::erase(const K& key_arg)
	-> size_type
{
	using std::begin;
	using std::end;
	using std::next;
	const flat::iterator_t<container_type> end_iter = end(m_key_value_pairs);
	const flat::iterator_t<container_type> iter = this->do_find(key_arg, begin(m_key_value_pairs), end_iter);
	if (iter == end_iter)
	{
		return 0;
	}
	if (next(iter) != end_iter)
	{
		*iter = std::move(m_key_value_pairs.back());
	}
	m_key_value_pairs.pop_back();
	return 1;
}

// Modifiers (extensions):
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename C, typename HasReserve>
void unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::reserve(const size_type n)
{
	m_key_value_pairs.reserve(n);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
void unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::shrink_to_fit()
{
	m_key_value_pairs.shrink_to_fit();
}

// Lookup:
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::find(const key_type& key_arg)
	 -> iterator
{
	using coopt_transparent = void;
	return this->find<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::find(const key_type& key_arg) const
	-> const_iterator
{
	using coopt_transparent = void;
	return this->find<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::count(const key_type& key_arg) const
	-> size_type
{
	using coopt_transparent = void;
	return this->count<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
bool unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::contains(const key_type& key_arg) const
{
	using coopt_transparent = void;
	return this->contains<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::equal_range(const key_type& key_arg)
	-> std::pair<iterator, iterator>
{
	using coopt_transparent = void;
	return this->equal_range<const key_type&, key_equal, coopt_transparent>(key_arg);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::equal_range(const key_type& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	using coopt_transparent = void;
	return this->equal_range<const key_type&, key_equal, coopt_transparent>(key_arg);
}

// Lookup (transparent):
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename C, typename IsTransparent>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::find(const K& key_arg)
	 -> iterator
{
	using std::begin;
	using std::end;
	return iterator{ this->do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) };
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename C, typename IsTransparent>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::find(const K& key_arg) const
	-> const_iterator
{
	using std::begin;
	using std::end;
	return const_iterator{ this->do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) };
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename C, typename IsTransparent>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::count(const K& key_arg) const
	-> size_type
{
	using std::begin;
	using std::end;
	return this->do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) != end(m_key_value_pairs) ? 1 : 0;
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename C, typename IsTransparent>
bool unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::contains(const K& key_arg) const
{
	using std::begin;
	using std::end;
	return this->do_find(key_arg, begin(m_key_value_pairs), end(m_key_value_pairs)) != end(m_key_value_pairs);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename C, typename IsTransparent>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::equal_range(const K& key_arg)
	-> std::pair<iterator, iterator>
{
	const iterator iter = this->find(key_arg);
	using std::get;
	using std::end;
	using std::next;
	return { iter, iter.get() == end(m_key_value_pairs) ? iter : next(iter) };
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename C, typename IsTransparent>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::equal_range(const K& key_arg) const
	-> std::pair<const_iterator, const_iterator>
{
	const const_iterator iter = this->find(key_arg);
	using std::get;
	using std::end;
	using std::next;
	return { iter, get<0>(iter) == end(m_key_value_pairs) ? iter : next(iter) };
}

// Observers:
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::key_eq() const
	-> key_equal
{
	return get_equal();
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::value_eq() const
	-> value_equal
{
	return value_equal{ get_equal() };
}
template <typename Key, typename T, typename KeyEqual, typename Container>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::container() const
	-> const container_type&
{
	return m_key_value_pairs;
}

// Non-member functions:
template <typename Key, typename T, typename KeyEqual, typename Container>
bool operator==(const unordered_adjacent_flat_map<Key, T, KeyEqual, Container>& lhs, const unordered_adjacent_flat_map<Key, T, KeyEqual, Container>& rhs)
{
	using std::end;
	using std::get;
	if (lhs.size() != rhs.size())
	{
		return false;
	}
	for (const auto& [key, value] : lhs)
	{
		const typename unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::const_iterator iter = rhs.find(key);
		if (iter == end(rhs) || (value == get<1>(*iter)) == false)
		{
			return false;
		}
	}
	return true;
}
template <typename Key, typename T, typename KeyEqual, typename Container>
bool operator!=(const unordered_adjacent_flat_map<Key, T, KeyEqual, Container>& lhs, const unordered_adjacent_flat_map<Key, T, KeyEqual, Container>& rhs)
{
	using std::end;
	using std::get;
	if (lhs.size() != rhs.size())
	{
		return true;
	}
	for (const auto& [key, value] : lhs)
	{
		const typename unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::const_iterator iter = rhs.find(key);
		if (iter == end(rhs) || value != get<1>(*iter))
		{
			return true;
		}
	}
	return false;
}

template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename Iterator>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::do_find(const key_type& key_arg, const Iterator first, const Iterator last) const
	-> Iterator
{
	using coopt_transparent = void;
	return this->do_find<const key_type&, Iterator, key_equal, coopt_transparent>(key_arg, first, last);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename Iterator, typename C, typename IsTransparent>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::do_find(const K& key_arg, const Iterator first, const Iterator last) const
	-> Iterator
{
	using std::find_if;
	const key_equal& equal = get_equal();
	return find_if(first, last,
		[&key_arg, &equal](const_reference key_value)
	{
		using std::get;
		return equal(key_arg, get<0>(key_value));
	});
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename InputIterator>
void unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::do_insert_back_without_checking_if_unique(const InputIterator first, const InputIterator last)
{
	using std::end;
	m_key_value_pairs.insert(end(m_key_value_pairs), first, last);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename... Args>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::do_emplace_back_if_unique(const key_type& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_back_if_unique(key_arg, std::forward<Args>(args)...);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename... Args>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::do_emplace_back_if_unique(key_type&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	return this->do_transparent_emplace_back_if_unique(std::move(key_arg), std::forward<Args>(args)...);
}
template <typename Key, typename T, typename KeyEqual, typename Container>
template <typename K, typename... Args>
auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::do_transparent_emplace_back_if_unique(K&& key_arg, Args&&... args)
	-> std::pair<iterator, bool>
{
	const iterator iter = this->find(key_arg);
	using std::end;
	using std::prev;
	if (iter.get() == end(m_key_value_pairs))
	{
		m_key_value_pairs.emplace_back(std::forward<K>(key_arg), mapped_type{ std::forward<Args>(args)... });
		return { iterator{ prev(end(m_key_value_pairs)) }, true };
	}
	return { iter, false };
}

template <typename Key, typename T, typename KeyEqual, typename Container>
constexpr auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::get_equal() noexcept
	-> key_equal&
{
	return *this;
}
template <typename Key, typename T, typename KeyEqual, typename Container>
constexpr auto unordered_adjacent_flat_map<Key, T, KeyEqual, Container>::get_equal() const noexcept
	-> const key_equal&
{
	return *this;
}

} // namespace sh

#endif
