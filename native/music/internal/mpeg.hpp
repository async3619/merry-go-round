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
	void initializeID3v2(void);

public:
	virtual std::string tagType(void) override;
	virtual native_data_t nativeData(void) override;

private:
	std::string tagTypeName;
	TagLib::MPEG::File* mpegFile;
	native_data_t native;
};

#endif // MERRY_GO_ROUND_MPEG_HPP
