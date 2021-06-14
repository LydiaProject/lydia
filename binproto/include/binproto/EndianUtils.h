#ifndef BINPROTO_ENDIANUTILS_H
#define BINPROTO_ENDIANUTILS_H

#include <bit>
#include <climits>
#include <cstdint>

// In this case, the compiler detection in this file is intended
// to condense intrinsics into a single macro, so that the code
// is more readable, and does not have lots of #ifdef garbage.
// Basically, a "get the ugliness done and over with so we can replace it with better code" strategy.
//
// For portability, the following macros are used to adapt the code across compilers
// the Lydia tree is expected to support.
// They are:
//
// std::uint16_t BINPROTO_BYTESWAP16(std::uint16_t x) -> 16bit byte swap
// std::uint32_t BINPROTO_BYTESWAP32(std::uint32_t x) -> 32bit byte swap
// std::uint64_t BINPROTO_BYTESWAP64(std::uint64_t x) -> 64bit byte swap
//
#ifdef _MSC_VER // MSVC
	#include <intrin.h>

	// Prefer the intrinsic function versions of the following functions.
	//
	// These will be similar to GCC intrinisics if the intrinsic
	// or even single instruction version exist on the target.
	// (we never call into the CRT anyways, so, it should still be relatively quick if not)

	#pragma intrinsic(_byteswap_ushort)
	#pragma intrinsic(_byteswap_ulong)
	#pragma intrinsic(_byteswap_uint64)

	#define BINPROTO_BYTESWAP16(x) _byteswap_ushort(x)
	#define BINPROTO_BYTESWAP32(x) _byteswap_ulong(x)
	#define BINPROTO_BYTESWAP64(x) _byteswap_uint64(x)
#else
	#ifdef __GNUC__ // GCC & Clang (including Emscripten)
		// Builtin functions with GNU C & Clang get turned into (sometimes single-instruction) intrinsics
		// usually by default if the target supports them.
		// Otherwise, they become inline functions
		// (which still *have* a speed penalty, but far less then if it had to make a call into the C runtime)
		#define BINPROTO_BYTESWAP16(x) __builtin_bswap16(x)
		#define BINPROTO_BYTESWAP32(x) __builtin_bswap32(x)
		#define BINPROTO_BYTESWAP64(x) __builtin_bswap64(x)
	#else
		#error Unsupported compiler. Check in support for it?
	#endif
#endif

namespace binproto::internal {

	namespace detail {

		/**
		 * Concept constraining a type which is byte-swappable.
		 * This type must be:
		 *  - Bigger than or equal to sizeof(std::uint16_t)...
		 *  - BUT smaller than or equal to sizeof(std::uint64_t)
		 *  - It must also be a "trivial" type i.e std::is_trivial_v<T> must be true
		 */
		template <class T>
		concept IsSwappable = (sizeof(T) >= sizeof(std::uint16_t)) && (sizeof(T) <= sizeof(std::uint64_t)) && std::is_trivial_v<T>;

		/**
		 * Do a swap of a thing of type T.
		 */
		template <class T>
		constexpr T Swap(const T& val) requires(IsSwappable<T>) {
			switch(sizeof(T)) {
				case sizeof(std::uint16_t):
					return BINPROTO_BYTESWAP16(val);

				case sizeof(std::uint32_t):
					return BINPROTO_BYTESWAP32(val);

				case sizeof(std::uint64_t):
					return BINPROTO_BYTESWAP64(val);

				// TODO: A worst-case path which swaps
				//  sizeof(std::uint16_t) aligned types.
				//  Once applied here, BinProto will uniformly support this, right away.
			}
		}

		template <class T>
		constexpr T SwapIfLE(const T& val) requires(IsSwappable<T>) {
			if constexpr(std::endian::native == std::endian::little) // NOLINT (we intentionally use this)
				return Swap(val);
			else
				return val;
		}

		template <class T>
		constexpr T SwapIfBE(const T& val) requires(IsSwappable<T>) {
			if constexpr(std::endian::native == std::endian::big) // NOLINT (we intentionally use this)
				return Swap(val);
			else
				return val;
		}

		/**
		 * Get a reference as T to a buffer.
		 */
		template <class T>
		constexpr T& PointerTo(void* ptr) {
			return *static_cast<T*>(ptr);
		}

		/**
		 * Get a const reference as T to a const buffer.
		 */
		template <class T>
		constexpr T& PointerTo(const void* ptr) requires(std::is_const_v<T>) {
			// Same as above PointerTo, but only participates in overload
			// resolution for const types.
			return *static_cast<T*>(ptr);
		}

	} // namespace detail

	/**
 	 * Read a big endian value from a buffer.
 	 *
 	 * \tparam T The type to read.
 	 * \param[in] base Pointer to buffer to read value in.
 	 * \return A value of type T in native endian.
 	 */
	template <class T>
	T ReadBE(const std::uint8_t* base) requires(detail::IsSwappable<std::remove_cvref_t<T>>) {
		const auto& ref = detail::PointerTo<const std::remove_cvref_t<T>>(base);
		return detail::SwapIfLE<T>(ref);
	}

	/**
	 * Write a big endian value to a buffer.
	 *
	 * \tparam T The type to write.
	 *
	 * \param[out] base Pointer to buffer to write value to.
	 * \param[in] val The value to write.
	 */
	template <class T>
	void WriteBE(std::uint8_t* base, const T& val) requires(detail::IsSwappable<std::remove_cvref_t<T>>) {
		auto& i = detail::PointerTo<T>(base);
		i = detail::SwapIfLE<T>(val);
	}

	// Not documented since they're not used in BinProto or Lydia,
	// but feel free to yoink these

	template <class T>
	T ReadLE(const std::uint8_t* base) requires(detail::IsSwappable<std::remove_cvref_t<T>>) {
		const auto& ref = detail::PointerTo<const std::remove_cvref_t<T>>(base);
		return detail::SwapIfBE<std::remove_cvref_t<T>>(ref);
	}

	template <class T>
	void WriteLE(std::uint8_t* base, const T& val) requires(detail::IsSwappable<std::remove_cvref_t<T>>) {
		auto& i = detail::PointerTo<T>(base);
		i = detail::SwapIfBE<T>(val);
	}

} // namespace binproto::internal

#undef BINPROTO_BYTESWAP16
#undef BINPROTO_BYTESWAP32
#undef BINPROTO_BYTESWAP64

#endif //BINPROTO_ENDIANUTILS_H
