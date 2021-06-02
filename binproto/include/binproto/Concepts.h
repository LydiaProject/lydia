#ifndef BINPROTO_CONCEPTS_H
#define BINPROTO_CONCEPTS_H

namespace binproto {

	struct BufferReader;
	struct BufferWriter;

	/**
	 * This concept constrains to types that are readable via a BufferReader.
	 * Ideally speaking, a Readable object should also implement Writable for protocol usage.
	 *
	 * In exemplar form, this would be
	 * \code
	 * 	class ReadableObject {
	 * 	public:
	 * 		bool Read(BufferReader& reader) {
	 * 			// Read some values!
	 * 			return true;
	 * 		}
	 * 	};
	 * \endcode
	 */
	template <class T>
	concept Readable = requires(T t, BufferReader& reader) {
		{ t.Read(reader) } -> std::convertible_to<bool>;
	};

	/**
	 * This concept constrains to types that are writable to a BufferWriter.
	 *
	 * In exemplar form, this would be
	 * \code
	 * 	class WritableObject {
	 * 	public:
	 * 		void Write(BufferWriter& writer) const {
	 * 			// Write some values!
	 * 		}
	 * 	};
	 * \endcode
	 */
	template <class T>
	concept Writable = requires(T t, BufferWriter& writer) {
		// T::Write must take a BufferWriter& only, and can only
		// return void (nothing else).
		{ t.Write(writer) } -> std::same_as<void>;
	};

	/**
	 * This concept constrains to types that are usable as message payloads.
	 * Ideally, message payloads could implement Readable and Writable themselves,
	 * but they don't at the moment.
	 */
	template<class T>
	concept MessagePayload = requires(T payload, BufferReader& reader, BufferWriter& writer) {
		{ payload.ReadPayload(reader) } -> std::same_as<bool>;
		{ payload.WritePayload(writer) } -> std::same_as<void>;
	};

} // namespace binproto

#endif //BINPROTO_CONCEPTS_H
