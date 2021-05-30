//
// Created by lily on 5/29/2021.
//

#include <lydia/messages/VMReference.h>

namespace lydia::messages {


	bool VMDescription::Read(binproto::BufferReader& reader) {
		name = reader.ReadString();
		description = reader.ReadString();
		motd = reader.ReadString();
		hypervisor = static_cast<Hypervisor>(reader.ReadByte());
		VCPUCount = reader.ReadByte();

		// possible maybedo: calculate best unit here?? idk
		RamSize = reader.ReadUint64();
		DiskSize = reader.ReadUint64();
		Legacy = reader.ReadByte();
		FileUploads = reader.ReadByte();
		Official = reader.ReadByte();

		return true;
	}


	void VMDescription::Write(binproto::BufferWriter& writer) const {
		writer.WriteString(name);
		writer.WriteString(description);
		writer.WriteString(motd);
		writer.WriteByte(static_cast<std::uint8_t>(hypervisor));
		writer.WriteByte(VCPUCount);
		writer.WriteUint64(RamSize);
		writer.WriteUint64(DiskSize);
		writer.WriteByte(Legacy);
		writer.WriteByte(FileUploads);
		writer.WriteByte(Official);
	}


	bool VMReference::Read(binproto::BufferReader& reader) {
		id = reader.ReadString();

		if(!reader.ReadMessage(description))
			return false;

		if(!reader.ReadMessage(preview_image))
			return false;

		return true;
	}

	void VMReference::Write(binproto::BufferWriter& writer) const {
		writer.WriteString(id);

		writer.WriteMessage(description);
		writer.WriteMessage(preview_image);
	}
}