/**
 * \file Messages related to users.
 */

#ifndef LYDIA_USERMESSAGES_H
#define LYDIA_USERMESSAGES_H

#include <binproto/Array.h>
#include <binproto/Optional.h>
#include <lydia/messages/LydiaConfig.h>

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

	struct AddUsersMessage : public LydiaMessage<MessageTypeID::AddUsers, AddUsersMessage> {
		/**
		 * The users we are adding.
		 * These references will have names attached to them
		 */
		binproto::Array<UserReference> users;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

	struct RemUsersMessage : public LydiaMessage<MessageTypeID::RemUsers, RemUsersMessage> {
		/**
		 * The users we are removing.
		 * These references won't have names attached to them.
		 */
		binproto::Array<UserReference> users;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

	/**
	 * A client to server rename message.
	 */
	struct UserRenameMessage : public LydiaMessage<MessageTypeID::UserRename, UserRenameMessage> {
		/**
		 * The name this client wants to rename to.
		 */
		ReadableString new_name;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

	/**
	 * Sent to the client attempting to rename.
	 */
	struct UserRenameResponse : public LydiaMessage<MessageTypeID::UserRename, UserRenameResponse> {
		enum class Result : std::uint8_t {
			Success,

			/**
			 * The username is too long.
			 */
			UsernameTooLong,

			/**
			 * The username is currently taken by a guest user,
			 * or a registered user.
			 */
			UsernameTaken,

			/**
			 * The username failed sanitization checks.
			 */
			UsernameInvalid
		};

		Result result;

		/**
		 * The new username. Sent if result == Success.
		 */
		binproto::Optional<ReadableString> new_name;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

	/**
	 * Sent to all connected clients except the user renaming when a user successfully renames.
	 */
	struct UserRenameBroadcast : public LydiaMessage<MessageTypeID::UserRename, UserRenameBroadcast> {
		/**
		 * The user renaming.
		 * The username sent will be the new username.
		 */
		UserReference user;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};
} // namespace lydia::messages

#endif //LYDIA_USERMESSAGES_H
