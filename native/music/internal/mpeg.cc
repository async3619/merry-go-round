#include "../../includes.hpp"

using TagLib::MPEG::File;
using ID3v2Tag = TagLib::ID3v2::Tag;

MPEGMusicInternal::MPEGMusicInternal(const TagLib::FileRef* file) :
	MusicInternal(file) {
	this->file = reinterpret_cast<File*>(file->file());
	this->initialize();
}
MPEGMusicInternal::~MPEGMusicInternal(void) {}

void MPEGMusicInternal::initialize(void) {
	if (this->file->hasID3v2Tag()) {
		this->initializeId3v2();
	} else if (this->file->hasID3v1Tag()) {

	} else if (this->file->hasAPETag()) {
	} else {
		this->tagTypeString = "Unknown";
	}
}

void MPEGMusicInternal::initializeId3v2(void) {
	auto* tag = this->file->ID3v2Tag();
	auto* header = tag->header();

	this->tagTypeString = "ID3v2." + std::to_string(header->majorVersion());

	const auto& frameList = tag->frameList();
	for (auto* frame : frameList) {		
		char buffer[5] = { 0, };
		std::memcpy(buffer, frame->frameID().data(), frame->frameID().size());

		this->nativeDataStore[buffer] = frame->toString();
	}
}
void MPEGMusicInternal::initializeId3v1(void) {
	this->tagTypeString = "ID3v1";
}
void MPEGMusicInternal::initializeAPE(void) {
	this->tagTypeString = "APE";
}

const NodeString& MPEGMusicInternal::tagType(void) {
	return this->tagTypeString;
}

const NodeObject& MPEGMusicInternal::nativeData(void) {
	return this->nativeDataStore;
}
