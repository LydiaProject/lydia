//
// Created by lily on 6/1/2021.
//

#include <lydia/messages/ControlMessages.h>

namespace lydia::messages {

	bool KeyMessage::ReadPayload(binproto::BufferReader& reader) {
		key_sym = reader.ReadUint16();
		pressed = reader.ReadByte();
		return true;
	}

	void KeyMessage::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteUint16(key_sym);
		writer.WriteByte(pressed);
	}

	bool MouseMessage::ReadPayload(binproto::BufferReader& reader) {
		buttons = static_cast<MouseMessage::Buttons>(reader.ReadByte());
		x = reader.ReadUint16();
		y = reader.ReadUint16();
		return true;
	}

	void MouseMessage::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteByte(static_cast<std::uint8_t>(buttons));
		writer.WriteUint16(x);
		writer.WriteUint16(y);
	}

	bool TurnServerMessage::ReadPayload(binproto::BufferReader& reader) {
		if(!reader.ReadMessage(users))
			return false;
		return true;
	}

	void TurnServerMessage::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteMessage(users);
	}
} // namespace lydia::messages