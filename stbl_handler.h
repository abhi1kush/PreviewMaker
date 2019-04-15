#include "common.h"
#include "byte_buffer.h"
#include "mp4-err.h"

//contains Total IFrame count. Stores serial number of IFrames.
class STSS {
};

//Contains Table of Run of Chunks. entry(first_chunk, frame_per_chunk, description_Box)
class STSC {
};

//stco box contains absolute offset of chunks.
class STCO {
};

//Its better to use it directly from file for Large Size videos.
class STSZ {
};

class CO64 {
};

class STBL {
};

class STTS {
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


