#ifndef LYDIA_PROTOCOL_LYDIACONFIG_H
#define LYDIA_PROTOCOL_LYDIACONFIG_H

#include <binproto/Message.h>

namespace lydia::messages {

	/**
	 * The Lydia message configuration.
	 */
	template<std::uint8_t TypeID, class Payload>
	using LydiaMessageConfig = binproto::Message<TypeID, 0x4C59444D, Payload>;

	/**
	 * Message with a given typeid that has no payload.
	 * This is intended to be inherited from.
	 */
	template<std::uint8_t TypeID>
	struct MessageWithNoPayload : public LydiaMessageConfig<TypeID, MessageWithNoPayload<TypeID>> {
		bool ReadPayload(binproto::BufferReader& reader) {
			return true;
		}

		void WritePayload(binproto::BufferWriter& writer) const {

		}
	};

}

#endif //LYDIA_PROTOCOL_LYDIACONFIG_H
