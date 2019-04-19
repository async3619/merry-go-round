#include "../../includes.hpp"

#define REGISTER_RESOLVER(frameName, functionName) \
	this->resolverMap["ID3v2::"#frameName] = std::bind(&MPEGMusicInternal::functionName, this, std::placeholders::_1);

void MPEGMusicInternal::registerAllId3v2Resolvers(void) {
	REGISTER_RESOLVER(APIC, resolveId3v2AttachedPicture);
	REGISTER_RESOLVER(PRIV, resolveId3v2Private);
	REGISTER_RESOLVER(CHAP, resolveId3v2Chapter);
	REGISTER_RESOLVER(COMM, resolveId3v2Comments);
	REGISTER_RESOLVER(ETCO, resolveId3v2EventTimeCodes);
	REGISTER_RESOLVER(OWNE, resolveId3v2Ownership);
	REGISTER_RESOLVER(POPM, resolveId3v2Popularimeter);
	REGISTER_RESOLVER(RVA2, resolveId3v2RelativeVolume);
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
NodeObject MPEGMusicInternal::resolveId3v2Ownership(ID3v2Frame* _frame) {
	auto* frame = reinterpret_cast<ID3v2OwnershipFrame*>(_frame);
	NodeObject object;

	object["pricePaid"] = frame->pricePaid();
	object["datePurchased"] = frame->datePurchased();
	object["seller"] = frame->seller();

	return object;
}
NodeObject MPEGMusicInternal::resolveId3v2Popularimeter(ID3v2Frame* _frame) {
	auto* frame = reinterpret_cast<ID3v2PopularimeterFrame*>(_frame);
	NodeObject object;

	object["email"] = frame->email();
	object["rating"] = frame->rating();
	object["counter"] = frame->counter();

	return object;
}
NodeObject MPEGMusicInternal::resolveId3v2RelativeVolume(ID3v2Frame* _frame) {
	auto* frame = reinterpret_cast<ID3v2RelativeVolumeFrame*>(_frame);
	NodeObject object;
	NodeArray channelArray;

	object["identification"] = frame->identification();

	auto channelList = frame->channels();
	for (auto channelType : channelList) {
		NodeObject volume, channel, peak;

		channel["id"] = static_cast<int>(channelType);
		channel["description"] = MPEGMusicInternal::idv2ChannelTypeDictionary[channelType];

		volume["type"] = channel;
		volume["adjustment"] = frame->volumeAdjustment(channelType);
		volume["adjustmentIndex"] = frame->volumeAdjustmentIndex(channelType);

		//TODO: fix error while reading peak volume information
		// auto peakVolume = frame->peakVolume(channelType);
		// peak["bitsRepresenting"] = peakVolume.bitsRepresentingPeak;
		// peak["volume"] = NodeBuffer<char>(peakVolume.peakVolume.data(), peakVolume.peakVolume.size());
		// volume["peak"] = peak;

		channelArray.push_back(volume);
	}

	object["channels"] = channelArray;

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
