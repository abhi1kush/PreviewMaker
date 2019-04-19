#include "common.h"
#include "byte_buffer.h"
#include "mp4_err.h"
#include <vector>

//contains Total IFrame count. Stores serial number of IFrames.
class STSS {
	uint32_t offset;
	FullBoxHeader fullHeader;
	uint32_t entryCount;
	std::vector<uint32_t> iFrameSequenceVec;
	public:
	err_t populateStss(ByteBuffer &stblBuffer, uint8_t offset);
};

typedef struct _StscEntry
{
	uint32_t firstChunk;
	uint32_t samplesPerChunk;
	uint32_t sampleDescriptionIndex;
}StscEntry_T;
//Contains Table of Run of Chunks. entry(first_chunk, frame_per_chunk, description_Box)
class STSC {
	uint32_t offset;
	FullBoxHeader fullHeader;
	uint32_t totalFrameCount;
	uint32_t entryCount;
	std::vector<StscEntry_T> runOfChunksVec;
	public:
	STSC();
	STSC(uint32_t totalFrameCount);
	err_t populateStsc(ByteBuffer &stblBuffer, uint8_t offset);
};

//stco box contains absolute offset of chunks.
class STCO {
	uint32_t offset;
	FullBoxHeader fullHeader;
	uint32_t entryCount;
	std::vector<uint32_t> frameAbsOffsetVec;
	public:
	err_t populateStco(ByteBuffer &stblBuffer, uint8_t offset);
};

//Its better to use it directly from file for Large Size videos.
class STSZ {
	uint32_t offset;
};

class CO64 {
	uint32_t offset;
	FullBoxHeader fullHeader;
	uint32_t entryCount;
	std::vector<uint64_t> frameAbsOffsetVec;
	public:
	err_t populateCo64(ByteBuffer &stblBuffer, uint8_t offset);
};

class STBL {
	ByteBuffer stblBuffer;
	uint32_t offset;
	BoxHeader header;
	STSS stssObj;
	STSC stscObj;
	STSZ stszObj;
	union annon { 
		STCO stcoObj;
		CO64 co64Obj;
	};
	public:
	STBL(void *fptrArg, size_t fileSizeArg, size_t stblOffset) : stblBuffer(fptrArg,fileSizeArg, stblOffset), offset(stblOffset) 
	{}
	err_t populateStbl();
};

typedef struct _SttsEntry
{
	uint32_t sampleCount;
	uint32_t sampleDelta;
}SttsEntry_T;

class STTS {
	FullBoxHeader fullHeader;
	uint32_t entryCount;
	std::vector<SttsEntry_T> deltaVec;
	uint32_t totalFrameCount; //calculate and store.
	public:
	void populateStts();
};

class StblHandler {
	ByteBuffer fileByteBuffer;
	public:
	err_t stscHandler(); //OUT chunk details arr.
	err_t stszHandler(); //IN chunk details arr. OUT chunk run array, IFrame size arr, stszObj.
	err_t stcoHandler(); //OUT chunk offset array.
	err_t stssHandler(); //OUT IFrame seq number array, stssObj.
	err_t sttsHandler(); //OUT delta array.
	err_t populateStbl(OUT STBL &stblObj);
		
};

class IFrame {
	ByteBuffer fileByteBuffer;
	uint8_t mdatCount;
	uint8_t trakCount;
	public:
	err_t extractIFrames(IN STBL &stblObj);
	/* for (Iterate on runs of chunks (stsc)) {
	 * 	for (Iterate on particular chunk) {
	 * 		(stco)
	 * 		while (Traverse each frame) {
	 * 			recognize IFrame and copy.
	 * 		}
	 * 	}
	 * }
	 *
	 * */
};


