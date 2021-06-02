#ifndef LYDIA_PROTOCOL_LYDIACONFIG_H
#define LYDIA_PROTOCOL_LYDIACONFIG_H

#include <binproto/Message.h>

namespace lydia::messages {

	/**
 	 * All message Type ID's.
 	 */
	enum class MessageTypeID : std::uint8_t {
		Connect,
		List,
		UserConnects,
		UserDisconnect,
		UserRename,
		Key,
		Mouse,
		Turn,
		ChatCreateWhisperChannel,
		ChatDeleteWhisperChannel,
		ChatMessage
	};

	/**
	 * The Lydia protocol message configuration.
	 */
	template <MessageTypeID TypeID, class Payload>
	using Message = binproto::Message<static_cast<std::uint8_t>(TypeID), 0x4C59444D, Payload>;

	/**
	 * Message with a given typeid that has no payload.
	 *
	 * This is intended to be inherited from for messages
	 * that don't need to read a payload (or ignore any sent.)
	 */
	template <MessageTypeID TypeID>
	struct MessageWithNoPayload : public Message<TypeID, MessageWithNoPayload<TypeID>> {
		bool ReadPayload(binproto::BufferReader& reader) {
			return true;
		}

		void WritePayload(binproto::BufferWriter& writer) const {
		}
	};

	/**
	 * A shim over std::string which makes it
	 * fulfill the Readable and Writable concepts.
	 */
	struct ReadableString {
		ReadableString& operator=(const std::string& other) {
			underlying_ = other;
			return *this;
		}

		ReadableString& operator=(const ReadableString& other) {
			if(&other == this)
				return *this;

			underlying_ = other.underlying_;
			return *this;
		}

		std::string& Get() {
			return underlying_;
		}

		explicit operator std::string&() {
			return Get();
		}

		bool Read(binproto::BufferReader& reader) {
			underlying_ = reader.ReadString();
			return true;
		}

		void Write(binproto::BufferWriter& writer) const {
			writer.WriteString(underlying_);
		}

	   private:
		std::string underlying_;
	};

	/**
 	 * This is a handy little thing to make bitflag enums nicer.
 	 * It'd be nice if the bit to set was a regular param...
 	 * clang complains about a "non const read" in the static_assert for some reason though.
 	 *
 	 * TODO: Put this in narwhal, along with strongly typed bitflag utilities
 	 */
	template <class Enum, std::underlying_type_t<Enum> bit_to_set>
	consteval std::underlying_type_t<Enum> bit() {
		using Type = std::underlying_type_t<Enum>;
		const auto BITS = (sizeof(Type) * CHAR_BIT);

		// idiot check beforehand that the bit to set can be held
		// inside of the underlying type of the enum
		static_assert(bit_to_set <= BITS, "invalid bit for the underlying enum type");

		return static_cast<Type>(1 << bit_to_set);
	}

} // namespace lydia::messages

#endif //LYDIA_PROTOCOL_LYDIACONFIG_H
