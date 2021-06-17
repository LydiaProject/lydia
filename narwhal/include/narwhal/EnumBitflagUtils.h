//
// Created by lily on 6/7/2021.
//

#ifndef NARWHAL_ENUMBITFLAGUTILS_H
#define NARWHAL_ENUMBITFLAGUTILS_H

#include <type_traits>

namespace narwhal {
	namespace detail {
		/**
		 * Concept constraining enumerators.
		 */
		template <class T>
		concept UnsignedEnum = std::is_enum_v<T> && std::is_unsigned_v<std::underlying_type_t<T>>;

		// ADL helper.
		template <UnsignedEnum T>
		consteval bool IsFlag() {
			return IsFlag(std::declval<T>());
		}

		/**
		 * Concept constraining to flag enumeration types.
		 * A flag enumeration is one which is marked with the
		 * NARWHAL_ENUM_IS_FLAG macro.
		 *
		 * \code
		 *  enum class MyFlags : std::uint8_t {
		 *   none,
		 *   cool = narwhal::bit<MyFlags, 0>();
		 *  };
		 *  NARWHAL_ENUM_IS_FLAG(MyFlags) // add the ADL overload
		 * \endcode
		 */
		template <class T>
		concept FlagEnum = UnsignedEnum<T> && IsFlag<T>();

		template <FlagEnum T>
		auto UnderlyingValue(T t) {
			return static_cast<std::underlying_type_t<T>>(t);
		}
	} // namespace detail

#define NARWHAL_ENUM_IS_FLAG(T) \
	consteval bool IsFlag(T) {  \
		return true;            \
	}

	template <detail::FlagEnum T>
	constexpr auto operator&(T left, T right) {
		return static_cast<std::underlying_type_t<T>>(detail::UnderlyingValue(left) & detail::UnderlyingValue(right));
	}

	/**
	 * This is a handy little thing to make bitflag enums nicer.
	 */
	template <class Enum, std::underlying_type_t<Enum> bit_to_set>
	consteval std::underlying_type_t<Enum> bit() {
		// It'd be nice if the bit to set was a regular param...
		// clang complains about a "non const read" in the static_assert for some reason though.
		using Type = std::underlying_type_t<Enum>;
		using BitCount = std::integral_constant<std::size_t, (sizeof(Type) * CHAR_BIT)>;

		// Idiot check beforehand that the bit to set can be held
		// inside of the underlying type of the enum
		static_assert(bit_to_set <= BitCount(),
					  "Invalid bit index for the underlying type of this enum.");

		return static_cast<Type>(1 << bit_to_set);
	}

} // namespace narwhal

#endif //NARWHAL_ENUMBITFLAGUTILS_H
