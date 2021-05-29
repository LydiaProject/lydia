#ifndef LYDIA_PROTOCOL_CONNECTMESSAGE_H
#define LYDIA_PROTOCOL_CONNECTMESSAGE_H

#include <lydia/messages/LydiaConfig.h>

namespace lydia::messages {

	struct ConnectMessage : public LydiaMessageConfig<MessageTypeID::Connect, ConnectMessage> {
		std::string vm;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

	struct ConnectResponse : public LydiaMessageConfig<MessageTypeID::Connect, ConnectResponse> {
		bool success;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

} // namespace lydia::messages

#endif //LYDIA_PROTOCOL_CONNECTMESSAGE_H
