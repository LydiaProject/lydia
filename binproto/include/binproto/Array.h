#ifndef BINPROTO_ARRAY_H
#define BINPROTO_ARRAY_H

#include <binproto/BufferReader.h>
#include <binproto/BufferWriter.h>
#include <binproto/Concepts.h>

namespace binproto {

	/**
	 * Represents an array of objects.
	 */
	template <class T>
	requires(Readable<T>&& Writable<T>) struct Array {
		bool Read(binproto::BufferReader& reader) {
			auto len = reader.ReadLength();
			array_.resize(len);

			for(auto i = 0; i < len; ++i)
				if(!array_[i].Read(reader))
					return false;

			return true;
		}

		void Write(binproto::BufferWriter& writer) const {
			writer.WriteUint32(static_cast<std::uint32_t>(array_.size()));

			for(auto& elem : array_)
				elem.Write(writer);
		}

		std::vector<T>& GetUnderlying() {
			return array_;
		}

	   private:
		std::vector<T> array_;
	};


	/**
	 * A generic array of bytes (wrapping over ReadBytes() basically)
	 */
	struct ByteArray {

		std::vector<std::uint8_t>& GetUnderlying();

		bool Read(binproto::BufferReader& reader);
		void Write(binproto::BufferWriter& writer) const;

	   private:
		std::vector<std::uint8_t> data;
	};

} // namespace binproto

#endif //BINPROTO_OPTIONAL_H
