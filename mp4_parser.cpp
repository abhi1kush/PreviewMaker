#include <cstdio>
#include <stack>
#include "Box.h"
#include "mp4_parser.h"

#define BOTTUM_MARKER NULL

int Node::level;
uint32_t Node::nodeCount;

Node::Node()
{
	nodeLevel = level;
	parent = NULL;
	child_box_list = NULL;
	nodeCount++;
}

Node::Node(uint64_t offset) : Node()
{
	this->offset = offset;
}

Node::~Node()
{
	nodeCount--;
}

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

void Node::print()
{
	//printf("%d : ", nodeLevel);
	header.print();
}

uint32_t Node::getNodeCount()
{
	return nodeCount;
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
	MP4_ASSERT((NULL != lastAddedNode), return -1, "lastAddedNode is null");
	if (NULL != lastAddedNode) {
		return lastAddedNode->getNodeLevel();
	}
}

int MP4::addNode(BoxHeader headerObj, uint64_t offset, int nodeLevel)
{
		Node *newNode = createNode(offset);
		MP4_ASSERT((NULL != newNode), return -1, "addNode: newNode is null"); 
		newNode->header.setBoxHeader(headerObj);
		//TODO: remove this if cond 
		//and adjust (getLastAddedNodeLevel() + 1 ==  newNode->getNodeLevel()) 
		// to make it false at first node.
		if (isFirstNode()) {
			lastAddedNode = newNode;
			return MP4_ERR_OK;
		}
		//VID_LOG("parser", VID_INFO, "lastLevel %u curLevel %u\n",getLastAddedNodeLevel(), newNode->getNodeLevel());
		if (getLastAddedNodeLevel() == newNode->getNodeLevel())
		{
			//VID_LOG("parser", VID_INFO, "added next node\n");
			lastAddedNode->addNextNode(newNode);
		}
		else if (getLastAddedNodeLevel() + 1 ==  newNode->getNodeLevel()) {
			//VID_LOG("parser", VID_INFO, "added child node\n");
			levelStack.push(lastAddedNode);
			lastAddedNode->addChildNode(newNode);
		}
		else if (getLastAddedNodeLevel() > newNode->getNodeLevel()) {
			Node * temp = levelStack.top();
			levelStack.pop();
			temp->addNextNode(newNode);
			//VID_LOG("parser", VID_INFO, "added next node\n");
		}

		lastAddedNode = newNode;
		return MP4_ERR_OK;
}

void MP4::printHelper(Node* currNode)
{
	MP4_ASSERT((NULL != currNode), return , "printHelper: currNode is NULL");     	 printSpace(currNode->getNodeLevel());
	currNode->print();
	if (currNode->hasChildNode()) {
		printHelper(currNode->childNode());
	}
	if (currNode->hasNextNode()) {
		printHelper(currNode->nextNode());
	}
}

Node* MP4::getNext()
{
	Node* ret;
	if (NULL == ftyp) {
		return NULL;
	}

	if (iteratorStack.empty()) {
		iteratorStack.push(BOTTUM_MARKER);
		iteratorStack.push(ftyp);
	}

	ret = iteratorStack.top();
	iteratorStack.pop();

	if (BOTTUM_MARKER == ret) {
		MP4_ASSERT(iteratorStack.empty(), return NULL, "iteratorStack suppose to be empty.");
		return NULL;
	}
	
	MP4_ASSERT(NULL != ret, return NULL, "iteratorStack suppose to be empty.");
	
	if (ret->hasNextNode()) {
		iteratorStack.push(ret->nextNode());
	}

	if (ret->hasChildNode()) {
		iteratorStack.push(ret->childNode());
	}

	return ret;
}

bool MP4::hasNext()
{
	if (NULL == ftyp) {
		return NULL;
	}

	return !iteratorStack.empty();
}

void MP4::printSpace(int spaceCount) {
	for (int i = 0; i < spaceCount; i++) {
		printf("----|");
	}
}

void MP4::print()
{
	printf("Node Count: %u\n", Node::getNodeCount());
	printHelper(ftyp);
}

void Mp4Parser::print()
{
	mp4Obj.print();
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
		
		mp4Obj.addNode(headerObj, posBackup, Node::getLevel());
		
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
		
		mp4Obj.addNode(headerObj, posBackup, Node::getLevel());
		
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
