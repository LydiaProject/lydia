#ifndef LYDIA_PROTOCOL_VMREFERENCE_H
#define LYDIA_PROTOCOL_VMREFERENCE_H

#include <binproto/BufferReader.h>
#include <binproto/BufferWriter.h>

#include <binproto/Optional.h>
#include <binproto/Array.h>

namespace lydia::messages {

	/**
	 * The description fields of a VM. May be left unused by a frontend
	 */
	struct VMDescription {
		/**
		 * The full name of a VM.
		 *
		 * Example: "Windows 7 X64"
		 */
		std::string name;

		/**
		 * A longer description.
		 *
		 * Example: "Windows 7 Professional VM for everyone to use."
		 */
		std::string description;

		/**
		 * Message of the day, shown first.
		 * Only used by Collab frontend
		 *
		 * Example: "Welcome to CollabVM!"
		 */
		std::string motd;

		/**
		 * The hypervisor this VM is running on.
		 */
		enum class Hypervisor : std::uint8_t {
			QEMU,
			Virtualbox,
			VMWare,
			Unknown
		} hypervisor;

		/**
		 * How many vCPUs are configured for the guest.
		 */
		std::uint8_t VCPUCount;

		/**
		 * RAM size in bytes. The client will do unit conversion to the best unit for this.
		 */
		std::uint64_t RamSize;

		/**
		 * Disk size in bytes. Same as above.
		 */
		std::uint64_t DiskSize;

		/**
		 * If the Lydia On-VM agent is not installed on the guest,
		 * or the VM is configured to only allow Legacy WebP mode,
		 * this is true
		 */
		bool Legacy;

		/**
		 * Whether or not the Lydia On-VM agent has been configured
		 * to enable file uploads.
		 */
		bool FileUploads;

		/**
		 * If this VM is running on a non-relayed Lydia host.
		 */
		bool Official;

		bool Read(binproto::BufferReader& reader);

		void Write(binproto::BufferWriter& writer) const;

	};

	/**
	 * A client-side reference to a VM.
	 */
	struct VMReference {
		/**
		 * ID of the VM this is referencing.
		 */
		std::string id;

		/**
		 * VM description.
		 */
		binproto::Optional<VMDescription> description;

		/**
		 * Preview image in WebP format,
		 * at 200x200.
		 */
		binproto::Optional<binproto::ByteArray> preview_image;

		bool Read(binproto::BufferReader& reader);
		void Write(binproto::BufferWriter& writer) const;
	};


}

#endif //LYDIA_PROTOCOL_VMREFERENCE_H
