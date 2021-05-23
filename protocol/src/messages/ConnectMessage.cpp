#include <lydia/messages/ConnectMessage.h>

namespace lydia::messages {

	bool ConnectMessage::ReadPayload(binproto::BufferReader& reader) {
		vm = reader.ReadString();
		return true;
	}

	void ConnectMessage::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteString(vm);
	}

	bool ConnectResponse::ReadPayload(binproto::BufferReader& reader) {
		success = reader.ReadByte();
		return true;
	}

	void ConnectResponse::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteByte(success);
	}

} // namespace lydia::messages
