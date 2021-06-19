#ifndef BINPROTO_BUFFERREADER_H
#define BINPROTO_BUFFERREADER_H

#include <cstdint>
#include <string>
#include <vector>

#include <binproto/Concepts.h>

namespace binproto {

	/**
	 * A buffer reader.
	 */
	struct [[deprecated("Use ReadStream")]] BufferReader {
		/**
		 * Type used for length storage.
		 */
		using LengthType = std::uint32_t;

		/** Default-ctor so it can just be in a codec class in user code */
		BufferReader();

		explicit BufferReader(const std::vector<std::uint8_t>& buf);


		/**
		 * Load a buffer into this object so it can be re-used to load lots of buffers.
		 * \param[in] buf A buffer to load.
		 */
		void LoadBuffer(const std::vector<std::uint8_t>& buf);

		/**
		 * Completely rewind the buffer back to the start.
		 */
		void Rewind();


		std::uint8_t ReadByte();

		std::uint16_t ReadUint16();
		std::int16_t ReadInt16();

		std::uint32_t ReadUint32();
		std::int32_t ReadInt32();

		std::uint64_t ReadUint64();
		std::int64_t ReadInt64();

		/**
		 * Read length, while bounds checking that value from our buffer
		 *
		 * \param[in] Individual element size.
		 */
		LengthType ReadLength(std::size_t elem_size = sizeof(std::uint8_t));

		//float ReadFloat();
		//double ReadDouble();

		std::string ReadString();
		std::vector<std::uint8_t> ReadBytes();

		/**
 		 * Shorthand to read a message or other Readable type.
 		 *
		 * \tparam T The message type to read. Must be Readable.
 		 * \param[out] message Message to read.
		 *
		 * \return True on success, false otherwise.
 		 */
		template<Readable T>
		bool ReadMessage(T& message) {
#ifndef __EMSCRIPTEN__
			try {
#endif
				if(!message.Read(*this))
					return false;
#ifndef __EMSCRIPTEN__
			} catch(std::exception& ex) {
				return false;
			}
#endif
			return true;
		}

	   private:
		/**
		 * Internal bounds checking helper
		 * Throws an internally defined exception if something is awry
		 */
		void BoundsCheck(std::size_t size) const;

		const std::uint8_t* cur;
		const std::uint8_t* begin;
		const std::uint8_t* end;
	};

} // namespace binproto

#endif //BINPROTO_BUFFERREADER_H
