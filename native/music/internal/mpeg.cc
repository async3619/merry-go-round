#include "../../includes.hpp"

using TagLib::MPEG::File;
using ID3v2Tag = TagLib::ID3v2::Tag;
using ID3v2PrivateFrame = TagLib::ID3v2::PrivateFrame;

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
	this->resolverMap["ID3v2::APIC"] = std::bind(&MPEGMusicInternal::resolveIdv2AttachedPicture, this, std::placeholders::_1);
	this->resolverMap["ID3v2::PRIV"] = std::bind(&MPEGMusicInternal::resolveIdv2Private, this, std::placeholders::_1);

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
	this->nativeDataStore["dataType"] = "ID3v2";

	std::unordered_map<std::string, size_t> arrayMap;
	const auto& frameList = tag->frameList();
	for (auto* frame : frameList) {
		TagLib::ByteVector byteVector = frame->frameID();
		std::string frameId(byteVector.data(), byteVector.size());

		// we should skip it when duplicated key found.
		if (arrayMap[frameId]) continue;

		// if there is already data on the store with same id, we should collect it all and make it array.
		if (this->nativeDataStore.exists(frameId) && arrayMap[frameId] == 0) {
			// do loop whole frame list again and collect frames with same id.
			NodeArray arr;
			for (auto* targetFrame : frameList) {
				if (targetFrame->frameID() == frameId.c_str()) {
					auto&& holder = this->getHolderFromFrame("ID3v2", targetFrame);
					arr.push_back(holder);
				}
			}

			// renew value as array.
			this->nativeDataStore[frameId] = arr;

			// we should set the flag to prevent make it deep array.
			arrayMap[frameId]++;
		} else {
			// or just add with its own type.
			this->nativeDataStore[frameId] = this->getHolderFromFrame("ID3v2", frame);
		}
	}
}
void MPEGMusicInternal::initializeId3v1(void) {
	this->tagTypeString = "ID3v1";
}
void MPEGMusicInternal::initializeAPE(void) {
	this->tagTypeString = "APE";
}

NodeObject MPEGMusicInternal::resolveIdv2AttachedPicture(ID3v2Frame* _frame) {
	auto* frame = reinterpret_cast<ID3v2AttachedPictureFrame*>(_frame);
	NodeObject object, type;

	type["id"] = static_cast<int>(frame->type());
	type["description"] = MPEGMusicInternal::idv2PictureTypeDictionary[frame->type()];

	object["mimeType"] = frame->mimeType();
	object["description"] = frame->description();
	object["type"] = type;

	auto picture = frame->picture();
	object["data"] = NodeBuffer<char>(picture.data(), picture.size());

	return object;
}
NodeObject MPEGMusicInternal::resolveIdv2Private(ID3v2Frame* _frame) {
	auto* frame = reinterpret_cast<ID3v2PrivateFrame*>(_frame);
	auto data = frame->data();

	NodeObject object;
	NodeBuffer<char> buffer(data.data(), data.size());

	object["owner"] = frame->owner();
	object["data"] = buffer;

	return object;
}

NodeBase::generator_holder MPEGMusicInternal::getHolderFromFrame(const std::string& type, ID3v2Frame* frame) {
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
