#include "Box.h"
#include "byte_buffer.h"

class Mp4Parser {
	ByteBuffer fileBuffer;

	public:
	Mp4Parser(ByteBuffer &fileBufferArg) : fileBuffer(fileBufferArg)
	{
	}

	void parseMp4();
};
