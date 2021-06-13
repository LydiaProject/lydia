//
// Created by lily on 6/1/2021.
//

#ifndef LYDIA_CONTROLMESSAGES_H
#define LYDIA_CONTROLMESSAGES_H

#include <lydia/messages/LydiaMessage.h>
#include <lydia/messages/UserMessages.h>
#include <narwhal/EnumBitflagUtils.h>

namespace lydia::messages {

	struct KeyMessage : public Message<MessageTypeID::Key, KeyMessage> {
		/**
		 * The keysym of the key that should be pressed or released.
		 */
		std::uint16_t key_sym {};

		/**
		 * True if the key is to be pressed, false otherwise.
		 */
		bool pressed {};

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

	struct MouseMessage : public Message<MessageTypeID::Mouse, MouseMessage> {
		/**
		 * Mouse button bitflags.
		 */
		enum class Buttons : std::uint8_t {
			None = 0,
			Left = narwhal::bit<Buttons, 0>(),
			Middle = narwhal::bit<Buttons, 1>(),
			Right = narwhal::bit<Buttons, 2>()

			// If needed: other misc buttons.
			// Injecting those will *definitely* need the agent to provide control facilities.
		};

		Buttons buttons { Buttons::None };

		std::uint16_t x {};
		std::uint16_t y {};

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

	/**
	 * Mouse cursor movement. Sent to other users
	 * (since it's kind of pointless sending it to the mover)
	 */
	struct MouseMoveMessage : public Message<MessageTypeID::MouseMovement, MouseMoveMessage> {
		std::uint16_t x {};
		std::uint16_t y {};

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

	/**
	 * Sent when the guest updates the cursor image
	 * with either a capable graphics adapter (for instance QEMU's QXL graphics adapter)
	 * or the Agent.
	 */
	struct MouseCursorUpdateMessage : public Message<MessageTypeID::MouseCursorUpdate, MouseCursorUpdateMessage> {
		bool hidden; // Whether or not the cursor is hidden.

		/**
		 * The cursor image.
		 * This is a raw 32bpp RGBA image.
		 */
		binproto::Optional<binproto::ByteArray> cursor_image;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

	struct TurnServerMessage : public Message<MessageTypeID::Turn, TurnServerMessage> {
		/**
		 * The current turn queue.
		 */
		binproto::Array<UserReference> users;

		/**
		 * Time when the turn queue will either empty
		 * or it will be your turn.
		 */
		std::uint32_t turn_ms;

		/**
		 * If the turn queue is paused, this will be sent true.
		 */
		bool paused;

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

	/**
	 * Sent by the client to indicate they want to take a turn.
	 */
	struct TurnClientMessage : public MessageWithNoPayload<MessageTypeID::Turn> {};

	// TODO move

	struct ChatChannelReference {
		enum class Type : std::uint8_t {
			VMChat, // "public" VM chat
			Whisper
		};

		std::uint64_t id;
		Type type;
	};


	NARWHAL_ENUM_IS_FLAG(MouseMessage::Buttons)
} // namespace lydia::messages

#endif //LYDIA_CONTROLMESSAGES_H
