#include "../../includes.hpp"

using namespace TagLib;

MPEGMusicInternal::MPEGMusicInternal(TagLib::File* targetFile) :
	MusicInternal(targetFile), mpegFile(reinterpret_cast<MPEG::File*>(targetFile)) { }
MPEGMusicInternal::~MPEGMusicInternal(void) { }

std::string MPEGMusicInternal::tagType(void) {
	if (this->tagTypeName.empty() == false)
		return this->tagTypeName;

	if (this->mpegFile->hasAPETag()) {
		tagTypeName = "APE";
	}
	else if (this->mpegFile->hasID3v2Tag()) {
		ID3v2::Tag* tag = this->mpegFile->ID3v2Tag();
		auto* header = tag->header();

		this->tagTypeName = "ID3v2.";
		this->tagTypeName += std::to_string(header->majorVersion());
	}
	else if (this->mpegFile->hasID3v1Tag()) {
		tagTypeName = "ID3v1";
	}
	else {
		tagTypeName = "Unknown";
	}

	return this->tagTypeName;
}
