#ifndef MERRY_GO_ROUND_MPEG_HPP
#define MERRY_GO_ROUND_MPEG_HPP

using ID3v2Tag = TagLib::ID3v2::Tag;
using ID3v2Frame = TagLib::ID3v2::Frame;
using ID3v2AttachedPictureFrame = TagLib::ID3v2::AttachedPictureFrame;

class MPEGMusicInternal : public MusicInternal 
{
private:
	static std::unordered_map<ID3v2AttachedPictureFrame::Type, const char*> idv2PictureTypeDictionary;

private:
	DISALLOW_COPY_AND_ASSIGN(MPEGMusicInternal);
	MPEGMusicInternal(void) = delete;

public:
	MPEGMusicInternal(TagLib::File* file);
	~MPEGMusicInternal(void);

private:
	void retrieveID3v2TagData(void);
	void generateNativeData(void);

private:
	void resolveIdv2AttachedPicture(string_array_t& array, ID3v2Frame* frame);
	void resolveIdv2Private(string_array_t& array, ID3v2Frame* frame);

public:
	virtual node_string_t tagType(void) override;
	virtual native_data_t nativeData(void) override;

private:
	node_string_t type;
	native_data_t data;
	TagLib::MPEG::File* file;

	std::unordered_map<std::string, std::function<void(string_array_t&, ID3v2Frame*)>> idv2Resolvers;
};

#endif // MERRY_GO_ROUND_MPEG_HPP
