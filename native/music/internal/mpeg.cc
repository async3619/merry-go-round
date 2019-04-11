#include "../../includes.hpp"

std::unordered_map<ID3v2AttachedPictureFrame::Type, const char*> MPEGMusicInternal::idv2PictureTypeDictionary = {
	//! A type not enumerated below
	{ ID3v2AttachedPictureFrame::Other, "Other" },
	//! 32x32 PNG image that should be used as the file icon
	{ ID3v2AttachedPictureFrame::FileIcon, "File Icon" },
	//! File icon of a different size or format
	{ ID3v2AttachedPictureFrame::OtherFileIcon, "Other File Icon" },
	//! Front cover image of the album
	{ ID3v2AttachedPictureFrame::FrontCover, "Front Cover" },
	//! Back cover image of the album
	{ ID3v2AttachedPictureFrame::BackCover, "Back Cover" },
	//! Inside leaflet page of the album
	{ ID3v2AttachedPictureFrame::LeafletPage, "Leaflet Page" },
	//! Image from the album itself
	{ ID3v2AttachedPictureFrame::Media, "Media" },
	//! Picture of the lead artist or soloist
	{ ID3v2AttachedPictureFrame::LeadArtist, "Lead Artist" },
	//! Picture of the artist or performer
	{ ID3v2AttachedPictureFrame::Artist, "Artist" },
	//! Picture of the conductor
	{ ID3v2AttachedPictureFrame::Conductor, "Conductor" },
	//! Picture of the band or orchestra
	{ ID3v2AttachedPictureFrame::Band, "Band" },
	//! Picture of the composer
	{ ID3v2AttachedPictureFrame::Composer, "Composer" },
	//! Picture of the lyricist or text writer
	{ ID3v2AttachedPictureFrame::Lyricist, "Lyricist" },
	//! Picture of the recording location or studio
	{ ID3v2AttachedPictureFrame::RecordingLocation, "Recording Location" },
	//! Picture of the artists during recording
	{ ID3v2AttachedPictureFrame::DuringRecording, "During Recording" },
	//! Picture of the artists during performance
	{ ID3v2AttachedPictureFrame::DuringPerformance, "During Performance" },
	//! Picture from a movie or video related to the track
	{ ID3v2AttachedPictureFrame::MovieScreenCapture, "Movie Screen Capture" },
	//! Picture of a large, coloured fish
	{ ID3v2AttachedPictureFrame::ColouredFish, "Coloured Fish" },
	//! Illustration related to the track
	{ ID3v2AttachedPictureFrame::Illustration, "Illustration" },
	//! Logo of the band or performer
	{ ID3v2AttachedPictureFrame::BandLogo, "Band Logo" },
	//! Logo of the publisher (record company)
	{ ID3v2AttachedPictureFrame::PublisherLogo, "Publisher Logo" },
};

MPEGMusicInternal::MPEGMusicInternal(TagLib::File* file) :
	MusicInternal(file), file(reinterpret_cast<TagLib::MPEG::File*>(file)), type(nullptr) {
	idv2Resolvers["APIC"] = std::bind(&MPEGMusicInternal::resolveIdv2AttachedPicture, this, std::placeholders::_1, std::placeholders::_2);
	idv2Resolvers["PRIV"] = std::bind(&MPEGMusicInternal::resolveIdv2Private, this, std::placeholders::_1, std::placeholders::_2);
}
MPEGMusicInternal::~MPEGMusicInternal(void) { }

void MPEGMusicInternal::retrieveID3v2TagData(void) {
	TagLib::ID3v2::Tag* tag = this->file->ID3v2Tag();

	const auto& frameList = tag->frameList();
	for (auto* frame : frameList) {
		string_array_t frameData;

		// add frame id
		char frameId[5] = { 0, };
		std::memcpy(frameId, frame->frameID().mid(0, 4).data(), sizeof(char) * 4);
		frameId[4] = '\0';

		frameData.push_back(NODE_STRING(frameId));

		// add frame data
		if (this->idv2Resolvers.find(frameId) != this->idv2Resolvers.cend()) {
			this->idv2Resolvers[frameId](frameData, frame);
		} else {
			// add data as string
			frameData.push_back(NODE_STRING(frame->toString()));
		}

		// add to data store
		this->data.push_back(frameData);
	}
}
void MPEGMusicInternal::generateNativeData(void) {
	if (!this->data.empty())
		this->data.clear();

	if (this->file->hasID3v2Tag()) {
		this->retrieveID3v2TagData();
	} else {
	}
}

void MPEGMusicInternal::resolveIdv2AttachedPicture(string_array_t& arr, ID3v2Frame* frame) {
	using TagLib::ID3v2::AttachedPictureFrame;

	// APIC - [mimeType, imageType, description, buffer]
	auto* attachedPictureFrame = reinterpret_cast<AttachedPictureFrame*>(frame);

	// add mime type
	arr.push_back(NODE_STRING(attachedPictureFrame->mimeType()));

	// add image type
	arr.push_back(NODE_STRING(MPEGMusicInternal::idv2PictureTypeDictionary[attachedPictureFrame->type()]));

	// add description
	arr.push_back(NODE_STRING(attachedPictureFrame->description()));

	// add buffer
	auto pictureBuffer = attachedPictureFrame->picture();
	auto&& bufferId = BufferManager::getInstance().reserve(pictureBuffer.data(), pictureBuffer.size());

	arr.push_back(NODE_STRING("__merry_go_round_buffer::" + bufferId + "::" + std::to_string(pictureBuffer.size())));
}
void MPEGMusicInternal::resolveIdv2Private(string_array_t& arr, ID3v2Frame* frame) {
	using TagLib::ID3v2::PrivateFrame;

	// PRIV - [ownerIdentifier, data]
	auto* privateFrame = reinterpret_cast<PrivateFrame*>(frame);

	// add owner identifier
	arr.push_back(NODE_STRING(privateFrame->owner()));

	// add data
	auto dataBuffer = privateFrame->data();
	auto&& bufferId = BufferManager::getInstance().reserve(dataBuffer.data(), dataBuffer.size());

	arr.push_back(NODE_STRING("__merry_go_round_buffer::" + bufferId + "::" + std::to_string(dataBuffer.size())));
}

node_string_t MPEGMusicInternal::tagType(void) {
	if (!this->type) {
		if (this->file->hasID3v2Tag()) {
			std::string version("ID3v2.");
			TagLib::ID3v2::Tag* tag = this->file->ID3v2Tag();

			if (!tag)
				goto unknown;

			auto* header = tag->header();
			version += std::to_string(header->majorVersion());

			this->type = NODE_STRING(version.c_str());
		} else if (this->file->hasID3v1Tag()) {
			this->type = NODE_STRING("ID3v1");
		} else if (this->file->hasAPETag()) {
			this->type = NODE_STRING("APE");
		} else {
		unknown:
			this->type = NODE_STRING("Unknown");
		}
	}

	return this->type;
}
native_data_t MPEGMusicInternal::nativeData(void) {
	if (this->data.empty())
		this->generateNativeData();

	return this->data;
}
