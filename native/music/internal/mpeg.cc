#include "../../includes.hpp"

using MPEGFile = TagLib::MPEG::File;
using ID3v2Tag = TagLib::ID3v2::Tag;

MPEGMusicInternal::MPEGMusicInternal(const TagLib::FileRef* file) :
	MusicInternal(file) {
	this->registerAllId3v2Resolvers();

	this->file = reinterpret_cast<MPEGFile*>(file->file());
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

void MPEGMusicInternal::initializeId3v1(void) {
	this->tagTypeString = "ID3v1";
}
void MPEGMusicInternal::initializeAPE(void) {
	this->tagTypeString = "APE";
}

NodeBase::generator_holder MPEGMusicInternal::extractDataFromFrame(const std::string& type, ID3v2Frame* frame) {
	TagLib::ByteVector byteVector = frame->frameID();
	std::string frameId(byteVector.data(), byteVector.size());
	NodeBase::generator_holder holder;
	
	const auto iter = this->resolverMap.find(type + "::" + frameId);
	if (iter != this->resolverMap.cend()) {
		holder = iter->second(frame);
	} else {
		holder = frame->toString();
	}

	return holder;
}

const NodeString& MPEGMusicInternal::tagType(void) {
	return this->tagTypeString;
}

const NodeObject& MPEGMusicInternal::nativeData(void) {
	return this->nativeDataStore;
}
