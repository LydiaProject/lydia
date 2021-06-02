//
// Created by lily on 6/1/2021.
//

#ifndef LYDIA_CONTROLMESSAGES_H
#define LYDIA_CONTROLMESSAGES_H

#include <lydia/messages/LydiaMessage.h>
#include <lydia/messages/UserMessages.h>

namespace lydia::messages {



	struct KeyMessage : public Message<MessageTypeID::Key, KeyMessage> {
		/**
		 * The keysym of the key that should be pressed or released.
		 */
		std::uint16_t key_sym{};

		/**
		 * True if the key is to be pressed, false otherwise.
		 */
		bool pressed{};

		bool ReadPayload(binproto::BufferReader& reader);
		void WritePayload(binproto::BufferWriter& writer) const;
	};

	struct MouseMessage : public Message<MessageTypeID::Mouse, MouseMessage> {

		/**
		 * Mouse button bitflags.
		 */
		enum class Buttons : std::uint8_t {
			None = 0,
			Left = bit<Buttons, 0>(),
			Middle = bit<Buttons, 1>(),
			Right = bit<Buttons, 2>()

			// If needed: other misc buttons.
			// Injecting those will *definitely* need the agent to provide control facilities.
		};

		Buttons buttons{Buttons::None};

		std::uint16_t x;
		std::uint16_t y;

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

	struct TurnClientMessage : public MessageWithNoPayload<MessageTypeID::Turn> {};

}

#endif //LYDIA_CONTROLMESSAGES_H
