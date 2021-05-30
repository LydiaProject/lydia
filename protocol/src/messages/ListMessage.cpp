#include <lydia/messages/ListMessage.h>

namespace lydia::messages {

	bool ListResponse::ReadPayload(binproto::BufferReader& reader) {
		if(!reader.ReadMessage(nodes))
			return false;
		return true;
	}

	void ListResponse::WritePayload(binproto::BufferWriter& writer) const {
		writer.WriteMessage(nodes);
	}

}