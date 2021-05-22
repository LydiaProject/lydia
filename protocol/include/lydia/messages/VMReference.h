#ifndef LYDIA_PROTOCOL_VMREFERENCE_H
#define LYDIA_PROTOCOL_VMREFERENCE_H

#include <binproto/BufferReader.h>
#include <binproto/BufferWriter.h>

namespace lydia::messages {

	//TODO: seperate .cpp TU for VMDescription and VMReference

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
		 * Example: "Production VM - 10.102.20.1 (win7.lydia.local)"
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
		 * How many VCPUs are configured for the guest.
		 */
		std::uint8_t vcpu_count;

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

		bool Read(binproto::BufferReader& reader) {
			name = reader.ReadString();
			description = reader.ReadString();
			motd = reader.ReadString();
			hypervisor = (Hypervisor)reader.ReadByte();
			vcpu_count = reader.ReadByte();

			// possible maybedo: calculate best unit here?? idk
			RamSize = reader.ReadUint64();
			DiskSize = reader.ReadUint64();
			Legacy = reader.ReadByte();
			FileUploads = reader.ReadByte();
			Official = reader.ReadByte();

			return true;
		}


		void Write(binproto::BufferWriter& writer) const {
			writer.WriteString(name);
			writer.WriteString(description);
			writer.WriteString(motd);
			writer.WriteByte((std::uint8_t)hypervisor);
			writer.WriteByte(vcpu_count);
			writer.WriteUint64(RamSize);
			writer.WriteUint64(DiskSize);
			writer.WriteByte(Legacy);
			writer.WriteByte(FileUploads);
			writer.WriteByte(Official);
		}

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
		 * If this reference has a attached description,
		 * due to an update.
		 */
		bool has_description = false;

		/**
		 * If the reference has a preview,
		 * this is true.
		 */
		bool has_preview = false;

		/**
		 * VM description. Only valid if has_description is true.
		 */
		VMDescription description;

		/**
		 * Preview image in WebP format,
		 * at 200x200. Only transmitted if has_preview == true.
		 */
		std::vector<std::uint8_t> preview_image;

		bool Read(binproto::BufferReader& reader) {
			id = reader.ReadString();
			has_description = reader.ReadByte();
			has_preview = reader.ReadByte();

			if(has_description)
				if(!description.Read(reader))
					return false;

			if(has_preview)
				preview_image = reader.ReadBytes();

			return true;
		}

		void Write(binproto::BufferWriter& writer) const  {
			writer.WriteString(id);

			// a little janky but hey
			writer.WriteByte(has_description);

			if(preview_image.empty())
				writer.WriteByte(false);
			else
				writer.WriteByte(true);

			// Write the name if we need to.
			if(has_description)
				description.Write(writer);

			// Write the preview image if we need to.
			if(!preview_image.empty())
				writer.WriteBytes(preview_image);
		}

	};


}

#endif //LYDIA_PROTOCOL_VMREFERENCE_H
