#include "../../includes.hpp"

using namespace TagLib;

const char* PICTURE_TYPE_DECL[] = {
	"Other",
	"File icon",
	"Other file icon",
	"Cover (front)",
	"Cover (back)",
	"Leaflet page",
	"Media",
	"Lead artist / Lead performer / Soloist",
	"Artist / Performer",
	"Conductor",
	"Band / Orchestra",
	"Composer",
	"Lyricist / Text writer",
	"Recording location",
	"During recording",
	"During performance",
	"Movie / Video screen capture",
	"A bright coloured fish",
	"Illustration",
	"Band / Artist logotype",
	"Publisher / Studio logotype",
};

MPEGMusicInternal::MPEGMusicInternal(TagLib::File* targetFile) :
	MusicInternal(targetFile), mpegFile(reinterpret_cast<MPEG::File*>(targetFile)), tagTypeName("Unknown") {
	this->composerMap["APIC"] = std::bind(&MPEGMusicInternal::composeAttachedPictureFrame, this, std::placeholders::_1, std::placeholders::_2);
	this->composerMap["PRIV"] = std::bind(&MPEGMusicInternal::composePrivateFrame, this, std::placeholders::_1, std::placeholders::_2);

	if (this->mpegFile->hasAPETag()) {
		tagTypeName = "APE";
	} else if (this->mpegFile->hasID3v2Tag()) {
		this->initializeID3v2();
	} else if (this->mpegFile->hasID3v1Tag()) {
		tagTypeName = "ID3v1";
	}
}
MPEGMusicInternal::~MPEGMusicInternal(void) { }

std::string MPEGMusicInternal::serializeBuffer(TagLib::ByteVector& byteVector) {
	auto bufferKey = BufferManager::getInstance().reserve(byteVector.data(), byteVector.size());
	std::string data = "__merry_go_round_buffer::" + bufferKey + "::" + std::to_string(byteVector.size());

	return data;
}

void MPEGMusicInternal::initializeID3v2(void) {
	ID3v2::Tag* tag = this->mpegFile->ID3v2Tag();
	auto* header = tag->header();

	this->tagTypeName = "ID3v2.";
	this->tagTypeName += std::to_string(header->majorVersion());

	auto frameList = tag->frameList();
	for (ID3v2::Frame* frame : frameList) {
		std::vector<std::string> frameData;

		// add frame id
		size_t frameIdLength = frame->frameID().size() + 1;
		char* frameId = new char[frameIdLength];
		memset(frameId, 0, frameIdLength);
		memcpy(frameId, frame->frameID().data(), frameIdLength - 1);

		frameData.emplace_back(frameId);

		// find composer and if succeeded to find suitable composer to compose its data,
		// compose with it or just make it string.
		auto& composerIterator = this->composerMap.find(frameId);
		if (composerIterator != this->composerMap.end()) {
			composerIterator->second(frameData, frame);
		} else {
			frameData.emplace_back(TO_UTF8(frame->toString().toCString()));
		}

		// frame id data isn't needed anymore, deallocate it.
		delete[] frameId;

		// push it to the result buffer.
		this->native.push_back(frameData);
	}
}

void MPEGMusicInternal::composeAttachedPictureFrame(std::vector<std::string>& frameDataStore, ID3v2::Frame* frame) {
	// APIC - [mimeType, imageType, description, buffer]
	ID3v2::AttachedPictureFrame* attachedPicture = reinterpret_cast<ID3v2::AttachedPictureFrame*>(frame);

	// add mine type
	frameDataStore.emplace_back(attachedPicture->mimeType().toCString());

	// add attached image type
	auto pictureType = attachedPicture->type();
	auto pictureTypeDecl = PICTURE_TYPE_DECL[static_cast<std::size_t>(pictureType)];
	frameDataStore.emplace_back(pictureTypeDecl);

	// add description
	frameDataStore.emplace_back(attachedPicture->description().toCString());

	// add actual image data
	auto imageData = attachedPicture->picture();
	frameDataStore.emplace_back(this->serializeBuffer(imageData));
}

void MPEGMusicInternal::composePrivateFrame(std::vector<std::string>& frameDataStore, ID3v2::Frame* frame) {
	// PRIV - [ownerIdentifier, data]
	ID3v2::PrivateFrame* privateFrame = reinterpret_cast<ID3v2::PrivateFrame*>(frame);

	// add owner identifier
	frameDataStore.emplace_back(privateFrame->owner().toCString());

	// add data
	auto dataBuffer = privateFrame->data();
	frameDataStore.emplace_back(this->serializeBuffer(dataBuffer));
}

native_data_t MPEGMusicInternal::nativeData(void) {
	return this->native;
}

std::string MPEGMusicInternal::tagType(void) {
	return this->tagTypeName;
}
