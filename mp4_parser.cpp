#include <cstdio>
#include "Box.h"
#include "mp4_parser.h"

bool Node::isContainer() 
{
	return 1 == header.isContainer();
}

bool Node::isFullBox()
{
	return 1 == header.isFullBox();
}

void Node::addParent(Node *parentNode) 
{
	parent = parentNode;
}

void Node::addNextNode(Node *nextNode)
{
	next = nextNode;
}

void Node::addChildNode(Node *childNode)
{
	child_box_list = childNode;
}

bool Node::hasNextNode()
{
	return NULL != next;
}

bool Node::hasChildNode()
{
	return NULL != child_box_list;
}

bool Node::hasParentNode()
{
	return NULL != parent;
}

Node* Node::parentNode() 
{
 	return parent;
}

Node* Node::nextNode()
{
	return next;
}

Node* Node::childNode()
{
	return child_box_list;
}

int Node::getLevel() 
{
	return level;
}

void Node::levelUp()
{
	level++;
}

void Node::levelDown()
{
	level--;
}

void Node::setOffset(uint64_t offset)
{
	this->offset = offset; 
}

uint64_t Node::getOffset()
{
	return offset;
}

int Node::getNodeLevel()
{
	return nodeLevel;
}

Node* MP4::createNode(uint64_t offset)
{
	Node* newNode = new Node(offset);
	if (NULL == ftyp) {
		ftyp = newNode;
	}
	return newNode;
}


bool MP4::isFirstNode()
{
	return NULL == lastAddedNode;
}

void MP4::setLastAddedNode(Node *node)
{
	lastAddedNode = node;
}

Node* MP4::getLastAddedNode()
{
	return lastAddedNode;
}

int MP4::getLastAddedNodeLevel()
{
	if (NULL == ftyp) {
		return Node::getLevel();
	}
	MP4_ASSERT((NULL != lastAddedNode),"lastAddedNode is null", return -1);
	if (NULL != lastAddedNode) {
		return lastAddedNode->getNodeLevel();
	}
}

int MP4::addNode(BoxHeader headerObj, uint64_t offset, int nodeLevel)
{
		Node *newNode = createNode(offset);
		newNode->header.setBoxHeader(headerObj);
		MP4_ASSERT((getLastAddedNodeLevel() == newNode->getNodeLevel()) 
				|| (getLastAddedNodeLevel() + 1 ==  newNode->getNodeLevel()), "Unsynched level", return -1);
		if (getLastAddedNodeLevel() == newNode->getNodeLevel())
		{
			lastAddedNode->addNextNode(newNode);
		}
		else if (getLastAddedNodeLevel() + 1 ==  newNode->getNodeLevel()) {
			lastAddedNode->addChildNode(newNode);
		}

		lastAddedNode = newNode;
}

void Mp4Parser::parseContainerBox(BoxHeader &headerObjArg)
{
	BoxHeader headerObj;
	FullBoxHeader fullHeaderObj;
	BoxHeader::addSpace();
	Node::levelUp();
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
	Node::levelDown();
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
