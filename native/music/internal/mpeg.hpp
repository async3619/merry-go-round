#ifndef MERRY_GO_ROUND_MPEG_HPP
#define MERRY_GO_ROUND_MPEG_HPP

using ID3v2Frame = TagLib::ID3v2::Frame;
using ID3v2AttachedPictureFrame = TagLib::ID3v2::AttachedPictureFrame;
using ID3v2PrivateFrame = TagLib::ID3v2::PrivateFrame;
using ID3v2ChapterFrame = TagLib::ID3v2::ChapterFrame;
using ID3v2FrameList = TagLib::ID3v2::FrameList;

typedef std::function<NodeObject(ID3v2Frame*)> resolver_t;

class MPEGMusicInternal : public MusicInternal 
{
private:
	static std::unordered_map<ID3v2AttachedPictureFrame::Type, const char*> idv2PictureTypeDictionary;

public:
	MPEGMusicInternal(void) = delete;
	MPEGMusicInternal(const TagLib::FileRef* file);
	~MPEGMusicInternal(void);

private:
	void initialize(void);
	void initializeId3v2(void);
	void initializeId3v1(void);
	void initializeAPE(void);

private: // resolvers
	NodeObject resolveId3v2AttachedPicture(ID3v2Frame* frame);
	NodeObject resolveId3v2Private(ID3v2Frame* frame);
	NodeObject resolveId3v2Chapter(ID3v2Frame* frame);

private:
	NodeObject parseId3v2FrameList(const ID3v2FrameList& frameList);
	NodeBase::generator_holder extractDataFromFrame(const std::string& type, ID3v2Frame* frame);

public:
	virtual const NodeString& tagType(void) override;
	virtual const NodeObject& nativeData(void) override;

private:
	std::unordered_map<std::string, resolver_t> resolverMap;
	NodeObject nativeDataStore;
	NodeString tagTypeString;
	TagLib::MPEG::File* file;
};

#endif // MERRY_GO_ROUND_MPEG_HPP
