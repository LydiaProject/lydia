#ifndef BINPROTO_MESSAGE_H
#define BINPROTO_MESSAGE_H

#include <binproto/BufferReader.h>
#include <binproto/BufferWriter.h>

namespace binproto {

	/**
	 * Message wire header.
	 */
	struct MessageHeader {
		std::uint32_t magic{};
		std::uint8_t id{};

		bool read = false;

		bool Read(binproto::BufferReader& reader) {
			magic = reader.ReadUint32();
			id = reader.ReadByte();

			read = true;
			return true;
		}

		void Write(binproto::BufferWriter& writer) const {
			writer.WriteUint32(magic);
			writer.WriteByte(id);
		}

		/**
		 * Check if another fully-defined message type matches this header.
		 *
		 * \tparam Message The fully-defined message type to check.
		 */
		template <class Message>
		[[nodiscard]] bool Is() const {
			if(typename Message::Magic_Const() != magic)
				return false;

			return typename Message::ID_Const() == id;
		}
	};

	/**
	 * A slightly high level message.
	 *
	 * Intended to be configured by a project-level using,
	 * and inherited by the payload class.
	 *
	 * Implements both the Readable and Writable concepts.
	 *
	 * \tparam ID Message type code.
	 * \tparam MAGIC Message magic.
	 * \tparam Payload Inherited payload class.
	 */
	template <std::uint8_t ID, std::uint32_t MAGIC, class Payload>
	struct Message {
		// Expose magic and this message type ID as constants.
		using Magic_Const = std::integral_constant<decltype(MAGIC), MAGIC>;
		using ID_Const = std::integral_constant<decltype(ID), ID>;

		using PayloadType = Payload;

		MessageHeader header { MAGIC, ID };

		bool Read(binproto::BufferReader& reader) {
			// The header can be read first by an application then verified.
			if(!header.read) {
				try {
					if(!header.Read(reader))
						return false;
				} catch(std::exception& ex) {
					return false;
				}
			}

			// FIXME(lily): try this?
			//  Might be wonky since it's not gonna resolve the payload type
			//  but it should work since this class is what defines Magic_Const and ID_Const
			//  if(!header.Is<decltype(*this)>())
			//	    return false;

			if(header.magic != MAGIC)
				return false;

			if(header.id != ID)
				return false;


			// This is kiiinda crusty.. but whatever.
			// at least not *that* code duplicatey. I Guess.
			// would be really nice if Payload could itself
			// implement Readable and Writable.

			try {
				if(!CRTPHelper()->ReadPayload(reader))
					return false;
			} catch(std::exception& ex) {
				return false;
			}
			return true;
		}

		void Write(binproto::BufferWriter& writer) const {
			header.Write(writer);

			// call the payload write function
			CRTPHelper()->WritePayload(writer);
		}

	   private:
		/**
		 * Retrieve a pointer to the payload type.
		 */
		constexpr Payload* CRTPHelper() const {
			return static_cast<Payload*>(this);
		}
	};

} // namespace binproto

#endif //BINPROTO_MESSAGE_H
