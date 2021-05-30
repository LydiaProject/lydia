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
		AddUsers,
		RemUsers,
		Key,
		Mouse,
		Turn
	};

	/**
	 * The Lydia message configuration.
	 */
	template<MessageTypeID TypeID, class Payload>
	using LydiaMessageConfig = binproto::Message<static_cast<std::uint8_t>(TypeID), 0x4C59444D, Payload>;

	/**
	 * Message with a given typeid that has no payload.
	 * This is intended to be inherited from for messages
	 * that don't need to read payload (or ignore any sent.)
	 */
	template<MessageTypeID TypeID>
	struct MessageWithNoPayload : public LydiaMessageConfig<TypeID, MessageWithNoPayload<TypeID>> {
		bool ReadPayload(binproto::BufferReader& reader) {
			return true;
		}

		void WritePayload(binproto::BufferWriter& writer) const {

		}
	};

	/**
	 * A wrapper over std::string which makes it
	 * able to be Readable and Writable.
	 */
	struct ReadableString {

		ReadableString& operator=(const std::string& other) {
			underlying_ = other;
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

}

#endif //LYDIA_PROTOCOL_LYDIACONFIG_H
