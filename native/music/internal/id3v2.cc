#include "../../includes.hpp"

#define REGISTER_RESOLVER(frameName, functionName) \
	this->resolverMap["ID3v2::"#frameName] = std::bind(&MPEGMusicInternal::functionName, this, std::placeholders::_1);

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

std::unordered_map<ID3v2EventTimingCodesFrame::EventType, const char*> MPEGMusicInternal::idv2EventTypeDictionary = {
    { ID3v2EventTimingCodesFrame::Padding, "Padding" },
    { ID3v2EventTimingCodesFrame::EndOfInitialSilence, "End Of Initial Silence" },
    { ID3v2EventTimingCodesFrame::IntroStart, "Intro Start" },
    { ID3v2EventTimingCodesFrame::MainPartStart, "Main Part Start" },
    { ID3v2EventTimingCodesFrame::OutroStart, "Outro Start" },
    { ID3v2EventTimingCodesFrame::OutroEnd, "Outro End" },
    { ID3v2EventTimingCodesFrame::VerseStart, "Verse Start" },
    { ID3v2EventTimingCodesFrame::RefrainStart, "Refrain Start" },
    { ID3v2EventTimingCodesFrame::InterludeStart, "Interlude Start" },
    { ID3v2EventTimingCodesFrame::ThemeStart, "Theme Start" },
    { ID3v2EventTimingCodesFrame::VariationStart, "Variation Start" },
    { ID3v2EventTimingCodesFrame::KeyChange, "Key Change" },
    { ID3v2EventTimingCodesFrame::TimeChange, "Time Change" },
    { ID3v2EventTimingCodesFrame::MomentaryUnwantedNoise, "Momentary Unwanted Noise" },
    { ID3v2EventTimingCodesFrame::SustainedNoise, "Sustained Noise" },
    { ID3v2EventTimingCodesFrame::SustainedNoiseEnd, "Sustained Noise End" },
    { ID3v2EventTimingCodesFrame::IntroEnd, "Intro End" },
    { ID3v2EventTimingCodesFrame::MainPartEnd, "Main Part End" },
    { ID3v2EventTimingCodesFrame::VerseEnd, "Verse End" },
    { ID3v2EventTimingCodesFrame::RefrainEnd, "Refrain End" },
    { ID3v2EventTimingCodesFrame::ThemeEnd, "Theme End" },
    { ID3v2EventTimingCodesFrame::Profanity, "Profanity" },
    { ID3v2EventTimingCodesFrame::ProfanityEnd, "Profanity End" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynch0, "Not Predefined Synch 0" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynch1, "Not Predefined Synch 1" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynch2, "Not Predefined Synch 2" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynch3, "Not Predefined Synch 3" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynch4, "Not Predefined Synch 4" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynch5, "Not Predefined Synch 5" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynch6, "Not Predefined Synch 6" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynch7, "Not Predefined Synch 7" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynch8, "Not Predefined Synch 8" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynch9, "Not Predefined Synch 9" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynchA, "Not Predefined Synch A" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynchB, "Not Predefined Synch B" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynchC, "Not Predefined Synch C" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynchD, "Not Predefined Synch D" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynchE, "Not Predefined Synch E" },
    { ID3v2EventTimingCodesFrame::NotPredefinedSynchF, "Not Predefined Synch F" },
    { ID3v2EventTimingCodesFrame::AudioEnd, "Audio End" },
    { ID3v2EventTimingCodesFrame::AudioFileEnds, "Audio File Ends" },
};

void MPEGMusicInternal::registerAllId3v2Resolvers(void) {
	REGISTER_RESOLVER(APIC, resolveId3v2AttachedPicture);
	REGISTER_RESOLVER(PRIV, resolveId3v2Private);
	REGISTER_RESOLVER(CHAP, resolveId3v2Chapter);
	REGISTER_RESOLVER(COMM, resolveId3v2Comments);
	REGISTER_RESOLVER(ETCO, resolveId3v2EventTimeCodes);
}

NodeObject MPEGMusicInternal::resolveId3v2AttachedPicture(ID3v2Frame* _frame) {
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
NodeObject MPEGMusicInternal::resolveId3v2Private(ID3v2Frame* _frame) {
	auto* frame = reinterpret_cast<ID3v2PrivateFrame*>(_frame);
	auto data = frame->data();

	NodeObject object;
	NodeBuffer<char> buffer(data.data(), data.size());

	object["owner"] = frame->owner();
	object["data"] = buffer;

	return object;
}
NodeObject MPEGMusicInternal::resolveId3v2Chapter(ID3v2Frame* _frame) {
	auto* frame = reinterpret_cast<ID3v2ChapterFrame*>(_frame);
	NodeObject object, time, offset;

	time["start"] = frame->startTime();
	time["end"] = frame->endTime();
	offset["start"] = frame->endTime();
	offset["end"] = frame->endOffset();

	object["time"] = time;
	object["offset"] = offset;

	const auto& embeddedFrames = frame->embeddedFrameList();
	if (embeddedFrames.size() <= 0)
		return object;

	object["embedded"] = this->parseId3v2FrameList(embeddedFrames);

	return object;
}
NodeObject MPEGMusicInternal::resolveId3v2Comments(ID3v2Frame* _frame) {
	auto* frame = reinterpret_cast<ID3v2CommentsFrame*>(_frame);
	NodeObject object;

	const TagLib::ByteVector languageBuffer = frame->language();
	char language[16] = { 0, };

	std::memcpy(language, languageBuffer.data(), languageBuffer.size() * sizeof(char));

	object["language"] = std::string(language);
	object["description"] = frame->description();
	object["text"] = frame->text();

	return object;
}
NodeObject MPEGMusicInternal::resolveId3v2EventTimeCodes(ID3v2Frame* _frame) {
	auto* frame = reinterpret_cast<ID3v2EventTimingCodesFrame*>(_frame);
	NodeObject object, timestampType;
	NodeArray eventArray;

	auto format = frame->timestampFormat();
	auto list = frame->synchedEvents();

	for (auto synchedEvent : list) {
		NodeObject type;
		NodeObject eventItem;

		type["id"] = static_cast<int>(synchedEvent.type);
		type["description"] = MPEGMusicInternal::idv2EventTypeDictionary[synchedEvent.type];

		eventItem["type"] = type;
		eventItem["time"] = synchedEvent.time;

		eventArray.push_back(eventItem);
	}

	timestampType["id"] = static_cast<int>(format);

	switch (format) {
	case TagLib::ID3v2::EventTimingCodesFrame::AbsoluteMpegFrames:
		timestampType["description"] = "Absolute MPEG Frames";
		break;
	
	case TagLib::ID3v2::EventTimingCodesFrame::AbsoluteMilliseconds:
		timestampType["description"] = "Absolute Milliseconds";
		break;

	default:
		timestampType["description"] = "Unknown";
		break;
	}

	object["format"] = timestampType;
	object["events"] = eventArray;

	return object;
}

void MPEGMusicInternal::initializeId3v2(void) {
	auto* tag = this->file->ID3v2Tag();
	auto* header = tag->header();

	this->tagTypeString = "ID3v2." + std::to_string(header->majorVersion());

	this->nativeDataStore = this->parseId3v2FrameList(tag->frameList());
	this->nativeDataStore["dataType"] = "ID3v2";
}

NodeObject MPEGMusicInternal::parseId3v2FrameList(const ID3v2FrameList& frameList) {
	NodeObject result;
	std::unordered_map<std::string, size_t> arrayMap;
	for (auto* frame : frameList) {
		TagLib::ByteVector byteVector = frame->frameID();
		std::string frameId(byteVector.data(), byteVector.size());

		// we should skip it when duplicated key found.
		if (arrayMap[frameId]) continue;

		// if there is already data on the store with same id, we should collect it all and make it array.
		if (result.exists(frameId) && arrayMap[frameId] == 0) {
			// do loop whole frame list again and collect frames with same id.
			NodeArray arr;
			for (auto* targetFrame : frameList) {
				if (targetFrame->frameID() == frameId.c_str()) {
					auto&& holder = this->extractDataFromFrame("ID3v2", targetFrame);
					arr.push_back(holder);
				}
			}

			// renew value as array.
			result[frameId] = arr;

			// we should set the flag to prevent make it deep array.
			arrayMap[frameId]++;
		} else {
			// or just add with its own type.
			result[frameId] = this->extractDataFromFrame("ID3v2", frame);
		}
	}

	return result;
}
