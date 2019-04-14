#include "byte_buffer.h"
#include <iostream>
#include <cstring>

#define READ_LIMIT 500

/************************ ByteOrder **********************/
ByteOrder::isBigEndian() 
{

}
/************************ ByteOrder END ******************/


void ByteBuffer::reset()
{
	fptr = base;
	currPos = 0;
}

size_t ByteBuffer::getPosition() const 
{
	return currPos;
}

void ByteBuffer::setPosition(size_t pos)
{
	if (pos >= fileSize) {
		std::cerr<<"Invalide position";
		return;
	}
	fptr += pos;
	currPos = pos;
}

void ByteBuffer::setPositionRel(size_t pos)
{
	if ((currPos + pos) >= fileSize) {
		std::cerr<<"Invalide Rel position";
		return;
	}
	fptr += pos;
	currPos += pos;
}

size_t ByteBuffer::read(byte *buf, size_t byteCount)
{
	if ((currPos + byteCount) >= fileSize) {
		std::cerr<<"Out of range read position.";
		return 0;
	}

	if (byteCount > READ_LIMIT) {
		std::cerr<<"too large read operation.";
		return 0;
	}
	memcpy(buf, fptr, byteCount);
	fptr += byteCount;
	currPos += byteCount;
}

size_t ByteBuffer::read_s(byte *buf, size_t maxBufSize, size_t byteCount)
{
	if ((byteCount > maxBufSize)) {
		std::cerr<<"Buffer overFlow";
		return 0;
	}
	read(buf, byteCount);
}

size_t ByteBuffer::readUint32(OUT uint32_t &num)
{
	byte tmpBuf[4];
	num = 0;
	fileBuffer.read(tmpBuf, 4);
	num = ByteOrder.byteToHostOrderUint32(tmpBuf);
}

size_t ByteBuffer::readUint64(OUT uint32_t &num)
{
	byte tmpBuf[8];
	num = 0;
	fileBuffer.read(tmpBuf, 8);
	num = ByteOrder.byteToHostOrderUint64(tmpBuf);
}
