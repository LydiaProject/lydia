#include <binproto/EndianUtils.h>

#include <bit>
#include <concepts>

#ifdef _MSC_VER
	#include <intrin.h>

	// Prefer the intrinsic function versions
	// of the following functions.
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
	// Builtin functions with GNU C & Clang get turned into (sometimes single-instruction) intrinsics
	// usually by default if the target supports them.
	// Otherwise, they become inline functions
	// (which still *have* a speed penalty, but far less then if it had to make a call into the C runtime)
	#define BINPROTO_BYTESWAP16(x) __builtin_bswap16(x)
	#define BINPROTO_BYTESWAP32(x) __builtin_bswap32(x)
	#define BINPROTO_BYTESWAP64(x) __builtin_bswap64(x)
#endif

namespace {

	template <class T>
	constexpr T Swap(const T& val) requires((sizeof(T) >= sizeof(std::uint16_t)) && (sizeof(T) <= sizeof(std::uint64_t))) {
		switch(sizeof(T)) {
			case sizeof(std::uint16_t):
				return BINPROTO_BYTESWAP16(val);

			case sizeof(std::uint32_t):
				return BINPROTO_BYTESWAP32(val);

			case sizeof(std::uint64_t):
				return BINPROTO_BYTESWAP64(val);
		}
	}

	template <class T>
	constexpr T SwapIfLE(const T& val) requires((sizeof(T) >= sizeof(std::uint16_t)) && (sizeof(T) <= sizeof(std::uint64_t))) {
		if constexpr(std::endian::native == std::endian::little)
			return Swap(val);
		else
			return val;
	}

	template <class T>
	constexpr T& PtrAs(void* ptr) {
		return *static_cast<T*>(ptr);
	}

	template <class T>
	constexpr T& PtrAs(const void* ptr) requires(std::is_const_v<T>) {
		// Same as above PtrAs, but only participates in overload
		// resolution for const types.
		return *static_cast<T*>(ptr);
	}

} // namespace

namespace binproto::internal {

	template <>
	std::uint16_t ReadBE<std::uint16_t>(const std::uint8_t* base) {
		const auto& i = PtrAs<const std::uint16_t>(base);
		return SwapIfLE<std::uint16_t>(i);
	}

	template <>
	void WriteBE<std::uint16_t>(std::uint8_t* base, const std::uint16_t& val) {
		auto& i = PtrAs<std::uint16_t>(base);
		i = SwapIfLE<std::uint16_t>(val);
	}

	template <>
	std::uint32_t ReadBE<std::uint32_t>(const std::uint8_t* base) {
		const auto& i = PtrAs<const std::uint32_t>(base);
		return SwapIfLE<std::uint32_t>(i);
	}

	template <>
	void WriteBE<std::uint32_t>(std::uint8_t* base, const std::uint32_t& val) {
		auto& i = PtrAs<std::uint32_t>(base);
		i = SwapIfLE<std::uint32_t>(val);
	}

	template <>
	std::uint64_t ReadBE<std::uint64_t>(const std::uint8_t* base) {
		const auto& i = PtrAs<const std::uint64_t>(base);
		return SwapIfLE<std::uint64_t>(i);
	}

	template <>
	void WriteBE<std::uint64_t>(std::uint8_t* base, const std::uint64_t& val) {
		auto& i = PtrAs<std::uint64_t>(base);
		i = SwapIfLE<std::uint64_t>(val);
	}

} // namespace binproto::internal
