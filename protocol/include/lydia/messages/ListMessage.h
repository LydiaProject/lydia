//
// Created by lily on 5/5/2021.
//

#ifndef LYDIA_PROTOCOL_LISTMESSAGE_H
#define LYDIA_PROTOCOL_LISTMESSAGE_H

#include <lydia/messages/LydiaMessage.h>
#include <lydia/messages/VMReference.h>

namespace lydia::messages {

	/**
	 * List message. Has no payload data of importance, besides
	 * the type id.
	 */
	struct ListMessage : public MessageWithNoPayload<MessageOpcode::List> {};

	/**
	 * The response to a ListMessage.
	 */
	struct ListResponse : public Message<MessageOpcode::List, ListResponse> {
		/**
		 * All nodes.
		 */
		binproto::Array<VMReference> nodes;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};



}

#endif //LYDIA_PROTOCOL_LISTMESSAGE_H
