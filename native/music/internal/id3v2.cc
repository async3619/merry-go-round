#include "../../includes.hpp"

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
