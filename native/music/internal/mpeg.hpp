#ifndef MERRY_GO_ROUND_MPEG_HPP
#define MERRY_GO_ROUND_MPEG_HPP

class MPEGMusicInternal : public MusicInternal 
{
private:
	DISALLOW_COPY_AND_ASSIGN(MPEGMusicInternal);
	MPEGMusicInternal(void) = delete;

public:
	MPEGMusicInternal(TagLib::File*);
	~MPEGMusicInternal(void);

private:
	std::string serializeBuffer(TagLib::ByteVector& byteVector);
	void initializeID3v2(void);

private:
	void composeAttachedPictureFrame(std::vector<std::string>& frameDataStore, TagLib::ID3v2::Frame* frame);
	void composePrivateFrame(std::vector<std::string>& frameDataStore, TagLib::ID3v2::Frame* frame);

public:
	virtual std::string tagType(void) override;
	virtual native_data_t nativeData(void) override;

private:
	std::unordered_map<std::string, std::function<void(std::vector<std::string>&, TagLib::ID3v2::Frame*)>> composerMap;
	std::string tagTypeName;
	TagLib::MPEG::File* mpegFile;
	native_data_t native;
};

#endif // MERRY_GO_ROUND_MPEG_HPP
