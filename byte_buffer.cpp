#include "byte_buffer.h"
#include <iostream>
#include <cstring>
#include <string>

#define READ_LIMIT 500

/************************ ByteOrder **********************/
//This could be replaced by macro.
bool ByteOrder::isBigEndianHost() 
{
	endian32 endianDetect;
	endianDetect.num = 1;
	return (1 == endianDetect.bytes[3]) ? true : false;
}

/*Input ByteArray is in BigEndian*/
uint32_t ByteOrder::byteToHostOrderUint32(byte *byteArr)
{
	endian32 tmpUnion;
	if (ByteOrder::isBigEndianHost()) {
		tmpUnion.bytes[0] = byteArr[0];
		tmpUnion.bytes[1] = byteArr[1];
		tmpUnion.bytes[2] = byteArr[2];
		tmpUnion.bytes[3] = byteArr[3];
		return tmpUnion.num;
	} else {
		tmpUnion.bytes[0] = byteArr[3];
		tmpUnion.bytes[1] = byteArr[2];
		tmpUnion.bytes[2] = byteArr[1];
		tmpUnion.bytes[3] = byteArr[0];
		return tmpUnion.num;
	}
}

uint64_t ByteOrder::byteToHostOrderUint64(byte *byteArr)
{
	endian64 tmpUnion;
	if (ByteOrder::isBigEndianHost()) {
		for (int i = 0; i < 8; i++) {
			tmpUnion.bytes[i] = byteArr[i];
		}
		return tmpUnion.num;
	} else {
		for (int i = 0; i < 8; i++) {
			tmpUnion.bytes[i] = byteArr[7 - i];
		}
		return tmpUnion.num;
	}
}

void ByteOrder::uint32ToNwByteArr(uint32_t num, byte *byteArr)
{
	endian32 tmpUnion;
	tmpUnion.num = num;
	if (ByteOrder::isBigEndianHost()) {
		byteArr[0] = tmpUnion.bytes[0];
		byteArr[1] = tmpUnion.bytes[1];
		byteArr[2] = tmpUnion.bytes[2];
		byteArr[3] = tmpUnion.bytes[3];
	} else {
		byteArr[0] = tmpUnion.bytes[3];
		byteArr[1] = tmpUnion.bytes[2];
		byteArr[2] = tmpUnion.bytes[1];
		byteArr[3] = tmpUnion.bytes[0];
	}
}

void ByteOrder::uint64ToNwByteArr(uint64_t num, byte *byteArr)
{
	endian64 tmpUnion;
	tmpUnion.num = num;
	if (ByteOrder::isBigEndianHost()) {
		for (int i = 0; i < 8; i++) {
			byteArr[i] = tmpUnion.bytes[i];
		}
	} else {
		for (int i = 0; i < 8; i++) {
			byteArr[i] = tmpUnion.bytes[7 - i];
		}
	}
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

uint32_t ByteBuffer::readUint32()
{
	byte tmpBuf[4];
	ByteBuffer::read(tmpBuf, 4);
	return ByteOrder::byteToHostOrderUint32(tmpBuf);
}

uint64_t ByteBuffer::readUint64()
{
	byte tmpBuf[8];
	ByteBuffer::read(tmpBuf, 8);
	return ByteOrder::byteToHostOrderUint64(tmpBuf);
}

void ByteBuffer::readBoxHeader(BoxHeader &headerObj)
{
	uint32_t size;
	size_t offset = currPos;
	byte name[4];
	size = readUint32();
	read(name, 4);
	headerObj.setBoxHeader(size, name, offset);
}

void ByteBuffer::readFullBoxHeader(FullBoxHeader &fullHeaderObj)
{
	byte version[1];
	byte flags[3];
	ByteBuffer::readBoxHeader(fullHeaderObj);
	read(version, 1);
	read(flags, 3);
	fullHeaderObj.setFullBoxHeader(version, flags);
}

void ByteBuffer::readPartialFullBoxHeader(FullBoxHeader &fullHeaderObj)
{
	byte version[1];
	byte flags[3];
	read(version, 1);
	read(flags, 3);
	fullHeaderObj.setFullBoxHeader(version, flags);
}

bool ByteBuffer::verifyBoxType(std::string boxName)
{
	BoxHeader headerObj = BoxHeader();
	size_t backupPosition = getPosition();
	readBoxHeader(headerObj);
	setPosition(backupPosition);
	std::cout<<"verifyBoxType: "<<headerObj.getName()<<std::endl;
	if (headerObj.getName() == boxName) {
		return true;
	} 
	return false;
}

void ByteBuffer::ASCIIDump(size_t start, size_t end) 
{
	byte readBuff[4];
	size_t posBackup = getPosition();
	int i = 0;
	printf("ASCII DUMP:\n");
	while (getPosition() <= end) {
		read(readBuff, 4);
		printf("%x%x %x%x   [%c-%c-%c-%c] ", readBuff[0], readBuff[1], readBuff[2], readBuff[3],  readBuff[0], readBuff[1], readBuff[2], readBuff[3]);
		if (i % 4 == 0) {
			printf("\n");
		}
		i++;
	}
	printf("\n");
	setPosition(posBackup);

}
