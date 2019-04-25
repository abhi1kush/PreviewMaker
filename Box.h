#ifndef BOX_H_
#define BOX_H_

#include <string>
#include <unordered_map>
#include "common.h"
#include "mp4_err.h"

#define SIMPLE_BOX 0
#define CONTAINER_BOX 0x1
#define FULLBOX 0x2

class BoxInfo {
	std::string name;
	uint8_t flag;
	public:
	BoxInfo(std::string name, uint8_t flag) {
		this->name = name;
		this->flag = flag;
	}
	
	bool isContainer() const
	{
		return flag & CONTAINER_BOX ? true : false;
	}

	bool isFullBox() const
	{
		return flag & FULLBOX ? true : false;
	}

};

static const std::unordered_map<std::string, BoxInfo> boxMap = {
	{"ftyp", BoxInfo("ftyp", SIMPLE_BOX)},
	{"pdin", BoxInfo("pdin", FULLBOX)},
	{"moov", BoxInfo("moov", CONTAINER_BOX)}, //container
	{"mvhd", BoxInfo("mvhd", FULLBOX)},
	{"trak", BoxInfo("trak", CONTAINER_BOX)}, //container
	{"tkhd", BoxInfo("tkhd", FULLBOX)},
	{"tref", BoxInfo("tref", SIMPLE_BOX)},
	{"edts", BoxInfo("edts", CONTAINER_BOX)}, //container
	{"elst", BoxInfo("elst", FULLBOX)},
	{"mdia", BoxInfo("mdia", CONTAINER_BOX)}, //container
	{"mdhd", BoxInfo("mdhd", FULLBOX)},
	{"hdlr", BoxInfo("hdlr", FULLBOX)},
	{"minf", BoxInfo("minf", CONTAINER_BOX)}, //container
	{"vmhd", BoxInfo("vmhd", FULLBOX)},
	{"smhd", BoxInfo("smhd", FULLBOX)},
	{"hmhd", BoxInfo("hmhd", FULLBOX)},
	{"nmhd", BoxInfo("nmhd", FULLBOX)},
	{"dinf", BoxInfo("dinf", CONTAINER_BOX)}, //container
	{"dref", BoxInfo("dref", FULLBOX)},
	{"stbl", BoxInfo("stbl", CONTAINER_BOX)}, //container
	{"stsd", BoxInfo("stsd", FULLBOX)},
	{"stts", BoxInfo("stts", FULLBOX)},
	{"stsc", BoxInfo("stsc", FULLBOX)},
	{"ctts", BoxInfo("ctts", FULLBOX)},
	{"stsz", BoxInfo("stsz", FULLBOX)},
	{"stz2", BoxInfo("stz2", FULLBOX)},
	{"stco", BoxInfo("stco", FULLBOX)},
	{"co64", BoxInfo("co64", FULLBOX)},
	{"stss", BoxInfo("stss", FULLBOX)},
	{"stsh", BoxInfo("stsh", FULLBOX)},
	{"padb", BoxInfo("padb", FULLBOX)},
	{"stdp", BoxInfo("stdp", FULLBOX)},
	{"sbgp", BoxInfo("sbgp", FULLBOX)},
	{"sgpd", BoxInfo("sgpd", FULLBOX)},
	{"subs", BoxInfo("subs", SIMPLE_BOX)},
	{"mvex", BoxInfo("mvex", CONTAINER_BOX)}, //container
	{"mehd", BoxInfo("mehd", FULLBOX)},
	{"trex", BoxInfo("trex", FULLBOX)},
	{"ipmc", BoxInfo("ipmc", SIMPLE_BOX)},
	{"moof", BoxInfo("moof", CONTAINER_BOX)}, //container
	{"mfhd", BoxInfo("mfhd", FULLBOX)},
	{"traf", BoxInfo("traf", CONTAINER_BOX)}, //container
	{"tfhd", BoxInfo("tfhd", FULLBOX)},
	{"trun", BoxInfo("trun", FULLBOX)},
	{"sdtp", BoxInfo("sdtp", FULLBOX)},
	{"sdgp", BoxInfo("sdgp", SIMPLE_BOX)},
	{"subs", BoxInfo("subs", FULLBOX)},
	{"mfra", BoxInfo("mfra", CONTAINER_BOX)}, //container
	{"tfra", BoxInfo("tfra", FULLBOX)},
	{"mfro", BoxInfo("mfro", FULLBOX)},
	{"mdat", BoxInfo("mdat", SIMPLE_BOX)},
	{"free", BoxInfo("free", SIMPLE_BOX)},
	{"skip", BoxInfo("skip", CONTAINER_BOX)}, //container
	{"udta", BoxInfo("udta", CONTAINER_BOX)}, //container
	{"cprt", BoxInfo("cprt", FULLBOX)},
	{"meta", BoxInfo("meta", CONTAINER_BOX | FULLBOX)}, //container
	{"hdlr", BoxInfo("hdlr", SIMPLE_BOX)},
	{"ilst", BoxInfo("ilst", CONTAINER_BOX)}, //container
	{"gsst", BoxInfo("gsst", SIMPLE_BOX)},
	{"gstd", BoxInfo("gstd", SIMPLE_BOX)},
	{"ipmc", BoxInfo("ipmc", FULLBOX)},
	{"iloc", BoxInfo("iloc", FULLBOX)},
	{"ipro", BoxInfo("ipro", CONTAINER_BOX | FULLBOX)}, //container
	{"sinf", BoxInfo("sinf", CONTAINER_BOX)}, //container
	{"frma", BoxInfo("frma", SIMPLE_BOX)},
	{"imif", BoxInfo("imif", FULLBOX)},
	{"schm", BoxInfo("schm", FULLBOX)},
	{"schi", BoxInfo("schi", SIMPLE_BOX)},
	{"iinf", BoxInfo("iinf", FULLBOX)},
	//{"xml", BoxInfo("xml",  false, true)}, only box name with 3 length, not required now
	{"bxml", BoxInfo("bxml", FULLBOX)},
	{"pitm", BoxInfo("pitm", FULLBOX)}
	//srpp fullbox
};

class BoxHeader {
	uint32_t size;
	byte name[4];
        static uint8_t spaceCount;
	size_t offset;
	public:
	BoxHeader();
	BoxHeader(uint32_t size, byte *name);

	//methods
	void setBoxHeader(uint32_t size, byte *name, size_t offset);
	void setBoxHeader(BoxHeader);
	void printHeader();
	virtual void print();
	int isContainer();
	int isFullBox();
	int isValidBox();
	std::string getName();
	uint32_t getSize() const {
		return size;
	}
	size_t getOffset() {
		return offset;
	}
	byte getNameChar(uint8_t i) {
		MP4_ASSERT((i >=0 && i <= 3), "getNameChar out of index", return 0);
		return name[i];
	}

	static void printSpace();
	static void addSpace();
	static void delSpace();

	virtual ~BoxHeader();
};

class FullBoxHeader : public BoxHeader{
	byte version[1];
	byte boxFlags[3];

	public:
	FullBoxHeader();
	FullBoxHeader(byte *version, byte *boxFlags);

	void setFullBoxHeader(byte *version, byte *boxFlags);
	void printFull();
	//virtual void print();
};

#endif /* BOX_H_ */
