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
	if (this->mpegFile->hasAPETag()) {
		tagTypeName = "APE";
	} else if (this->mpegFile->hasID3v2Tag()) {
		this->initializeID3v2();
	} else if (this->mpegFile->hasID3v1Tag()) {
		tagTypeName = "ID3v1";
	}
}
MPEGMusicInternal::~MPEGMusicInternal(void) { }

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
		delete[] frameId;

		// APIC - [mimeType, imageType, description, bufferKey, bufferSize]
		if (frame->frameID() == "APIC") {
			ID3v2::AttachedPictureFrame* attachedPicture = reinterpret_cast<ID3v2::AttachedPictureFrame*>(frame);

			// add mine type
			frameData.emplace_back(attachedPicture->mimeType().toCString());

			// add attached image type
			auto pictureType = attachedPicture->type();
			auto pictureTypeDecl = PICTURE_TYPE_DECL[static_cast<std::size_t>(pictureType)];
			frameData.emplace_back(pictureTypeDecl);

			// add description
			frameData.emplace_back(attachedPicture->description().toCString());

			// add actual image data buffer key
			auto imageData = attachedPicture->picture();
			auto bufferKey = BufferManager::getInstance().reserve(imageData.data(), imageData.size());

			frameData.emplace_back(bufferKey);

			// add image data size
			frameData.emplace_back(std::to_string(imageData.size()));
		} else {
			frameData.emplace_back(TO_UTF8(frame->toString().toCString()));
		}

		this->native.push_back(frameData);
	}
}
native_data_t MPEGMusicInternal::nativeData(void) {
	return this->native;
}

std::string MPEGMusicInternal::tagType(void) {
	return this->tagTypeName;
}

#include <nbind/nbind.h>

NBIND_CLASS(MPEGMusicInternal) {
	method(nativeData);
	method(tagType);
}
