/*
 * Box.cpp
 *
 *  Created on: 13-Apr-2019
 *      Author: Abhishek
 */

#include <cstring>
#include "Box.h"
BoxHeader::BoxHeader() {
	this->name[0] = 'i';
	this->name[1] = 'n';
	this->name[2] = 'v';
	this->name[3] = 'l';
	this->flag = 0;
}

BoxHeader::BoxHeader(unsigned char *name, uint8_t &flag) {
	this->name[0] = name[0];
	this->name[1] = name[1];
	this->name[2] = name[2];
	this->name[3] = name[3];
	this->flag = flag;
}

BoxHeader::print() {
	cout<<this->name;
	cout<<" "<<
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

