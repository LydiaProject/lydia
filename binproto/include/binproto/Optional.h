#ifndef BINPROTO_OPTIONAL_H
#define BINPROTO_OPTIONAL_H

#include <binproto/BufferReader.h>
#include <binproto/BufferWriter.h>
#include <binproto/Concepts.h>

#include <cassert>

namespace binproto {

	/**
	 * Represents a optional field, which may or
	 * may not exist in a Readable<> or Message payload.
	 */
	template <class T>
	requires(Readable<T>&& Writable<T>) struct Optional {
		void operator=(const T& value) {
			if(!has_value)
				has_value = true;

			// We don't have to do any funky stuff with
			// the BufferWriter or anything,
			// since this class stores device endian data.
			// Only once it's been written is it made BE.

			if constexpr(std::is_trivial_v<T>) {
				std::memcpy(&data[0], (void*)&value, sizeof(T));
			} else {
				// Since the object is not trivial,
				// we choose to invoke the copy ctor using placement new.
				// We might be able to elide it anyways?
				new(GetPtr()) T(value);
			}
		}

		// TODO operator= for Optional&
		// if other does not have a value, don't bother,
		// otherwise do similar behaviour(or even invoke = for const T&)

		/**
		 * Get if this Optional has a stored value
		 */
		bool HasValue() const {
			return has_value;
		}

		T& Value() {
			assert(has_value);
			return *GetPtr();
		}

		T* operator->() {
			assert(has_value);
			return GetPtr();
		}

		// Implementation of Readable and Writable concepts by self

		bool Read(binproto::BufferReader& reader) {
			has_value = reader.ReadByte();

			// doesn't have a value
			if(!has_value)
				return true;

			return GetPtr()->Read(reader);
		}

		void Write(binproto::BufferWriter& writer) const {
			writer.WriteByte(HasValue());

			if(HasValue())
				GetPtr()->Write(writer);
		}

	   private:
		/**
		 * Get a pointer to the internal optional buffer as a T*.
		 */
		T* GetPtr() {
			return static_cast<T*>(&data[0]);
		}

		bool has_value = false;
		alignas(T) std::uint8_t data[sizeof(T)];
	};
} // namespace binproto

#endif //BINPROTO_OPTIONAL_H
