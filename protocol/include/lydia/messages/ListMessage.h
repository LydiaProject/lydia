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

		/**
		 * How many VMReferences there are,
		 * (unused when writing)
		 */
		std::uint32_t count;

		/**
		 * All the VMReferences we collected,
		 * or on the client, have.
		 */
		std::vector<VMReference> vms;

		bool ReadPayload(binproto::BufferReader& reader) {
			count = reader.ReadUint32();
			vms.resize(count);

			// Read all the VMReferences
			for(std::uint32_t i = 0; i < count; ++i)
				if(!vms[i].Read(reader))
					return false;

			return true;
		}

		void WritePayload(binproto::BufferWriter& writer) {
			writer.WriteUint32(vms.size());

			for(std::uint32_t i = 0; i < vms.size(); ++i)
				vms[i].Write(writer);
		}
	};

}

#endif //LYDIA_PROTOCOL_LISTMESSAGE_H
