#include "../../includes.hpp"

using TagLib::MPEG::File;
using ID3v2Tag = TagLib::ID3v2::Tag;

std::unordered_map<ID3v2AttachedPictureFrame::Type, const char*> MPEGMusicInternal::idv2PictureTypeDictionary = {
	{ ID3v2AttachedPictureFrame::Other, "Other" },
	{ ID3v2AttachedPictureFrame::FileIcon, "File Icon" },
	{ ID3v2AttachedPictureFrame::OtherFileIcon, "Other file icon" },
	{ ID3v2AttachedPictureFrame::FrontCover, "Cover (front)" },
	{ ID3v2AttachedPictureFrame::BackCover, "Cover (back)" },
	{ ID3v2AttachedPictureFrame::LeafletPage, "Leaflet page" },
	{ ID3v2AttachedPictureFrame::Media, "Media (e.g. lable side of CD)" },
	{ ID3v2AttachedPictureFrame::LeadArtist, "Lead artist/lead performer/soloist" },
	{ ID3v2AttachedPictureFrame::Artist, "Artist/performer" },
	{ ID3v2AttachedPictureFrame::Conductor, "Conductor" },
	{ ID3v2AttachedPictureFrame::Band, "Band/Orchestra" },
	{ ID3v2AttachedPictureFrame::Composer, "Composer" },
	{ ID3v2AttachedPictureFrame::Lyricist, "Lyricist/text writer" },
	{ ID3v2AttachedPictureFrame::RecordingLocation, "Recording Location" },
	{ ID3v2AttachedPictureFrame::DuringRecording, "During recording" },
	{ ID3v2AttachedPictureFrame::DuringPerformance, "During performance" },
	{ ID3v2AttachedPictureFrame::MovieScreenCapture, "Movie/video screen capture" },
	{ ID3v2AttachedPictureFrame::ColouredFish, "A bright coloured fish" },
	{ ID3v2AttachedPictureFrame::Illustration, "Illustration" },
	{ ID3v2AttachedPictureFrame::BandLogo, "Band/artist logotype" },
	{ ID3v2AttachedPictureFrame::PublisherLogo, "Publisher/Studio logotype" },
};

MPEGMusicInternal::MPEGMusicInternal(const TagLib::FileRef* file) :
	MusicInternal(file) {
	this->resolverMap["ID3v2::APIC"] = std::bind(&MPEGMusicInternal::resolveId3v2AttachedPicture, this, std::placeholders::_1);
	this->resolverMap["ID3v2::PRIV"] = std::bind(&MPEGMusicInternal::resolveId3v2Private, this, std::placeholders::_1);
	this->resolverMap["ID3v2::CHAP"] = std::bind(&MPEGMusicInternal::resolveId3v2Chapter, this, std::placeholders::_1);

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
