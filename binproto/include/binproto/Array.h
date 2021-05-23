#ifndef BINPROTO_ARRAY_H
#define BINPROTO_ARRAY_H

#include <binproto/BufferReader.h>
#include <binproto/BufferWriter.h>
#include <binproto/Concepts.h>

namespace binproto {

	/**
	 * Represents a optional field, which may or
	 * may not exist in a Readable<> or Message payload.
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
			writer.WriteLength(array_.size());

			for(auto& elem : array_)
				elem.Write(writer);
		}

		std::vector<T> GetUnderlying() {
			return array_;
		}

	   private:
		std::vector<T> array_;
	};
} // namespace binproto

#endif //BINPROTO_OPTIONAL_H
