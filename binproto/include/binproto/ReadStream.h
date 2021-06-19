//
// Created by lily on 6/18/21.
//

#ifndef LYDIA_READSTREAM_H
#define LYDIA_READSTREAM_H

#include <binproto/EndianUtils.h> // needed

#include <cstring>
#include <string>
#include <vector>

namespace binproto {

	/**
	 * A Stream which reads values from a buffer.
	 */
	struct ReadStream {
		inline ReadStream() = default;

		/**
		 * Shortcut ctor to automatically load a buffer.
		 */
		explicit inline ReadStream(const std::vector<std::uint8_t>& vec) {
			Load(vec);
		}

		/**
		 * Load a buffer for this stream to read.
		 * \param[in] vec The buffer to load in.
		 */
		void Load(const std::vector<std::uint8_t>& vec) {
			begin = vec.data();
			end = begin + vec.size();
			cur = begin;
		}

		void Rewind() {
			cur = begin;
		}

		// Implements Stream

		void Error(const std::string& message) {
			if(!is_errored) {
				is_errored = true;
				error = message;
			}
		}

		[[nodiscard]] inline bool HasError() const {
			return is_errored;
		}

		[[nodiscard]] inline std::string GetError() const {
			if(HasError())
				return error;
			return "";
		}

		inline void Byte(std::uint8_t& byte) {
			if(!HasError()) {
				if(BoundCheckType<std::uint8_t>()) {
					byte = *cur++;
				}
			}
		}

		template <std::endian Endian = std::endian::big>
		inline void Uint16(std::uint16_t& uint16) {
			ReadSwappable<Endian, std::uint16_t>(uint16);
		}

		template <std::endian Endian = std::endian::big>
		inline void Uint32(std::uint32_t& uint32) {
			ReadSwappable<Endian, std::uint32_t>(uint32);
		}

		template <std::endian Endian = std::endian::big>
		inline void Uint64(std::uint64_t& uint64) {
			ReadSwappable<Endian, std::uint64_t>(uint64);
		}

		template <std::endian Endian = std::endian::big>
		inline void Int16(std::int16_t& int16) {
			ReadSwappable<Endian, std::int16_t>(int16);
		}

		template <std::endian Endian = std::endian::big>
		inline void Int32(std::int32_t& int32) {
			ReadSwappable<Endian, std::int32_t>(int32);
		}

		template <std::endian Endian = std::endian::big>
		inline void Int64(std::int64_t& int64) {
			ReadSwappable<Endian, std::int64_t>(int64);
		}

		inline void String(std::string& string) {
			// Strings are stored ala:
			//
			// struct StringWire {
			//  std::uint32_t length; // Big endian
			//  char data[length];
			// };
			// They're Pascal strings. Because Pascal strings = Best Strings.

			// Read the length.
			std::uint32_t length;
			Uint32<std::endian::big>(length);

			if(!HasError()) {
				// TODO: check for a non-ridiculous size here.
				string.resize(length);
				memcpy(&string[0], cur, length * sizeof(char));
				cur += length;
			}
		}

		template<class T>
		constexpr void TransformOther(T& transformable) {
			transformable.Transform(*this);
		}

	   private:
		template <std::endian Endian, class T>
		requires(internal::detail::IsSwappable<T>) inline void ReadSwappable(T& swappable) {
			if(!is_errored) {
				if(BoundCheckType<T>()) {
					// TODO: Fix this
					switch(Endian) {
						case std::endian::little:
							swappable = internal::ReadLE<T>(cur);
							break;
						case std::endian::big:
							swappable = internal::ReadBE<T>(cur);
							break;
					}
					cur += sizeof(T);
				}
			}
		}

		[[nodiscard]] inline bool CanRead(std::size_t bytes) const {
			return !((cur + bytes) > end);
		}

		// bounds check a basic integral type.
		template <class T>
		[[nodiscard]] inline bool BoundCheckType() {
			return BoundCheck(sizeof(T));
		}

		[[nodiscard]] inline bool BoundCheck(std::size_t size) {
			if(!CanRead(size)) {
				Error("Cannot read " + std::to_string(size) + " bytes from the provided buffer.");
				return false;
			}
			return true;
		}

		/**
		 * True if the stream is errored.
		 */
		bool is_errored{false};

		/**
		 * The error message.
		 */
		std::string error;

		const std::uint8_t* cur{};
		const std::uint8_t* begin{};
		const std::uint8_t* end{};
	};

} // namespace binproto

#endif //LYDIA_READSTREAM_H
