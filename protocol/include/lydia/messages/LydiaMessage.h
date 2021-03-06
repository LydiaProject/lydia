#ifndef LYDIA_PROTOCOL_LYDIACONFIG_H
#define LYDIA_PROTOCOL_LYDIACONFIG_H

#include <binproto/Message.h>

namespace lydia::messages {

	/**
 	 * All message Type ID's.
 	 */
	enum class MessageOpcode : std::uint8_t {
		Connect,
		List,
		UserConnects,
		UserDisconnect,
		UserRename,
		Key,
		Mouse,
		MouseMovement, // sent to other clients
		MouseCursorUpdate,
		Turn,
		TurnAdministration, // pause turns, such

		ChatCreateWhisperChannel, // TODO
		ChatDeleteWhisperChannel, // TODO
		ChatMessage
	};

	/**
	 * The Lydia protocol message configuration.
	 */
	template <MessageOpcode Opcode, class Payload>
	using Message = binproto::Message<static_cast<std::uint8_t>(Opcode), 0x4C59444D, Payload>;

	/**
	 * Message with a given opcode that has no payload.
	 *
	 * This is intended to be inherited from for messages
	 * that don't need to read a payload (or ignore any sent.)
	 */
	template <MessageOpcode Opcode>
	struct MessageWithNoPayload : public Message<Opcode, MessageWithNoPayload<Opcode>> {
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



} // namespace lydia::messages

#endif //LYDIA_PROTOCOL_LYDIACONFIG_H
