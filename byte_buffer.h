#include "common.h"
#include "Box.h"
typedef uint8_t byte;

class ByteOrder {
	const static uint32_t one = 0x0001;
	byte *bytes = (byte *)&one;
	public:
	static bool isBigEndianHost();
	static uint32_t byteToHostOrderUint32(byte * byteArr);
	static uint64_t byteToHostOrderUint64(byte * byteArr);
	static uint32_t uint32ToNwByteArr(IN uint32_t num, OUT byte *byteArr);
	static uint64_t uint64ToNwByteArr(IN uint64_t num, OUT byte * byteArr);
};

class ByteBuffer {
	byte *base;
	byte *fptr;
	const size_t fileSize;
	size_t currPos;

	public:
	ByteBuffer(void *fptrArg, size_t fileSizeArg) : fptr((byte *)fptrArg), base((byte *)fptrArg), fileSize(fileSizeArg)
	{
	}

	void reset();
	size_t getPosition() const;
	void setPosition(size_t pos);
	void setPositionRel(size_t pos);
	size_t read(byte *buf, size_t byteCount);
	size_t read_s(byte *buf, size_t maxBufSize, size_t byteCount);
	uint32_t readUint32(uint32_t &num);
};
