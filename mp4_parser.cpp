#include <cstdio>
#include "Box.h"
#include "mp4_parser.h"

void Mp4Parser::parseMp4()
{
	byte size[4];
	uint32_t size2 = 0;
	byte name[4];
	fileBuffer.read((byte *)&size2, 4);
	fileBuffer.read(name, 4);
	printf("WIP %u %s\n", size2, name);
}
