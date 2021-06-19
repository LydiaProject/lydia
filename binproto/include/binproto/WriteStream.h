//
// Created by lily on 6/18/21.
//

#ifndef LYDIA_WRITESTREAM_H
#define LYDIA_WRITESTREAM_H

#include <binproto/EndianUtils.h> // needed

#include <cstring>
#include <string>
#include <vector>

namespace binproto {

	/**
	 * A Stream which reads values from a buffer.
	 */
	struct WriteStream {
		/**
		 * Shortcut ctor to control starting size
		 */
		inline WriteStream(std::size_t starting_size = 8) {
			Grow(starting_size);
		}

		std::vector<std::uint8_t> Release() {
			std::vector<std::uint8_t> vec;
			vec.resize(cur_index_);

			memcpy(&vec[0], &buffer_[0], cur_index_);

			// Clear the data buffer entirely.
			buffer_.clear();
			return vec;
		}

		// Implements Stream

		void Error(const std::string& message) {
		}

		[[nodiscard]] inline bool HasError() const {
			return false;
		}

		[[nodiscard]] inline std::string GetError() const {
			return "";
		}

		inline void Byte(const std::uint8_t& byte) {
			if((cur_index_ + sizeof(std::uint8_t)) > buffer_.size())
				Grow(sizeof(std::uint8_t));
			buffer_[cur_index_++] = byte;
		}

		template <std::endian Endian = std::endian::big>
		inline void Uint16(const std::uint16_t& uint16) {
			WriteSwappable<Endian, std::uint16_t>(uint16);
		}

		template <std::endian Endian = std::endian::big>
		inline void Uint32(const std::uint32_t& uint32) {
			WriteSwappable<Endian, std::uint32_t>(uint32);
		}

		template <std::endian Endian = std::endian::big>
		inline void Uint64(const std::uint64_t& uint64) {
			WriteSwappable<Endian, std::uint64_t>(uint64);
		}

		template <std::endian Endian = std::endian::big>
		inline void Int16(std::int16_t& int16) {
			WriteSwappable<Endian, std::int16_t>(int16);
		}

		template <std::endian Endian = std::endian::big>
		inline void Int32(std::int32_t& int32) {
			WriteSwappable<Endian, std::int32_t>(int32);
		}

		template <std::endian Endian = std::endian::big>
		inline void Int64(std::int64_t& int64) {
			WriteSwappable<Endian, std::int64_t>(int64);
		}

		inline void String(const std::string& string) {
			// Strings are stored ala:
			//
			// struct StringWire {
			//  std::uint32_t length; // Big endian
			//  char data[length];
			// };
			// They're Pascal strings. Because Pascal strings = Best Strings.

			Uint32<std::endian::big>(string.length());

			if((cur_index_ + string.length()) > buffer_.size())
				Grow(string.length());

			memcpy(&buffer_[cur_index_], &string[0], string.length() * sizeof(char));
			cur_index_ += string.length();
		}

		template <class T>
		constexpr void TransformOther(T& transformable) {
			// TODO fix transformable
			transformable.Transform(*this);
		}

	   private:
		template <std::endian Endian, class T>
		requires(internal::detail::IsSwappable<T>) inline void WriteSwappable(const T& swappable) {
			if((cur_index_ + sizeof(T)) > buffer_.size())
				Grow(sizeof(T));

			// TODO: Fix this...
			switch(Endian) {
				case std::endian::little:
					internal::WriteLE<T>(&buffer_[cur_index_], swappable);
					break;
				case std::endian::big:
					internal::WriteBE<T>(&buffer_[cur_index_], swappable);
					break;
			}
			cur_index_ += sizeof(T);
		}

		inline void Grow(std::size_t grow_by) {
			buffer_.resize(buffer_.size() + grow_by);
		}

		std::vector<std::uint8_t> buffer_;
		std::size_t cur_index_{};
	};

} // namespace binproto

#endif //LYDIA_WRITESTREAM_H
