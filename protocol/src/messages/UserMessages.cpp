//
// Created by lily on 5/29/2021.
//

#include <lydia/messages/UserMessages.h>

namespace lydia::messages {

	bool UserReference::Read(binproto::BufferReader& reader) {
		uid = reader.ReadUint64();

		if(!reader.ReadMessage(username))
			return false;

		return true;
	}

	void UserReference::Write(binproto::BufferWriter& writer) const {
		writer.WriteUint64(uid);
		writer.WriteMessage(username);
	}

	bool AddUsersMessage::ReadPayload(binproto::BufferReader& reader) {
		if(!reader.ReadMessage(users))
			return false;

		return true;
	}

	void AddUsersMessage::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteMessage(users);
	}

	bool RemUsersMessage::ReadPayload(binproto::BufferReader& reader) {
		if(!reader.ReadMessage(users))
			return false;

		return true;
	}

	void RemUsersMessage::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteMessage(users);
	}

	bool UserRenameMessage::ReadPayload(binproto::BufferReader& reader) {
		if(!reader.ReadMessage(new_name))
			return false;

		return true;
	}

	void UserRenameMessage::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteMessage(new_name);
	}

	bool UserRenameResponse::ReadPayload(binproto::BufferReader& reader) {
		result = static_cast<Result>(reader.ReadByte());
		if(!reader.ReadMessage(new_name))
			return false;
		return true;
	}

	void UserRenameResponse::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteByte(static_cast<std::uint8_t>(result));
		writer.WriteMessage(new_name);
	}

	bool UserRenameBroadcast::ReadPayload(binproto::BufferReader& reader) {
		if(!reader.ReadMessage(user))
			return false;
		return true;
	}

	void UserRenameBroadcast::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteMessage(user);
	}

} // namespace lydia::messages
