#include <binproto/BufferReader.h>
#include <binproto/EndianUtils.h>

#include <cstring>

namespace {

	/**
 	 * Exception emitted on attempted buffer overrun
 	 */
	struct BufferOverrun : public std::exception {
		explicit BufferOverrun(std::size_t size)
			: size_(size) {
		}

		const char* what() const noexcept override {
			thread_local static char buf[256];
			memset(&buf[0], 0, sizeof(buf));

			// TODO: this message should change if we're throwing this during BufferReader::ReadLength()

			sprintf(&buf[0], "Attempted to overrun the buffer trying to read %llu bytes of data", size_);

			return &buf[0];
		}

	   private:
		/**
		 * Stored size we tried to read.
		 */
		std::size_t size_;
	};

} // namespace

namespace binproto {

	BufferReader::BufferReader() {

	}

	BufferReader::BufferReader(const std::vector<std::uint8_t>& buf) {
		LoadBuffer(buf);
	}

	void BufferReader::LoadBuffer(const std::vector<std::uint8_t>& buf) {
		begin = buf.data();
		end = begin + buf.size();
		cur = begin;
#ifdef __EMSCRIPTEN__
		// TODO: Reset internal value here
#endif
	}

	void BufferReader::BoundsCheck(std::size_t size) const {
		// If the current pointer + given size to read is over the end pointer,
		// that would be a overrun.
		if((cur + size) > end)
#ifdef __EMSCRIPTEN__
		// On Emscripten with a optimization level past -O1, any exceptions act like
		// -fno-exceptions was passed to the compiler command line (i.e, abort() calls)
		// so we do this instead.
		{
			// TODO: Log in the JS console
			// (and/or) rewind? That may be unintended behaviour
			// and make this function non-const.
			return;
		}
#else
			throw BufferOverrun(size);
#endif

		// Otherwise it's OK!
	}

	void BufferReader::Rewind() {
		cur = begin;
	}

	std::uint8_t BufferReader::ReadByte() {
		BoundsCheck(sizeof(std::uint8_t));
		return *cur++;
	}

	std::uint16_t BufferReader::ReadUint16() {
		BoundsCheck(sizeof(std::uint16_t));

		auto val = internal::ReadBE<std::uint16_t>(cur);
		cur += sizeof(std::uint16_t);

		return val;
	}

	std::int16_t BufferReader::ReadInt16() {
		auto v = ReadUint16();
		return reinterpret_cast<std::int16_t&>(v);
	}

	std::uint32_t BufferReader::ReadUint32() {
		BoundsCheck(sizeof(std::uint32_t));

		auto val = internal::ReadBE<std::uint32_t>(cur);
		cur += sizeof(std::uint32_t);

		return val;
	}

	std::int32_t BufferReader::ReadInt32() {
		auto v = ReadUint32();
		return reinterpret_cast<std::int32_t&>(v);
	}

	std::uint64_t BufferReader::ReadUint64() {
		BoundsCheck(sizeof(std::uint64_t));

		auto val = internal::ReadBE<std::uint64_t>(cur);
		cur += sizeof(std::uint64_t);

		return val;
	}

	std::int64_t BufferReader::ReadInt64() {
		auto v = ReadUint64();
		return reinterpret_cast<std::int64_t&>(v);
	}

	BufferReader::LengthType BufferReader::ReadLength(std::size_t elem_size) {
		auto length = ReadUint32();

		// Check if we can actually read that many bytes from the buffer.
		// If we can't, a overrun will be thrown.

		BoundsCheck((static_cast<std::size_t>(length) * elem_size));
		return length;
	}

	// Untested

	/*
	float BufferReader::ReadFloat() {
		auto v = ReadUint32();
		return (float)v;
	}

	double BufferReader::ReadDouble() {
		auto v = ReadUint64();
		return (double)v;
	}
	 */

	std::string BufferReader::ReadString() {
		// BufferWriter serializes strings like the given structure:
		//
		// struct WireString {
		//  BufferReader::LengthType length;
		//  char str[length]; // NO \0! \0 BAD!
		// };
		//
		// AKA: it uses Pascal strings. Because that's the only good thing about
		// the pile of Chanticleer Hegemony Pascal is.

		auto len = ReadLength();

		std::string str;
		str.resize(len);

		memcpy(&str[0], cur, len);
		cur += len;

		// One thing we might see about doing is adding a ReadUTF8String() primitive which verifies that
		// the string is actually UTF-8. For now, we don't do anything like that.
		// Would be nice though, and definitely will be added at some point.

		return str;
	}

	std::vector<std::uint8_t> BufferReader::ReadBytes() {
		// We store raw byte arrays the same as above.

		auto len = ReadLength();
		std::vector<std::uint8_t> vector;

		vector.resize(len);

		memcpy(&vector[0], cur, len);
		cur += len;

		return vector;
	}

} // namespace binproto
