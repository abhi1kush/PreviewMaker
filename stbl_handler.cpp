#include "stbl_handler.h"

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

err_t STBL::populateStbl()
{

}
