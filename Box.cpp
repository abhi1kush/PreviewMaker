#include <cstring>
#include <iostream>
#include "Box.h"

static inline std::string convertBoxNameToString(byte *name)
{
	char tmpName[5];
	tmpName[0] = name[0];
	tmpName[1] = name[1];
	tmpName[2] = name[2];
	tmpName[3] = name[3];
	tmpName[4] = '\0';
	std::string nameStr(tmpName);
	return nameStr;
}

BoxHeader::BoxHeader() {
	this->name[0] = 'i';
	this->name[1] = 'n';
	this->name[2] = 'v';
	this->name[3] = 'l';
}

BoxHeader::BoxHeader(uint32_t size, byte *name) {
	this->size = size;
	this->name[0] = name[0];
	this->name[1] = name[1];
	this->name[2] = name[2];
	this->name[3] = name[3];
}

void BoxHeader::printHeader() {
	printSpace();
	fwrite((char *)name, sizeof(byte), 4, stdout);	
	printf(" %u\n", size);
}

void BoxHeader::print() {
	printHeader();
}

void BoxHeader::setBoxHeader(uint32_t size, byte *name, size_t offset)
{
	this->offset = offset;
	this->size = size;
	this->name[0] = name[0];
	this->name[1] = name[1];
	this->name[2] = name[2];
	this->name[3] = name[3];
}

void BoxHeader::setBoxHeader(BoxHeader headerObj)
{
	this->offset = headerObj.getOffset();
	this->size = headerObj.getSize();
	this->name[0] = headerObj.getNameChar(0);
	this->name[1] = headerObj.getNameChar(1);
	this->name[2] = headerObj.getNameChar(2);
	this->name[3] = headerObj.getNameChar(3);
}

std::string BoxHeader::getName()
{
	return convertBoxNameToString(name);
}

BoxHeader::~BoxHeader() {
	// TODO Auto-generated destructor stub
}

FullBoxHeader::FullBoxHeader() {
	this->version[0] = 0;
	this->boxFlags[0] = 0;
	this->boxFlags[1] = 0;
	this->boxFlags[2] = 0;
}

FullBoxHeader::FullBoxHeader(byte *version, byte *boxFlags) {
	this->version[0] = version[0];
	this->boxFlags[0] = boxFlags[0];
	this->boxFlags[1] = boxFlags[1];
	this->boxFlags[2] = boxFlags[2];
}

void FullBoxHeader::setFullBoxHeader(byte *version, byte *boxFlags) {
	this->version[0] = version[0];
	this->boxFlags[0] = boxFlags[0];
	this->boxFlags[1] = boxFlags[1];
	this->boxFlags[2] = boxFlags[2];
}


int BoxHeader::isContainer() {
	auto it = boxMap.find(convertBoxNameToString(name));
	if (it != boxMap.end()) {
		return it->second.isContainer() ? 1 : 0;
	}
	return -1;
}

int BoxHeader::isFullBox() {
	auto it = boxMap.find(convertBoxNameToString(name));
	if (it != boxMap.end()) {
		return (it->second.isFullBox()) ? 1 : 0;
	}
	return -1;
}

int BoxHeader::isValidBox() {
	std::string nameStr = convertBoxNameToString(name);
	if (boxMap.find(nameStr) != boxMap.end()) {
		return 1;
	}
	return -1;
}

uint8_t BoxHeader::spaceCount = 0;

void BoxHeader::printSpace() {
	for (int i = 0; i < spaceCount; i++) {
		printf("----|");
	}
}

void BoxHeader::addSpace() {
	spaceCount++;
}

void BoxHeader::delSpace() {
	spaceCount--;
}
