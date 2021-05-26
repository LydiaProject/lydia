#include <binproto/Array.h>

namespace binproto {

	std::vector<std::uint8_t>& ByteArray::GetUnderlying() {
		return data;
	}

	bool ByteArray::Read(binproto::BufferReader &reader) {
		data = reader.ReadBytes();
		return true;
	}

	void ByteArray::Write(binproto::BufferWriter &writer) const {
		writer.WriteBytes(data);
	}

}