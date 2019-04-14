#include "Box.h"
typedef uint8_t byte;

class ByteBuffer {
	byte *fptr;
	const size_t fileSize;
	size_t currPos;

	ByteBuffer(void *fptr, size_t fileSize);

	void reset();
	size_t getPosition() const;
	void setPosition(size_t pos);
	size_t read(byte *buff, size_t count);
};

class Mp4Parser {
	ByteBuffer fileBuffer;

	public:
	Mp4Parser(ByteBuffer fileBuffer);

	void parseMp4();
};
