#include <cstdio>
#include "Box.h"
#include "mp4_parser.h"

void Mp4Parser::parseMp4()
{
	uint32_t size;
	byte name[4];
	size = fileByteBuffer.readUint32();
	fileByteBuffer.read(name, 4);
	printf("WIP %u %s\n", size, name);
}
