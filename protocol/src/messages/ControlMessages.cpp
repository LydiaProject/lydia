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

	bool MouseMoveMessage::ReadPayload(binproto::BufferReader& reader) {
		x = reader.ReadUint16();
		y = reader.ReadUint16();
		return true;
	}

	void MouseMoveMessage::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteUint16(x);
		writer.WriteUint16(y);
	}

	bool MouseCursorUpdateMessage::ReadPayload(binproto::BufferReader& reader) {
		hidden = reader.ReadByte();
		reader.ReadMessage(cursor_image);
		return true;
	}

	void MouseCursorUpdateMessage::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteByte(hidden);
		writer.WriteMessage(cursor_image);
	}

	bool TurnServerMessage::ReadPayload(binproto::BufferReader& reader) {
		if(!reader.ReadMessage(users))
			return false;
		turn_ms = reader.ReadUint32();
		paused = reader.ReadByte();
		return true;
	}

	void TurnServerMessage::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteMessage(users);
		writer.WriteUint32(turn_ms);
		writer.WriteByte(paused);
	}
} // namespace lydia::messages