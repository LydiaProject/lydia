//
// Created by lily on 5/5/2021.
//

#ifndef LYDIA_PROTOCOL_LISTMESSAGE_H
#define LYDIA_PROTOCOL_LISTMESSAGE_H

#include <lydia/messages/LydiaConfig.h>
#include <lydia/messages/VMReference.h>

namespace lydia::messages {

	/**
	 * List message. Has no payload data of importance, besides
	 * the type id.
	 */
	struct ListMessage : public MessageWithNoPayload<0x1> {};

	/**
	 * The response to a ListMessage.
	 */
	struct ListResponse : public LydiaMessageConfig<0x1, ListResponse> {

		binproto::Array<VMReference> nodes;

		bool ReadPayload(binproto::BufferReader& reader) {
			if(!nodes.Read(reader))
				return false;
			return true;
		}

		void WritePayload(binproto::BufferWriter& writer) {
			nodes.Write(writer);
		}
	};

}

#endif //LYDIA_PROTOCOL_LISTMESSAGE_H
