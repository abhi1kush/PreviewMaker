#include <stack>
#include "Box.h"
#include "byte_buffer.h"

class Node
{
	private:
	Node *parent;
	Node *child_box_list;
	Node *next;
	uint64_t offset;
	uint8_t boxFrequency;
	int nodeLevel;
	static int level;
	static uint32_t nodeCount;
	public:
	Node();
	Node(uint64_t offset);
	~Node();
	BoxHeader header;
	void print();
	void setOffset(uint64_t offset);
	uint64_t getOffset();
	bool isContainer();
	bool isFullBox();
	void addParent(Node *parentNode);
	void addNextNode(Node *nextNode);
	void addChildNode(Node * childNode);
	bool hasNextNode();
	bool hasChildNode();
	bool hasParentNode();
	Node* nextNode();
	Node* parentNode();
	Node* childNode();
	int getNodeLevel();
	static int getLevel();
	static void levelUp();
	static void levelDown();
	static uint32_t getNodeCount();

};

class MP4 {
	private:
	std::stack <Node *> iteratorStack;
	void printHelper(Node* currNode);
	std::stack <Node *> levelStack;
	public:
	Node *lastAddedNode;
	Node *ftyp; //First mandatory box, will be used as root.
        Node* saerchBox(std::string boxName);
	MP4()
	{
		ftyp = NULL; 
		lastAddedNode = NULL;
	}
	Node* createNode(uint64_t offset);
	bool hasNext();	
	bool isFirstNode();
	void setLastAddedNode(Node *node);
	Node* getLastAddedNode();
	int getLastAddedNodeLevel();
	int addNode(BoxHeader headerObj, uint64_t offset, int nodeLevel);
	void print();
	Node* getNext();
	static void printSpace(int spaceCount);
};

class Mp4Parser {
	ByteBuffer fileByteBuffer;
	MP4 mp4Obj;

	public:
	Mp4Parser(ByteBuffer &fileBufferArg) : fileByteBuffer(fileBufferArg)
	{
		mp4Obj = MP4();
	}

	void parseMp4();
	void parseContainerBox(BoxHeader &headerObjArg);
	void print();
};
