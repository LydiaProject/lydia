#include <binproto/ReadStream.h>
#include <binproto/WriteStream.h>

#include <cstdint>
#include <iostream>

/**
 * This is a test of the new BinProto Transform/Streams code...
 */
struct TransformTest {
	std::uint8_t byte;
	std::string str;

	template <class Stream>
	void Transform(Stream& stream) {
		stream.Byte(byte);
		stream.String(str);
	}
};

int main(int argc, char** argv) {
	TransformTest t;
	TransformTest t2;
	binproto::WriteStream writer;
	binproto::ReadStream reader;

	t.byte = 23;
	t.str = "Hello World";
	t.Transform<binproto::WriteStream>(writer);

	auto buf = writer.Release();

	// let's see if we can read that test class back in from the writer,
	// for a quick test.
#if 0
	reader.Load(buf);
	t2.Transform<binproto::ReadStream>(reader);

	if(t2.byte == t.byte) {
		std::cout << "t2.byte == " << (int)t2.byte << ", test passaronied\n";
		if(t2.str == t.str) {
			std::cout << "t2.str == \"" << t2.str << "\", test passaronied\n";
		}
	}
#endif

	std::cout.write(reinterpret_cast<const char*>(&buf[0]), buf.size());

	return 0;
}