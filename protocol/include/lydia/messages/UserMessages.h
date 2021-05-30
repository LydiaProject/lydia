/**
 * \file Messages related to users.
 */

#ifndef LYDIA_USERMESSAGES_H
#define LYDIA_USERMESSAGES_H

#include <lydia/messages/LydiaConfig.h>
#include <binproto/Optional.h>
#include <binproto/Array.h>

namespace lydia::messages {

	/**
	 * A reference to a user.
	 */
	struct UserReference {
		/**
		 * User snowflake. Always sent.
		 */
		std::uint64_t uid;

		/**
		 * Username. Only sent if the username is changing or this is the first user reference for this user;
		 * a client implementation shall have its own uid => username mapping cache.
		 */
		binproto::Optional<ReadableString> username;

		bool Read(binproto::BufferReader& reader);
		void Write(binproto::BufferWriter& writer) const;
	};

	struct AddUsersMessage : public LydiaMessageConfig<MessageTypeID::AddUsers, AddUsersMessage> {
		/**
		 * The users we are adding.
		 */
		binproto::Array<UserReference> users;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

	struct RemUsersMessage : public LydiaMessageConfig<MessageTypeID::RemUsers, RemUsersMessage> {
		/**
		 * The users we are removing.
		 */
		binproto::Array<UserReference> users;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

}

#endif //LYDIA_USERMESSAGES_H
