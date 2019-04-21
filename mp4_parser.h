#include "Box.h"
#include "byte_buffer.h"

class Node
{
	private:
	Node *parent;
	Node *child_box_list;
	Node *next;
	uint64_t offset;
	public:
	static uint32_t nodeCount;
	Node()
	{
		parent = NULL;
		child_box_list = NULL;
		next = NULL;
		nodeCount++;
	}
	~Node()
	{
		nodeCount--;
	}
	BoxHeader header;
	bool isContainer;
	bool isFullBox;
};

class MP4 {
	private:
	Node* createNode();	
	public:
	Node *ftyp; //First mandatory box, will be used as root.
        Node* saerchBox(std::string boxName);
	Node* addNextNode();
	Node* addChildNode();

};

class Mp4Parser {
	ByteBuffer fileByteBuffer;

	public:
	Mp4Parser(ByteBuffer &fileBufferArg) : fileByteBuffer(fileBufferArg)
	{
	}

	void parseMp4();
	void parseContainerBox(BoxHeader &headerObjArg);
};
