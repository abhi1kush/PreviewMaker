#include "stbl_handler.h"
#include "log.h"
#include "mp4_err.h"

#define MODULE_NAME "stbl"

err_t STSS::populateStss(ByteBuffer &stblBuffer, uint8_t stssOffset)
{
	stblBuffer.setPosition(stssOffset);
	stblBuffer.readFullBoxHeader(fullHeader);
	entryCount = stblBuffer.readUint32();
	for (uint32_t i = 0; i < entryCount; i++) {
		uint32_t iFrameSequence = stblBuffer.readUint32();
		iFrameSequenceVec.push_back(iFrameSequence);
	}
	return MP4_ERR_OK;
}

void STSS::print()
{
	for (uint32_t i = 0; i < iFrameSequenceVec.size(); i++) {
		PRINT_MSG("[%u: %u] ", i, iFrameSequenceVec[i]);	
	}
	PRINT_MSG("\n");
}

err_t STSC::populateStsc(ByteBuffer &stblBuffer, uint8_t stscOffset)
{
	stblBuffer.setPosition(stscOffset);
	stblBuffer.readFullBoxHeader(fullHeader);
	entryCount = stblBuffer.readUint32();
	for (uint32_t i = 0; i < entryCount; i++) {
		StscEntry_T stscEntry;
		stscEntry.firstChunk = stblBuffer.readUint32();
		stscEntry.samplesPerChunk = stblBuffer.readUint32();
		stscEntry.sampleDescriptionIndex = stblBuffer.readUint32();
		runOfChunksVec.push_back(stscEntry);
	}
	return MP4_ERR_OK;
}

void STSC::print()
{
	PRINT_MSG("[firstChunk samplesPerChunk sampleDesIdx]\n");	
	for (uint32_t i = 0; i < runOfChunksVec.size(); i++) {
		PRINT_MSG("[%u: %u %u %u] ", i, runOfChunksVec[i].firstChunk,
				runOfChunksVec[i].samplesPerChunk,
				runOfChunksVec[i].sampleDescriptionIndex);	
	}
	PRINT_MSG("\n");
}

err_t STCO::populateStco(ByteBuffer &stblBuffer, uint8_t stcoOffset)
{
	stblBuffer.setPosition(stcoOffset);
	stblBuffer.readFullBoxHeader(fullHeader);
	entryCount = stblBuffer.readUint32();
	for (uint32_t i = 0; i < entryCount; i++) {
		uint32_t frameAbsOffset = stblBuffer.readUint32();
		frameAbsOffsetVec.push_back(frameAbsOffset);
	}
	return MP4_ERR_OK;
	
}

void STCO::print()
{
	for (uint32_t i = 0; i < frameAbsOffsetVec.size(); i++) {
		PRINT_MSG("[%u: %u] ", i, frameAbsOffsetVec[i]);
	}
	PRINT_MSG("\n");
}

err_t CO64::populateCo64(ByteBuffer &stblBuffer, uint8_t co64Offset)
{
	stblBuffer.setPosition(co64Offset);
	stblBuffer.readFullBoxHeader(fullHeader);
	entryCount = stblBuffer.readUint32();
	for (uint32_t i = 0; i < entryCount; i++) {
		uint64_t frameAbsOffset = stblBuffer.readUint64();
		frameAbsOffsetVec.push_back(frameAbsOffset);
	}
	return MP4_ERR_OK;
}

void CO64::print()
{
	for (uint32_t i = 0; i < frameAbsOffsetVec.size(); i++) {
		PRINT_MSG("[%u: %lu] ", i, frameAbsOffsetVec[i]);
	}
	PRINT_MSG("\n");
}

err_t STBL::parseStbl()
{
	MP4_ASSERT((stblBuffer.getPosition() == offset), "(stblBuffer.getPosition() == offset)", return MP4_PARSE_FAILED);
	while (stblBuffer.getPosition() <= (offset + size)) {
		
	}
}

err_t STBL::populateSubBoxOffset()
{

}

err_t STBL::populateStbl()
{
	err_t ret;
	MP4_ASSERT(stblBuffer.verifyBoxType("stbl"), "stbl verifyBoxType failed", return MP4_PARSE_FAILED);
	ret = populateSubBoxOffset();
	if ((stssOffset == INVALID_OFFSET) 
		|| (stscOffset == INVALID_OFFSET) 
		|| (stcoOffset == INVALID_OFFSET)) {
		VID_LOG("stbl", VID_ERROR, "Offset of subBoxes in stbl are invalid");
		return MP4_INVAL_OFFSET;
	}
	ret = stssObj.populateStss(stblBuffer, stssOffset);
	RETURN_ON_FAIL(MP4_ERR_OK != ret, ret);
	ret = stscObj.populateStsc(stblBuffer, stscOffset);
	RETURN_ON_FAIL(MP4_ERR_OK != ret, ret);
	ret = stcoObj.populateStco(stblBuffer, stcoOffset);
	RETURN_ON_FAIL(MP4_ERR_OK != ret, ret);
	return MP4_ERR_OK;
}

void STBL::print()
{
	stssObj.print();
	stscObj.print();
	stcoObj.print();
}
