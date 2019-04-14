#include <cstring>
#include <iostream>
#include "Box.h"

BoxHeader::BoxHeader() {
	this->name[0] = 'i';
	this->name[1] = 'n';
	this->name[2] = 'v';
	this->name[3] = 'l';
}

BoxHeader::BoxHeader(uitn32_t size, byte *name) {
	this->size = size;
	this->name[0] = name[0];
	this->name[1] = name[1];
	this->name[2] = name[2];
	this->name[3] = name[3];
}

void BoxHeader::printHeader() {
	std::cout<<size<<" "<<name;
}

BoxHeader::~BoxHeader() {
	// TODO Auto-generated destructor stub
}

FullBoxHeader::FullBoxHeader() {
	this->version = 0;
	this->boxFlags[0] = 0;
	this->boxFlags[1] = 0;
	this->boxFlags[2] = 0;
}

FullBoxHeader::FullBoxHeader(uint8_t &version, uint8_t *boxFlags) {
	this->version = version;
	this->boxFlags[0] = boxFlags[0];
	this->boxFlags[1] = boxFlags[1];
	this->boxFlags[2] = boxFlags[2];
}

int isContainer(std::string &name) {
	auto it = boxMap.find(name);
	if (it != boxMap.end()) {
		return it->second.isContainer() ? 1 : 0;
	}
	return -1;
}

int isContainer(unsigned char *nameCharArr) {
	unsigned char tempArr[5];
	tempArr[0] = nameCharArr[0];
	tempArr[1] = nameCharArr[1];
	tempArr[2] = nameCharArr[2];
	tempArr[3] = nameCharArr[3];
	tempArr[4] = '\0';
	std::string name((char *)tempArr);
	auto it = boxMap.find(name);
	if (it != boxMap.end()) {
		return it->second.isContainer() ? 1 : 0;
	}
	return -1;
}

int isFullBox(std::string &name) {
	auto it = boxMap.find(name);
	if (it != boxMap.end()) {
		return (it->second.isFullBox()) ? 1 : 0;
	}
	return -1;
}

int isValidBox(std::string &name) {
	if (boxMap.find(name) != boxMap.end()) {
		return 1;
	}
	return -1;
}
