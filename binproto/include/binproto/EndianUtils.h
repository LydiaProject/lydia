#ifndef BINPROTO_ENDIANUTILS_H
#define BINPROTO_ENDIANUTILS_H

#include <climits>
#include <cstdint>

// TODO: Even though this is internal and limited specialization-wize,
// maybe emit these in the main binproto namespace?

namespace binproto::internal {

	/**
	 * Read a big endian value from a buffer.
	 *
	 * \tparam T The type to read. Must be one of the following, otherwise linker errors will happen:
	 * 			- std::uint16_t
	 * 			- std::uint32_t
	 * 			- std::uint64_t
	 *
	 * \param[in] base Pointer to buffer to read value in.
	 * \return A value of type T in native endian.
	 */
	template <class T>
	T ReadBE(const std::uint8_t* base);

	/**
	 * Write a big endian value to a buffer.
	 *
	 * \tparam T The type to write. Must be one of the following, otherwise linker errors will happen:
	 * 			- std::uint16_t
	 * 			- std::uint32_t
	 * 			- std::uint64_t
	 *
	 * \param[out] base Pointer to buffer to write value to.
	 * \param[in] val The value to write.
	 */
	template <class T>
	void WriteBE(std::uint8_t* base, const T& val);
} // namespace binproto::internal

#endif //BINPROTO_ENDIANUTILS_H
