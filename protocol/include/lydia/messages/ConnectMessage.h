#ifndef LYDIA_PROTOCOL_CONNECTMESSAGE_H
#define LYDIA_PROTOCOL_CONNECTMESSAGE_H

#include <lydia/messages/LydiaConfig.h>

namespace lydia::messages {

	struct ConnectMessage : public LydiaMessageConfig<0x0, ConnectMessage> {
		std::string vm;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

	struct ConnectResponse : public LydiaMessageConfig<0x0, ConnectResponse> {
		bool success;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

} // namespace lydia::messages

#endif //LYDIA_PROTOCOL_CONNECTMESSAGE_H
