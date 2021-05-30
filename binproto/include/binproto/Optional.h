#ifndef BINPROTO_OPTIONAL_H
#define BINPROTO_OPTIONAL_H

#include <binproto/BufferReader.h>
#include <binproto/BufferWriter.h>
#include <binproto/Concepts.h>

#include <cassert>
#include <cstring>

namespace binproto {

	/**
	 * Represents a optional field, which may or
	 * may not exist in a Readable or Message payload.
	 *
	 * I know this misses some features from the native std::optional<T> type,
	 * however, this is really only meant to be a nicety type and Lydia only
	 * needs the niceties here.
	 */
	template <class T>
	requires(Readable<T>&& Writable<T>) struct Optional {
		Optional& operator=(const T& value) {
			if(!has_value)
				has_value = true;

			// We don't do any funky stuff with
			// the BufferWriter or anything,
			// since this class doesn't store wire data.
			// Only once it's been written is it wire-ivied.

			if constexpr(std::is_trivial_v<T>) {
				std::memcpy(&data[0], (void*)&value, sizeof(T));
			} else {
				// Since the object is not trivial,
				// we choose to invoke the copy ctor using placement new.
				// We might be able to elide it anyways?
				new(GetPtr()) T(value);
			}

			return *this;
		}

		Optional& operator=(const Optional& other) {
			// handle self-assignment by not doing anything
			if(&other == this)
				return *this;

			if(other.HasValue()) {
				this = *other.GetPtr();
			}
			return *this;
		}

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

			// doesn't have a value, so we just return true.
			if(!has_value)
				return true;

			return GetPtr()->Read(reader);
		}

		void Write(binproto::BufferWriter& writer) const {
			writer.WriteByte(HasValue());

			// If we have a value, then we should write it!
			if(HasValue())
				GetPtr()->Write(writer);
		}

	   private:
		/**
		 * Get a pointer to the internal optional buffer as a T*.
		 */
		T* GetPtr() {
			return reinterpret_cast<T*>(&data[0]);
		}

		const T* GetPtr() const {
			return reinterpret_cast<const T*>(&data[0]);
		}

		bool has_value = false;
		alignas(T) std::uint8_t data[sizeof(T)]{};
	};
} // namespace binproto

#endif //BINPROTO_OPTIONAL_H
