#ifndef BOX_H_
#define BOX_H_

#include <string>
#include <unordered_map>
#include "common.h"
#include "mp4_err.h"

#define SIMPLE_BOX 0
#define CONTAINER_BOX 0x1
#define FULLBOX 0x2

#define INVALID_BOX_NAME "invl"

#define TRUE 1
#define FALSE 0
#define ERROR -1

std::string convertBoxNameToString(byte *name);

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
		MP4_ASSERT((i >=0 && i <= 3), return 0, "getNameChar out of index");
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

class TKHD {
	uint32_t reserved[2];
	int16_t layer;
        int16_t alternate_group;
	int16_t volume;
	uint16_t reserved2;
	int32_t matrix[9];
	uint32_t width;
	uint32_t height;
};

class TKHD32 : TKHD{
	public:
	FullBoxHeader header;
	uint32_t creation_time;
	uint32_t modification_time;
	uint32_t track_ID;
	uint32_t reserved1;
	uint32_t duration;
};

class TKHD64 : TKHD{
	public:
	FullBoxHeader header;
	uint64_t creation_time;
	uint64_t modification_time;
	uint32_t track_ID;
	uint32_t reserved1;
	uint64_t duration;
};

typedef struct _lang_decode
{
	uint8_t bit:1;
	uint8_t lang1:5;
	uint8_t lang2:5;
	uint8_t lang3:5;
}lang_decode_t;

typedef union _mdhd_lang_decode
{
	byte language[2];
	lang_decode_t st_lang;
}mdhd_lang_decode_t;

class MDHD_COMMON {
	public:
	//bit(1) pad = 0;
	byte language[2]; //uint5_t langauge[3] 
	uint16_t pre_defined;
};

class MDHD64 : public MDHD_COMMON {
	uint64_t creation_time;
	uint64_t modification_time;
	uint32_t timescale;
	uint64_t duration;
};

class MDHD32 : public MDHD_COMMON {
	uint32_t creation_time;
	uint32_t modification_time;
	uint32_t timescale;
	uint32_t duration;
};

class HDLR {
	public:
	FullBoxHeader header;
	uint32_t pre_defined;
	byte handler_type[4];
	uint32_t reserved[3];
	byte name;	
};


#endif /* BOX_H_ */
