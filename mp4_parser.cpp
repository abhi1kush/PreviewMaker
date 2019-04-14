#include <cstdio>
#include "Box.h"
#include "mp4_parser.h"

void Mp4Parser::parseContainerBox(BoxHeader &headerObjArg)
{
	BoxHeader headerObj;
	FullBoxHeader fullHeaderObj;
	BoxHeader::addSpace();
	while(fileByteBuffer.getPosition() < headerObjArg.getOffset() + headerObjArg.getSize()) {
		size_t posBackup = fileByteBuffer.getPosition();
		fileByteBuffer.readBoxHeader(headerObj);
		headerObj.printHeader();
		if (1 != headerObj.isValidBox()) {
			printf("Invalid Box : ");
			headerObj.printHeader();
			break;
		}
		if (1 == headerObj.isFullBox()) {
			fileByteBuffer.readPartialFullBoxHeader(fullHeaderObj);
		}
		if (1 == headerObj.isContainer()) {
			parseContainerBox(headerObj);
		} else {
			size_t haveRead = fileByteBuffer.getPosition() - posBackup;
			fileByteBuffer.setPositionRel(headerObj.getSize() - haveRead);
		}
		if (fileByteBuffer.getPosition() == posBackup) {
			printf("Breaking infinite while loop\n");
			break;
		}
	}
	BoxHeader::delSpace();
}

void Mp4Parser::parseMp4()
{
	BoxHeader headerObj;
	FullBoxHeader fullHeaderObj;
	while(fileByteBuffer.getPosition() < fileByteBuffer.size()) {
		size_t posBackup = fileByteBuffer.getPosition();
		fileByteBuffer.readBoxHeader(headerObj);
		headerObj.printHeader();
		if (1 != headerObj.isValidBox()) {
			printf("Invalid Box : ");
			headerObj.printHeader();
			break;
		}
		if (1 == headerObj.isFullBox()) {
			fileByteBuffer.readPartialFullBoxHeader(fullHeaderObj);
		}
		if (1 == headerObj.isContainer()) {
			parseContainerBox(headerObj);
		} else {
			size_t haveRead = fileByteBuffer.getPosition() - posBackup;
			fileByteBuffer.setPositionRel(headerObj.getSize() - haveRead);
		}
		if (fileByteBuffer.getPosition() == posBackup) {
			printf("Breaking infinite while loop\n");
			break;
		}
	}
}
