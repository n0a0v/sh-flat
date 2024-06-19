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

#ifndef INC_SH__FLAT_HPP
#define INC_SH__FLAT_HPP

#include <cassert>
#include <type_traits>

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

	/**	Check if a given type has a reserve function like std::vector's.
	 *	@tparam T The type to check for a callable T::reserve(size_type) member function.
	 */
	template <typename T, typename = void> struct has_reserve : std::false_type {};
	template <typename T> struct has_reserve<T,
		std::void_t<
			decltype(
				std::declval<T>().reserve(
					std::size_t{}
				)
			)
		>
	> : std::true_type {};
	/**	True if a given type has a reserve function like std::vector's. False otherwise.
	 *	@tparam T The type to check for a callable T::reserve(size_type) member function.
	 */
	template <typename T> constexpr bool has_reserve_v = has_reserve<T>::value;

	/**	Check if a given type has an allocator_type convertible to given allocator type.
	 */
	template <typename T, typename Alloc, typename = void> struct has_convertible_allocator_type : std::false_type {};
	template <typename T, typename Alloc> struct has_convertible_allocator_type<T, Alloc,
		std::void_t<typename T::allocator_type>
	>
	{
		static constexpr bool value = std::is_convertible_v<Alloc, typename T::allocator_type>;
	};
	/**	Check if a given type has an allocator_type convertible to given allocator type.
	 */
	template <typename T, typename Alloc> constexpr bool has_convertible_allocator_type_v
		= has_convertible_allocator_type<T, Alloc>::value;

	/**	Check if a given type has an allocator_type convertible from a given type of allocator.
	 *	@tparam T The type that could have a T::allocator_type.
	 *	@tparam Alloc The type to test if it can be converted to T::allocator_type.
	 */
	template <typename T, typename Alloc>
	struct uses_allocator
	{
		static constexpr bool value = has_convertible_allocator_type_v<T, Alloc>;
	};
	/**	True if a given type has an allocator_type convertible from a given given type of allocator. False otherwise.
	 *	@tparam T The type that could have a T::allocator_type.
	 *	@tparam Alloc The type to test if it can be converted to T::allocator_type.
	 */
	template <typename T, typename Alloc> constexpr bool uses_allocator_v = uses_allocator<T, Alloc>::value;

} // namespace sh

#endif
