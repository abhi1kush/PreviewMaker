#include "Box.h"
#include "byte_buffer.h"

class Mp4Parser {
	ByteBuffer fileByteBuffer;

	public:
	Mp4Parser(ByteBuffer &fileBufferArg) : fileByteBuffer(fileBufferArg)
	{
	}

	void parseMp4();
	void parseContainerBox(BoxHeader &headerObjArg);
};
