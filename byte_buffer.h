#include "common.h"
#include "Box.h"
#include <string>

class ByteOrder {
	public:
	static bool isBigEndianHost();
	static uint32_t byteToHostOrderUint32(byte * byteArr);
	static uint64_t byteToHostOrderUint64(byte * byteArr);
	static void uint32ToNwByteArr(IN uint32_t num, OUT byte *byteArr);
	static void uint64ToNwByteArr(IN uint64_t num, OUT byte * byteArr);
};

class ByteBuffer {
	byte *base;
	byte *fptr;
	const size_t fileSize;
	size_t currPos;

	public:
	ByteBuffer(void *fptrArg, size_t fileSizeArg) : base((byte *)fptrArg), fptr((byte *)fptrArg), fileSize(fileSizeArg), currPos(0)
	{
	}
	
	ByteBuffer(void *fptrArg, size_t fileSizeArg, size_t currPosArg) : base((byte *)fptrArg), fptr((byte *)fptrArg), fileSize(fileSizeArg), currPos(currPosArg)
	{
	}

	void reset();
	size_t getPosition() const;
	void setPosition(size_t pos);
	void setPositionRel(size_t pos);
	size_t read(byte *buf, size_t byteCount);
	size_t read_s(byte *buf, size_t maxBufSize, size_t byteCount);
	uint32_t readUint32();
	uint64_t readUint64();
	void readBoxHeader(OUT BoxHeader &headerObj);
	void readFullBoxHeader(OUT FullBoxHeader &headerObj);
	void readPartialFullBoxHeader(OUT FullBoxHeader &headerObj);
	bool verifyBoxType(std::string boxName);
	size_t size() const { 
		return fileSize; 
	}
};
